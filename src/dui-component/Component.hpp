#pragma once

#include <vector>
#include <memory>
#include <DuiLib.h>

namespace DuiLib {
    class VControl;
    
    class Component {
    private:
        CPaintManagerUI* m_manager;
    public:
        Component(CPaintManagerUI* pManager);
        virtual ~Component();
        virtual std::vector<VControl> render() {
            return;
        };
        static std::shared_ptr<Component> CreateComponent(CPaintManagerUI* pManager) {
            return std::make_shared<Component>(new Component(pManager));
        };
    };
    Component::Component(CPaintManagerUI* pManager): m_manager(pManager) {}
    Component::~Component(){}
}
