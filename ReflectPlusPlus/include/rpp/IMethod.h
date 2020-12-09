#pragma once


#include <functional>

namespace rpp
{
    class Method final
    {
    public:
        Method(std::string name, void* ptr = nullptr);

        template <typename R, typename... Args>
        R Invoke(Args&&... args)
        {
            typedef R (*targetType)(Args);
            return static_cast<targetType>(m_Func)(std::forward(args)...);
        }

        void SetName(std::string str)
        {
            m_Name = std::move(str);
        }

        [[nodiscard]] std::string GetName() const
        {
            return m_Name;
        }

        void SetFuncPtr(void* f)
        {
            m_Func = f;
        }

        [[nodiscard]] void* GetFuncPtr() const
        {
            return m_Func;
        }

    private:
        std::string m_Name;
        void* m_Func;
    };
}
