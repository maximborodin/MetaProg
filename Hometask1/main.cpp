#include <iostream>
#include <vector>

class InterfaceB
{
public:
    virtual ~InterfaceB() {};
    virtual void func() = 0;
};


class InterfaceObserver
{
public:
    virtual ~InterfaceObserver() {};
    virtual void approveNotification() = 0;
};


class InterfaceObservable
{
public:
    virtual ~InterfaceObservable() {};
    virtual void subscribe(InterfaceObserver* newSubscriber) = 0;
};


class B : public InterfaceB
{
public:
    explicit B(size_t id_) :
        id(id_),
        observer(nullptr)
    {}

    void func() override
    {
        std::cout << "Class B, ID " << id << std::endl;
        if (observer != nullptr) {
            observer ->approveNotification();
        }
    }

    void addObserver(InterfaceObserver* observer_)
    {
        observer = observer_;
    }
private:
    size_t id;
    InterfaceObserver* observer;
};


enum Patterns { Mediator, Proxy, Observer};

template <Patterns Type>
class C {};


/*
 * Выбирает следующий объект класса B и вызывает у него нужную функцию
 */
template <>
class C<Patterns::Mediator> : public InterfaceB
{
public:
    explicit C(std::vector<B*>& objects) :
        b_objects(objects)
    {}

    void func() override
    {
        std::cout << "Playing guitar solo" << std::endl;
        b_objects[counter++]->func();
        counter %= b_objects.size();
    }
private:
    std::vector<B*> b_objects;
    int counter = 0;
};


template <>
class C<Patterns::Proxy> : public InterfaceB
{
public:
    explicit C(InterfaceB* b_object_) :
        b_object(b_object_)
    {}

    void func() override
    {
        std::cout << "Proxy intercepted" << std::endl;
        b_object->func();

    }
private:
    InterfaceB* b_object;
};


/*
 * При создании Observer к классу Б добавляется слушатель. При изменении объекта класса B (вызове функции func()),
 * он уведомляет Observer
 *
 * Подтверждение получения сообщения (approveNotification()) у обсервера состоит в том, что он запрашивает подтверждение
 * получения сообщения у всех своих слушателей.
 */
template <>
class C<Patterns::Observer> : public InterfaceObserver, public InterfaceObservable
{
public:
    explicit C(B* observedTarget_) :
        observedTarget(observedTarget_)
    {
        observedTarget->addObserver(this);
    }

    void subscribe(InterfaceObserver* newSubscriber) override
    {
        subscribers.push_back(newSubscriber);
    }

    void approveNotification() override
    {
        std::cout << "Observer is notifying every subscriber" << std::endl;
        for (auto &subscriber : subscribers) {
            subscriber->approveNotification();
        }
    }
private:
    B* observedTarget;
    std::vector<InterfaceObserver*> subscribers;
};


class InterfaceA
{
public:
    virtual ~InterfaceA() {};
    virtual void callServant(InterfaceB* servant) = 0;
};

/*
 * Экземпляр класса умеет подзывать официанта (изначально, это экземпляр класс B)
 * И как слушатель, умеет подтверждать получение уведомления
 */
class A : public InterfaceA, public InterfaceObserver
{
public:
    explicit A(size_t id_) :
        id(id_)
    {}
    void callServant(InterfaceB* servant) override
    {
        std::cout << "ID " << id << " is calling servant" << std::endl;
        servant ->func();
    }

    void approveNotification() override
    {
        std::cout << "Notification recieved ID " << id << std::endl;
    }
private:
    size_t id;
};



int main ()
{
    B b1(1), b2(2), b3(3);
    A a1(1), a2(2), a3(3);

    // проверка медиатора: класс A зовет официанта 4 раза. Приходят
    std::vector<B*> pointers;
    pointers.push_back(&b1);
    pointers.push_back(&b2);
    C<Patterns::Mediator> mediator(pointers);
    a1.callServant(&mediator);
    a1.callServant(&mediator);
    a1.callServant(&mediator);
    a1.callServant(&mediator);


    // проверка observer
    C<Patterns::Observer> observer(&b1);
    observer.subscribe(&a2);
    observer.subscribe(&a3);
    b1.func();


    // проверка proxy
    C<Patterns::Proxy> proxy(&b2);
    a3.callServant(&proxy);
    return 0;
}