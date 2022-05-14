#include "UIFlash.h"
#include "UIActiveX.h"

UILIB_API void RegisterControlAx() {
    auto controlFactory = DuiLib::CControlFactory::GetInstance();
    // ActiveX
	controlFactory->INNER_REGISTER_DUICONTROL(DuiLib::CActiveXUI);
    // Flash
    controlFactory->INNER_REGISTER_DUICONTROL(DuiLib::CFlashUI);
    
}
