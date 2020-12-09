#pragma once

#include "IClass.h"
#include "StaticExec.h"
#include <cassert>
#include <stdexcept>
#include <unordered_set>


#include "Class.h"

#define interface struct

namespace rpp
{
    struct EmptyInterface
    {
    };

#define CLASS(name, baseNames) \
    private: \
    inline static IClass* s_Class = nullptr; \
    static void ensureClassData##name##Initialized() { while (!s_Class) { s_Class = new Class<name>(#name); } } \
    static void ensureClassData##name##Singleton() { assert(ClassManager::GetClass(#name) == s_Class); } \
    inline static StaticExec s_Initialize##name = StaticExec([]() -> void { \
    ensureClassData##name##Initialized(); \
    std::string names = baseNames;        \
    std::string delimiter = ";"; \
    if (names.empty()) return; \
    size_t pos = 0;\
    while ((pos = names.find(delimiter)) != std::string::npos) {\
        std::string token = names.substr(0, pos);\
        names.erase(0, pos + delimiter.length());\
        s_Class->AddBaseClass(token);\
    }\
    s_Class->AddBaseClass(names); \
    }); \
    friend void initInfo##name##fn();\
    \
    public: \
    static const IClass* GetStaticType() { ensureClassData##name##Initialized(); ensureClassData##name##Singleton(); return s_Class; } \
    [[nodiscard]] virtual const IClass* GetClass() const { ensureClassData##name##Initialized(); ensureClassData##name##Singleton(); return s_Class; }\
    private:

#define INTERFACE(x) CLASS(x, "") \
    public: \

#define REG_CLASS(name) \
    static void initInfo##name##fn() {\
    name :: ensureClassData##name##Initialized(); \
    name :: s_Class->SetSize(sizeof(name));\
    }\
    static StaticExec s_InitInfo##name = StaticExec(initInfo##name##fn);\
    static_assert(std::is_trivially_destructible<name>::value);

#define REG_INTERFACE(x) REG_CLASS(x)

    class Object
    {
    public:
        Object() = default;


        virtual bool Equals(const Object* other) const;

        template <typename T>
        [[nodiscard]] bool InstanceOf() const
        {
            std::unordered_set<const IClass*> visited;
            return internalInstanceOf(GetClass(), T::GetStaticType(), visited);
        }

        [[nodiscard]] bool InstanceOf(const std::string& name) const
        {
            const IClass* targetType = ClassManager::GetClass(name);
            if (!targetType)
            {
                return false;
            }
            std::unordered_set<const IClass*> visited;
            return internalInstanceOf(GetClass(), targetType, visited);
        }

        [[nodiscard]] virtual const IClass* GetClass() const;

        [[nodiscard]] virtual std::string ToString() const
        {
            return "";
        }

        [[nodiscard]] static const Class<Object>* GetStaticType()
        {
            _ASSERT(s_Class != nullptr);
            return dynamic_cast<Class<Object>*>(s_Class);
        }

        template <typename T>
        [[nodiscard]] static bool StaticInstanceOf(const T* obj, const IClass* targetType)
        {
            if (!obj)
            {
                return false;
            }
            const Object* object = dynamic_cast<const Object*>(obj);
            if (!object)
            {
                throw std::runtime_error("Object type resides outside the managed class hierarchy!");
            }
            if (!targetType)
            {
                return false;
            }
            std::unordered_set<const IClass*> visited;
            return internalInstanceOf(object->GetClass(), targetType, visited);
        }

    private:
        [[nodiscard]] static bool internalInstanceOf(const IClass* objectType, const IClass* targetType,
                                                     std::unordered_set<const IClass*>& visited)
        {
            _ASSERT(objectType != nullptr);
            _ASSERT(targetType != nullptr);
            if (visited.find(objectType) != visited.end())
            {
                // this node has been visited
                return false;
            }
            visited.insert(objectType);
            if (objectType == targetType)
            {
                return true;
            }
            const auto& baseClassList = objectType->GetBaseClassList();
            for (const IClass* cls : baseClassList)
            {
                if (internalInstanceOf(cls, targetType, visited))
                {
                    return true;
                }
            }

            return false;
        }

        static IClass* s_Class;
    };

    static_assert(std::is_trivially_destructible<Object>::value);
}
