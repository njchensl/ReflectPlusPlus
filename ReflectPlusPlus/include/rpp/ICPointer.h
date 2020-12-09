#pragma once
#include <memory>

namespace rpp
{
    class ICPointer
    {
    public:
        virtual ~ICPointer() = default;
        [[nodiscard]] virtual void* Get() const = 0;
        virtual void Delete() const = 0;
    };

    template <typename T>
    class CPointer final : public ICPointer
    {
    public:
        CPointer(T* ptr = nullptr) : m_Ptr(ptr), m_OriginalPtr(ptr)
        {
        }

        [[nodiscard]] virtual void* Get() const override
        {
            return m_Ptr;
        }

        T* operator->()
        {
            return m_Ptr;
        }

        const T* operator->() const
        {
            return m_Ptr;
        }

        virtual void Delete() const override
        {
            free(m_OriginalPtr);
        }

        template <typename U>
        operator CPointer<U>();

        template <typename R, typename U>
        friend CPointer<R> StaticPointerCast(CPointer<U> in);
    private:
        T* m_Ptr;
        void* m_OriginalPtr;
    };

    template <typename R, typename U>
    CPointer<R> StaticPointerCast(CPointer<U> in)
    {
        auto newPtr = CPointer<R>(static_cast<R*>(in.m_Ptr));
        newPtr.m_OriginalPtr = in.m_OriginalPtr;
        return newPtr;
    }

    template <typename T>
    template <typename U>
    CPointer<T>::operator CPointer<U>()
    {
        return StaticPointerCast<U>(*this);
    }
}
