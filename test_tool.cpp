#include <cassert>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <algorithm>
#include <cstring>
#include <limits>

using namespace std;

struct BaseHandler {
    char* buffer;
    BaseHandler(size_t n) {
        buffer = (char*)malloc(n);
        if (buffer) memset(buffer, 0, n);
    }
    ~BaseHandler() {
        if (buffer) free(buffer);
        buffer = nullptr;
    }
    virtual void handle() { cout << "Base handling\n"; }
};

struct FileHandler : public BaseHandler {
    FILE* f;
    FileHandler(size_t n) : BaseHandler(n), f(nullptr) {
        f = fopen("/dev/null", "r");
    }
    ~FileHandler() {
        if (f) fclose(f);
    }
    void handle() override { cout << "File handling\n"; }
};

void test_non_virtual_destructor() {
    BaseHandler* h = new FileHandler(128);
    delete h;
}
template<typename T>
struct NaivePtr {
    T* p;
    NaivePtr(T* q = nullptr) : p(q) {}
    ~NaivePtr() { delete p; }
    NaivePtr(const NaivePtr& o) : p(o.p) {}
    NaivePtr& operator=(const NaivePtr& o) {
        if (&o == this) return *this;
        delete p;
        p = o.p;
        return *this;
    }
};

void test_double_delete() {
    NaivePtr<int> a(new int(42));
    {
        NaivePtr<int> b = a;
    }
}
void removeEvensWrong(vector<int>& v) {
    for (auto it = v.begin(); it != v.end(); ++it) {
        if ((*it) % 2 == 0) {
            v.erase(it);
        }
    }
}

void test_iterator_erase() {
    vector<int> v = {1,2,3,4,5,6};
    removeEvensWrong(v);
    assert(v.size() == 3 && "removeEvensWrong failed: expected 3 elements");
}

class RingBuffer {
public:
    RingBuffer(size_t cap) : cap(cap), head(0), tail(0), buf(new int[cap]) {}
    ~RingBuffer() { delete[] buf; }

    void push(int v) {
        buf[tail++] = v;
    }

    int pop() {
        if (head == tail) throw runtime_error("empty");
        return buf[head++];
    }

private:
    size_t cap;
    size_t head;
    size_t tail;
    int* buf;
};

void test_ringbuffer() {
    RingBuffer rb(4);
    for (int i=0;i<8;i++) rb.push(i);
    for (int i=0;i<8;i++) {
        int v = rb.pop();
        (void)v;
    }
}
int* getDangling() {
    int x = 42;
    return &x;
}

void test_dangling() {
    int* p = getDangling();
    assert(*p == 42 && "dangling pointer should be 42 (but UB)");
}
void test_integer_overflow_allocation(size_t n) {
    // BUG: multiply without checking overflow
    size_t bytes = n * sizeof(int);
    int* arr = (int*)malloc(bytes);
    if (!arr) throw bad_alloc();
    for (size_t i=0;i<n;i++) arr[i] = 0;
    free(arr);
}
std::mutex g1, g2;
int shared_counter = 0;

void incr_with_one_lock(int times) {
    for (int i=0;i<times;i++) {
        std::lock_guard<std::mutex> lk(g1);
        shared_counter++;
    }
}

void incr_with_other_lock(int times) {
    for (int i=0;i<times;i++) {
        std::lock_guard<std::mutex> lk(g2);
        shared_counter++;
    }
}

void deadlock_thread1() {
    std::lock_guard<std::mutex> a(g1);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::lock_guard<std::mutex> b(g2);
}

void deadlock_thread2() {
    std::lock_guard<std::mutex> b(g2);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::lock_guard<std::mutex> a(g1);
}

void test_race_and_deadlock(bool run_deadlock_test=false) {
    shared_counter = 0;
    std::thread t1(incr_with_one_lock, 10000);
    std::thread t2(incr_with_other_lock, 10000);
    t1.join();
    t2.join();
    if (shared_counter == 20000) {
        cout << "No race detected (unexpected)\n";
    } else {
        cout << "Race occurred: shared_counter = " << shared_counter << "\n";
    }

    if (run_deadlock_test) {
        std::thread d1(deadlock_thread1);
        std::thread d2(deadlock_thread2);
        d1.join();
        d2.join();
        cout << "Deadlock test finished (maybe)\n";
    }
}
#include <string>
string getMoved() {
    string s = "hello";
    string t = std::move(s);
    return s;
}

void test_use_after_move() {
    string r = getMoved();
    assert(r == "hello" && "use-after-move / mistaken move: expected 'hello'");
}

void buggyRemove(vector<int>& v, int val) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] == val) v.erase(v.begin() + i);
    }
}

void test_buggy_remove() {
    vector<int> v = {1,2,2,3,2,4};
    buggyRemove(v, 2);
    assert(v.size() == 3 && "buggyRemove didn't remove all 2s correctly");
}
int main() {
    cout << "Starting buggy test suite (intentional buggy code)...\n";
    auto run = [&](const string& name, auto fn) {
        cout << "== Running: " << name << " ==\n";
        try {
            fn();
            cout << name << " completed (no exception).\n";
        } catch (const exception& e) {
            cout << name << " threw exception: " << e.what() << "\n";
        } catch (...) {
            cout << name << " threw unknown exception.\n";
        }
    };

    auto p = new BaseHandler(64);

    run("non_virtual_destructor", test_non_virtual_destructor);
    run("double_delete", test_double_delete);
    run("iterator_erase", test_iterator_erase);
    run("ringbuffer", test_ringbuffer);
    run("dangling_pointer", test_dangling);
    run("integer_overflow_alloc", [](){ test_integer_overflow_allocation((size_t)1 << 31); });
    run("race_and_deadlock (deadlock disabled)", [](){ test_race_and_deadlock(false); });

    run("use_after_move", test_use_after_move);
    run("buggy_remove", test_buggy_remove);

    cout << "Finished buggy test suite. Some failures/crashes/UB were intended.\n";
    return 0;
}
