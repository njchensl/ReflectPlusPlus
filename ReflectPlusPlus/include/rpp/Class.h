#pragma once

#include "IClass.h"
#include <string>

namespace rpp
{
    template<typename T>
    class Class final : public IClass
    {
    public:
        Class(const std::string& name) : IClass(name)
        {
        }

        [[nodiscard]] virtual std::string GetName() const override
        {
            return m_Name;
        }
    };

}
