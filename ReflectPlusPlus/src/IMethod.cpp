#include "IMethod.h"

namespace rpp
{
    Method::Method(std::string name, void* ptr) : m_Name(std::move(name)), m_Func(ptr)
    {
    }
}
