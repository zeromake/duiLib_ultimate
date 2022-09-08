#pragma once

#include <string>
#include "Attribute.hpp"
#include <memory>

namespace DuiLib {
    class VControl
    {
    public:
        VControl(/* args */);
        ~VControl();
    public:
        std::string m_type;
        std::unique_ptr<Attribute> m_props;
        std::string m_key;
    };
    VControl::VControl(/* args */) {}
    VControl::~VControl() {}
}
