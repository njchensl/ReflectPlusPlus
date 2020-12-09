#pragma once
#include <unordered_map>
#include <string>
#include <unordered_set>

#include "ICPointer.h"

namespace rpp
{
    class IClass;

    class ClassManager
    {
        inline static std::unordered_map<std::string, const IClass*>* s_NameToClass = nullptr;

        static void ensureInitialized();

        static void internalCheckClassHierarchy(const IClass* clsToCheck, std::unordered_set<const IClass*>& visited);

    public:
        static void AddClass(const std::string& name, const IClass* cls);

        static const IClass* GetClass(const std::string& name);

        static void CheckClassHierarchy();

        template <typename T, typename ...Args>
        static CPointer<T> New(Args&&...args)
        {
            T* ptr = nullptr;
            while (!ptr)
            {
                ptr = static_cast<T*>(malloc(sizeof(T)));
            }
            new(ptr)T(std::forward<Args>(args)...);
            return ptr;
        }

    };
}
