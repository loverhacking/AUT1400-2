#ifndef SHARED_PTR
#define SHARED_PTR

template <typename T>
class SharedPtr {
    private:
        T* _p;
        int* _count;
    
    public:
        SharedPtr();
        SharedPtr(T* ptr);

        // copy constructor and assignment operator
        SharedPtr(const SharedPtr& other);
        SharedPtr& operator=(const SharedPtr& other);

        // move constructor and assignment operator
        SharedPtr(SharedPtr&& other) noexcept;
        SharedPtr& operator=(SharedPtr&& other) noexcept;

        ~SharedPtr();

        // explicit boolean conversion operator
        explicit operator bool() const noexcept;

        // return the number of instances pointing to the same memory location
        int use_count() const;

        // return the raw pointer stored in the class
        T* get() const;

        // dereferenced exactly like raw pointers
        T& operator*() const;

        // use the arrow operator like raw pointers
        T* operator->() const;

        // delete the pointer and assigns nullptr to it
        void reset();

        // make a new pointer after deleting the old pointer
        void reset(T* ptr);

        
    
};

template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args);

#include "shared_ptr.hpp"

#endif //SHARED_PTR