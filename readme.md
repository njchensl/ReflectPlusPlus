# ReflectPlusPlus
### A simple reflection library for C++

#### UPDATE 08.10.2020: Multiple inheritance is officially BANNED from ReflectPlusPlus. Please call ClassManager::CheckClassHierarchy() at the start of int main(); to ensure that you are not using multiple inheritance. To compensate for that, interfaces have been added, and there is no limit as to how many interfaces you can implement in a class.
#### UPDATE 09.10.2020: All classes MUST be trivially destructible.

## Examples
```c++
interface Runnable
{
INTERFACE(Runnable)
    virtual void run() = 0;
};

REG_INTERFACE(Runnable)
```

```c++
class RunnableImpl : public Object, public Runnable
{
CLASS(RunnableImpl, "rpp::Object;Runnable")
public:
    virtual void run() override
    {
        std::cout << "Run" << std::endl;
    }
};

REG_CLASS(RunnableImpl)
```
```c++
int main()
{
    ClassManager::CheckClassHierarchy();

    CPointer<A> a = ClassManager::New<A>();
    CPointer<B> b = ClassManager::New<B>();
    //CPointer<A> r = ClassManager::New<Thread>(); // this will not compile because static_cast<A*>(Thread*) does not work!
    CPointer<Runnable> r = ClassManager::New<RunnableImpl>(); // implicit upcast utilizing static casts
    assert(a->InstanceOf<A>());
    assert(!a->InstanceOf<B>());
    assert(b->InstanceOf<Object>());
    assert(Object::StaticInstanceOf(r.operator->(), Runnable::GetStaticType()));

    a.Delete();
    b.Delete();
    r.Delete(); // free will be called on RunnableImpl instead of Runnable

    return 0;
}

```
