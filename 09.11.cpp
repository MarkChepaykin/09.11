#include <iostream>

using namespace std;

template <typename T>
class MyUniquePtr {
private:
    T* ptr;

public:
    explicit MyUniquePtr(T* p = nullptr) : ptr(p) {}

    ~MyUniquePtr() {
        delete ptr;
    }

    MyUniquePtr(const MyUniquePtr&) = delete;
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;

    MyUniquePtr(MyUniquePtr&& moving) noexcept : ptr(moving.ptr) {
        moving.ptr = nullptr;
    }

    MyUniquePtr& operator=(MyUniquePtr&& moving) noexcept {
        if (!this != moving) {
            delete ptr;
            ptr = moving.ptr;
            moving.ptr = nullptr;
        }
        return *this;
    }

    T& operator*() const {
        return *ptr;
    }

    T& release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T* p = nullptr) {
        T* old = ptr;
        ptr = p;
        if (old) {
            delete old;
        }
    }

    T* get() const {
        return ptr;
    }


};

template <typename T>
class MySharedPtr {
private:
    T* ptr;
    unsigned* count; 

public:

    explicit MySharedPtr(T* p = nullptr) : ptr(p), count(new unsigned(1)) {}

    MySharedPtr(const MySharedPtr& other) : ptr(other.ptr), count(other.count) {
        (*count)++;
    }

    MySharedPtr& operator=(const MySharedPtr& other) {
        if (this != &other) {
            if (--(*count) == 0) {
                delete ptr;
                delete count;
            }
            ptr = other.ptr;
            count = other.count;
            (*count)++;
        }
        return *this;
    }

    ~MySharedPtr() {
        if (--(*count) == 0) {
            delete ptr;
            delete count;
        }
    }

    T& operator*() const {
        return *ptr;
    }

    T* operator->() const {
        return ptr;
    }

    T* get() const {
        return ptr;
    }

    unsigned use_count() const {
        return *count;
    }
};




int main()
{
    MyUniquePtr<int> uniquePtr(new int(10));
    cout << "uniquePtr value: " << *uniquePtr << endl;

    MySharedPtr<int> sharedPtr1(new int(20));
    MySharedPtr<int> sharedPtr2 = sharedPtr1;
    cout << "sharedPtr1 value: " << *sharedPtr1 << ", use_count: " << sharedPtr1.use_count() << std::endl;
    cout << "sharedPtr2 value: " << *sharedPtr2 << ", use_count: " << sharedPtr2.use_count() << std::endl;
}
