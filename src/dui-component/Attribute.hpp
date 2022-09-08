#pragma once

#include <stdint.h>
#include <map>
#include <vector>
#include <string>
#include <any>
#include <memory>

namespace DuiLib {
    enum AttributeValueType {
        Int32Type,
        Int64Type,
        FloatType,
        DoubleType,
        BoolType,
        StringType,
        PtrType,
    };
    struct AttributeValue {
        AttributeValueType type;
        std::any value;
    };
    typedef struct AttributeValue AttributeValue;
    class Attribute {
    private:
        std::map<const std::string&, const AttributeValue&> m_store;
    public:
        Attribute();
        ~Attribute();
        bool has(const std::string &key);
        const AttributeValue& load(const std::string &key) const;
        void store(const std::string &key, const AttributeValue& value);
        void del(const std::string &key);
        template <typename T> T asAny(const std::string &key, T defaultValue);
        template <typename T> std::shared_ptr<T> asPtr(const std::string &key);
    };
}
