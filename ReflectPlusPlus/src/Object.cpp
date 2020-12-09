#include "Object.h"
#include "Class.h"

namespace rpp
{
    IClass* Object::s_Class = new Class<Object>("rpp::Object");

    bool Object::Equals(const Object* other) const
    {
        return this == other;
    }

    const IClass* Object::GetClass() const
    {
        _ASSERT(s_Class != nullptr);
        return s_Class;
    }
}
