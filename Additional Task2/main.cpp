#include <utility>
#include <string>
#include <iostream>

template<typename T>
class UniquePtr
{
public:
    UniquePtr () = default;

    explicit UniquePtr (T *ptr) :
            pointer(ptr) {}

    UniquePtr (UniquePtr<T> &&other) noexcept
    {
        pointer = other.pointer;
        other.pointer = nullptr;
    }

    ~UniquePtr ()
    {
        if (pointer != nullptr) {
            delete pointer;
        }
        pointer = nullptr;
    }

    T &operator* () const
    {
        return *pointer;
    }

    UniquePtr &operator= (UniquePtr<T> &&other) noexcept
    {
        if (*this != other) {
            delete pointer;
            pointer = std::move(other.pointer);
            other.pointer = nullptr;
        }
        return *this;
    }

    T *operator-> () const
    {
        return pointer;
    }

    UniquePtr (const UniquePtr<T> &other) = delete;

    UniquePtr &operator= (const UniquePtr<T> &other) = delete;

    T* getPointer () const
    {
        return pointer;
    }

private:
    T *pointer;
};

template<typename T, typename... Args>
UniquePtr<T> makeUnique (Args &&... args)
{
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename U>
bool operator== (const UniquePtr<T> &first, const UniquePtr<U> &second)
{
    return first.getPointer() == second.getPointer();
}

template<typename T, typename U>
bool operator!= (const UniquePtr<T> &first, const UniquePtr<U> &second)
{
    return first.getPointer() != second.getPointer();
}


struct Vec3
{
    int x, y, z;
    Vec3() : x(0), y(0), z(0) { }
    Vec3(int x, int y, int z) :x(x), y(y), z(z) { }
    friend std::ostream& operator<<(std::ostream& os, Vec3& v) {
        return os << '{' << "x:" << v.x << " y:" << v.y << " z:" << v.z  << '}';
    }
};

int main ()
{
    auto *a = new std::string("aaa");
    UniquePtr<std::string> ptr(a);
    UniquePtr<std::string> ptr2(new std::string());
    std::cout << "Ptr address before move:" << ptr.getPointer() << std::endl;
    std::cout << "Ptr2 address before move:" << ptr2.getPointer() << std::endl;
    ptr2 = std::move(ptr);
    std::cout << "Ptr address after move:" << ptr.getPointer() << std::endl;
    std::cout << "Ptr2 address after move:" << ptr2.getPointer() << std::endl;
    auto ptr3 = a;
    std::cout << "before, a: " << *a << std::endl;
    // *ptr += "bbb";
    std::cout << "After, a: " << *a << std::endl;
    *ptr3 += "ccc";
    std::cout << "after 2, a: " << *a << std::endl;


    UniquePtr<Vec3> v1 = makeUnique<Vec3>();
    UniquePtr<Vec3> v2 = makeUnique<Vec3>(0, 1, 2);

    std::cout << "make_unique<Vec3>():      " << *v1 << '\n'
              << "make_unique<Vec3>(0,1,2): " << *v2 << '\n';
}