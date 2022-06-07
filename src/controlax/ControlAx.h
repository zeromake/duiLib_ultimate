#include "UIFlash.h"
#include "UIActiveX.h"

using namespace DuiLib;

UILIB_API void RegisterControlAx() {
    auto controlFactory = CControlFactory::GetInstance();
    // ActiveX
	controlFactory->INNER_REGISTER_DUICONTROL(CActiveXUI);
    // Flash
    controlFactory->INNER_REGISTER_DUICONTROL(CFlashUI);
}
