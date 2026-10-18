/*
The menu on the left side of the window (four rounded buttons plus the white
line that closes the menu).
*/

#pragma once
#include <raylib.h>

enum class MenuChoice
{
    None,
    Current,
    Logs,
    SaveFiles,
    Create
};

struct SidePanelResult
{
    MenuChoice chosen = MenuChoice::None;
    bool closeMenu = false;
};

// `active` is the item whose window is open; its label gets the "..." suffix.
// The Create button is only offered when showCreate is true.
SidePanelResult drawSidePanel(Rectangle area, bool showCreate, MenuChoice active);
