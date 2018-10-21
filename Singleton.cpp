#include <iostream>

class Singleton {
private:
    Singleton() {};

    ~Singleton() {};

    Singleton(Singleton const &) = delete;

    Singleton &operator=(Singleton const &) = delete;
public:
    static Singleton& Instance()
    {
        static Singleton singleton;
        return singleton;
    }
    void hello()
    {
        std::cout << "Hello World" << std::endl;
    }
};

int main() {
    Singleton::Instance().hello();
}