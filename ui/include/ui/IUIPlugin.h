#pragma once
#ifdef CAE_ENABLE_GUI

#include "core/ActionManager.h"
class MainWindow;

class IUIPlugin {
public:
    virtual ~IUIPlugin() = default;
    virtual void setupUI(MainWindow* mainWindow) = 0;
    virtual void teardownUI() = 0;
};

#endif // CAE_ENABLE_GUI
