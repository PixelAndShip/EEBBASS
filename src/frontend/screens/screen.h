/*
One Screen per window of the design. The Interface draws the chrome around
them (title bar, menu, window buttons) and asks the active Screen to fill the
content area.
*/

#pragma once
#include <memory>
#include <raylib.h>
#include "src/frontend/components/side_panel.h"

class Interface;

enum class ScreenId
{
    Tutorial,
    SaveFiles,
    Logs,
    Create,
    CustomVariables,
    MultiVariables,
    FromSave,
    Current
};

class Screen
{
public:
    virtual ~Screen() = default;

    virtual void onEnter(Interface &app) { (void)app; }
    virtual void draw(Interface &app, Rectangle area) = 0;

    // the white line in the top right corner of the window
    virtual bool hasCloseButton() const { return true; }
    virtual ScreenId closeTarget() const { return ScreenId::Tutorial; }

    // false: the screen draws edge to edge and handles its own header
    virtual bool padded() const { return true; }

    // whether the menu may offer "Create Environment" next to the other three
    virtual bool menuHasCreate() const { return true; }
    virtual MenuChoice activeMenuItem() const { return MenuChoice::None; }
};

std::unique_ptr<Screen> makeScreen(ScreenId id);
