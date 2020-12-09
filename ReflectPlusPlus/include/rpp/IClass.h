#pragma once

#include <string_view>
#include <vector>

#include "ClassManager.h"

namespace rpp
{
    class IClass
    {
    public:
        IClass(const std::string& name) : m_Name(name), m_Size(std::numeric_limits<size_t>::max())
        {
            ClassManager::AddClass(name, this);
        }

        [[nodiscard]] virtual std::string GetName() const = 0;

        [[nodiscard]] const std::vector<const IClass*>& GetBaseClassList() const
        {
            if (!m_BaseListCache)
            {
                // query the class manager for class pointers
                m_BaseListCache = new std::vector<const IClass*>();
                m_BaseListCache->reserve(m_BaseNames.size());
                for (const std::string& name : m_BaseNames)
                {
                    m_BaseListCache->push_back(ClassManager::GetClass(name));
                }
            }
            return *m_BaseListCache;
        }

        void AddBaseClass(const std::string& name)
        {
            m_BaseNames.push_back(name);
            delete m_BaseListCache;
        }

        void SetSize(size_t size)
        {
            m_Size = size;
        }

        size_t GetSize() const
        {
            return m_Size;
        }

    protected:
        std::string m_Name;
        std::vector<std::string> m_BaseNames;
        mutable std::vector<const IClass*>* m_BaseListCache = nullptr;
        size_t m_Size;
    };
}
