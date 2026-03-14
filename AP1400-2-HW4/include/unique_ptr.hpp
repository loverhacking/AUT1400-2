#include "unique_ptr.h"

template <typename T>
UniquePtr<T>::UniquePtr() : ptr(nullptr) {}

template <typename T>
UniquePtr<T>::UniquePtr(T* ptr) : ptr(ptr) {}

template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
    other.ptr = nullptr; // set the other pointer to nullptr to prevent double deletion
}

template <typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) noexcept {
    if (this != &other) { // check for self-assignment
        delete ptr;
        ptr = other.ptr; // transfer ownership of the pointer
        other.ptr = nullptr; // set the other pointer to nullptr to prevent double deletion
    }
    return *this;
}

template <typename T>
UniquePtr<T>::~UniquePtr() {
    delete ptr;
}

template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)... ));
}

template <typename T>
UniquePtr<T>::operator bool() const noexcept {
    return ptr != nullptr;
}

template <typename T>
T* UniquePtr<T>::get() const {
    return ptr;
}

template <typename T>
T& UniquePtr<T>::operator*() const {
    return *ptr;
}

template <typename T>
T* UniquePtr<T>::operator->() const {
    return ptr;
}

template <typename T>
void UniquePtr<T>::reset() {
    delete ptr;
    ptr = nullptr;
}

template <typename T>
void UniquePtr<T>::reset(T* ptr) {
    delete this->ptr; // delete the old pointer
    this->ptr = ptr; // assign the new pointer
}

template <typename T>
T* UniquePtr<T>::release() {
    T* new_Ptr = std::move(ptr);
    ptr = nullptr;
    return new_Ptr;
}