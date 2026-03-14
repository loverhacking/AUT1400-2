#ifndef UNIQUE_PTR
#define UNIQUE_PTR

template <typename T>
class UniquePtr
{
    private:
        T* ptr;

    public:

        UniquePtr();
        UniquePtr(T* ptr);

        // copy constructor and copy assignment operator are deleted to prevent copying
        UniquePtr(const UniquePtr& other) = delete;
        UniquePtr& operator=(const UniquePtr& other) = delete;

        UniquePtr(UniquePtr&& other) noexcept; // move constructor
        UniquePtr& operator=(UniquePtr&& other) noexcept; // move assignment operator

        ~UniquePtr();

        // explicit boolean conversion operator
        explicit operator bool() const noexcept;

        // return the raw pointer stored in the class
        T* get() const;

        // dereferenced exactly like raw pointer
        T& operator*() const;

        // use the arrow operator like normal pointers
        T* operator->() const;

        // delete the ponter and assign nullptr to it
        void reset();

        // make a new pointer after deleting the old pointer
        void reset(T* ptr);

        // returns the stored pointer in the clas
        // The UniquePtr class won't store the pointer anymore
        // and also deleting the pointer should not be done after calling release()
        T* release();
};

template<typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args);

#include "unique_ptr.hpp"



#endif //UNIQUE_PTR