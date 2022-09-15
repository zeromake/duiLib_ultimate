#include "Attribute.hpp"

namespace DuiLib {
    Attribute::Attribute(){}
    Attribute::~Attribute(){}

    bool Attribute::has(const std::string &key) {
        return m_store.find(key) != m_store.end();
    }

    const AttributeValue& Attribute::get(const std::string &key) const {
        return m_store.find(key)->second;
    }

    void Attribute::del(const std::string &key) {
        auto iter = m_store.find(key);
        if (iter != m_store.end()) {
            m_store.erase(iter);
        }
    }

    void Attribute::add(const std::string &key, const AttributeValue& value) {
        m_store.insert({key, value});
    }

    template <typename T> T Attribute::asAny(const std::string &key, T defaultValue) {
        auto iter = m_store.find(key);
        if (iter != m_store.end()) {
            return std::any_cast<T>(iter->second.value);
        }
        return defaultValue;
    }

    template <typename T> std::shared_ptr<T> Attribute::asPtr(const std::string &key) {
        auto iter = m_store.find(key);
        if (iter != m_store.end() && iter->second.type == AttributeValueType::PtrType) {
            return std::any_cast<std::shared_ptr<T>>(iter->second.value);
        }
        return std::shared_ptr(nullptr);
    }
}
