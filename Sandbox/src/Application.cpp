#include <iostream>
#include <rpp/ReflectPlusPlus.hpp>


#include "rpp/ICPointer.h"
#include "rpp/StaticExec.h"

using namespace rpp;


class A : public Object
{
CLASS(A, "rpp::Object")
public:
    int a = 10;
};

REG_CLASS(A)


class B : public A
{
CLASS(B, "A")
public:
    int b = 20;
};

REG_CLASS(B)

class X : public Object
{
CLASS(X, "rpp::Object")
public:
    int x = 30;
};

REG_CLASS(X)

class Y : public X
{
CLASS(Y, "X")
public:
    int y;
};

REG_CLASS(Y)

interface Runnable
{
INTERFACE(Runnable)
    virtual void run() = 0;
};

REG_INTERFACE(Runnable)

class Thread : public Object, public Runnable
{
CLASS(Thread, "rpp::Object;Runnable")
public:
    virtual void run() override
    {
        std::cout << "Run" << std::endl;
    }
};

REG_CLASS(Thread)


int main()
{
    ClassManager::CheckClassHierarchy();

    CPointer<A> a = ClassManager::New<A>();
    CPointer<B> b = ClassManager::New<B>();
    //CPointer<A> r = ClassManager::New<Thread>(); // this will not compile because static_cast<A*>(Thread*) does not work!
    CPointer<Runnable> r = ClassManager::New<Thread>(); // implicit upcast utilizing static casts
    assert(a->InstanceOf<A>());
    assert(!a->InstanceOf<B>());
    assert(b->InstanceOf<Object>());
    assert(Object::StaticInstanceOf(r.operator->(), Runnable::GetStaticType()));

    a.Delete();
    b.Delete();
    r.Delete();

    return 0;
}
