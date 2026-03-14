#include "shared_ptr.h"

template <typename T> 
SharedPtr<T>::SharedPtr() : _p(nullptr), _count(new int(0)) {}

template <typename T> 
SharedPtr<T>::SharedPtr(T* ptr) : _p(ptr), _count(new int(1)) {}

template <typename T> 
SharedPtr<T>::SharedPtr(const SharedPtr& other) : _p(other._p), _count(other._count) {
    (*_count)++;
}

template <typename T> 
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other) {
    if (this != &other) {
        _p = other._p;
        _count = other._count;
        (*_count)++;
    }
    return *this;
}

template <typename T> 
SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept : _p(other._p), _count(other._count) {
    other._p = nullptr;
    other._count = nullptr;  // set the other pointer to nullptr to prevent double deletion
}

template <typename T> 
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr&& other) noexcept {
    if (this != &other) {
        // release the current resource
        (*_count)--;
        if (*_count == 0) {
            delete _p;
            delete _count;
        }
        
        _p = other._p;
        _count = other._count;
        other._p = nullptr;
        other._count = nullptr;  // set the other pointer to nullptr to prevent double deletion
    }
    return *this;
}

template <typename T> 
SharedPtr<T>::~SharedPtr() {
    (*_count)--;
    if (*_count == 0) {
        delete _p;
        _p = nullptr;
    }
}

template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}


template <typename T> 
SharedPtr<T>::operator bool() const noexcept {
    return _p != nullptr;
}

template <typename T> 
int SharedPtr<T>::use_count() const {
    return *_count;
}

template <typename T> 
T* SharedPtr<T>::get() const {
    return _p;
}

template <typename T> 
T& SharedPtr<T>::operator*() const {
    return *_p;
}

template <typename T> 
T* SharedPtr<T>::operator->() const {
    return _p;
}

template <typename T> 
void SharedPtr<T>::reset() {
    delete _p;
    _p = nullptr;
    (*_count) = 0;
}

template <typename T> 
void SharedPtr<T>::reset(T* ptr) {
    delete _p;
    _p = ptr;
    (*_count) = 1;
}
