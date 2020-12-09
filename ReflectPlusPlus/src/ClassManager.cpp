#include "ClassManager.h"


#include <stdexcept>
#include <unordered_set>

#include "Class.h"

namespace rpp
{
    void ClassManager::ensureInitialized()
    {
        while (!s_NameToClass)
        {
            s_NameToClass = new std::unordered_map<std::string, const IClass*>();
        }
    }

    void ClassManager::internalCheckClassHierarchy(const IClass* clsToCheck, std::unordered_set<const IClass*>& visited)
    {
        if (visited.find(clsToCheck) != visited.end())
        {
            throw std::runtime_error("Multiple inheritance detected!");
        }
        visited.insert(clsToCheck);
        for (const IClass* cls : clsToCheck->GetBaseClassList())
        {
            internalCheckClassHierarchy(cls, visited);
        }
    }

    void ClassManager::AddClass(const std::string& name, const IClass* cls)
    {
        ensureInitialized();
        _ASSERT(cls != nullptr);
        s_NameToClass->operator[](name) = cls;
    }

    const IClass* ClassManager::GetClass(const std::string& name)
    {
        const IClass* cls = s_NameToClass->operator[](name);
        _ASSERT(cls != nullptr);
        return cls;
    }

    void ClassManager::CheckClassHierarchy()
    {
        for (auto& [_, cls] : *s_NameToClass)
        {
            std::unordered_set<const IClass*> visited;
            internalCheckClassHierarchy(cls, visited);
        }
    }
}
