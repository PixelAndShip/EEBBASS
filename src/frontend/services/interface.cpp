#include "interface.h"
#include "src/frontend/components/side_panel.h"
#include <algorithm>

namespace
{
    constexpr float TitleBarHeight = 40;
}

Interface::Interface() = default;

Interface::~Interface()
{
    shutdown();
}

bool Interface::init(int width, int height)
{
    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(width, height, "EEBBASS");
    if (!IsWindowReady())
    {
        return false;
    }
    windowOpen = true;
    SetWindowMinSize(900, 540);
    SetExitKey(KEY_NULL); // Escape must not close the program
    SetTargetFPS(60);
    Ui::init();

    screens.clear();
    for (ScreenId id : {ScreenId::Tutorial, ScreenId::SaveFiles, ScreenId::Logs, ScreenId::Create,
                        ScreenId::CustomVariables, ScreenId::MultiVariables, ScreenId::FromSave, ScreenId::Current})
    {
        screens[id] = makeScreen(id);
    }

    current = ScreenId::Tutorial;
    screens[current]->onEnter(*this);
    return true;
}

void Interface::shutdown()
{
    controller.kill(); // environments write their save files
    if (windowOpen)
    {
        screens.clear();
        Ui::shutdown();
        CloseWindow();
        windowOpen = false;
    }
}

int Interface::run()
{
    if (!init())
    {
        return 1;
    }
    while (!exitRequested and !WindowShouldClose())
    {
        frame();
    }
    shutdown();
    return 0;
}

void Interface::navigate(ScreenId id)
{
    pending = id;
}

void Interface::applyNavigation()
{
    if (pending)
    {
        current = *pending;
        pending.reset();
        screens[current]->onEnter(*this);
    }
}

void Interface::toggleFullscreen()
{
    ToggleBorderlessWindowed();
}

void Interface::frame()
{
    applyNavigation();

    if (!externalInput)
    {
        Ui::input().poll();
    }

    controller.update(GetFrameTime());
    if (controller.multiFinished())
    {

        controller.kill();
        navigate(ScreenId::Create);
    }

    BeginDrawing();
    ClearBackground(Ui::Black);
    drawChrome();
    EndDrawing();

    applyNavigation();
}

void Interface::drawChrome()
{
    float width = (float)GetScreenWidth();
    float height = (float)GetScreenHeight();
    Screen &screen = *screens[current];

    // ---- title bar: full screen toggle and close program ----
    DrawLineEx({0, TitleBarHeight}, {width, TitleBarHeight}, 2.0f, Ui::Cyan);
    if (Ui::fullscreenButton({width - 104, 4, 48, 32}))
    {
        toggleFullscreen();
    }
    if (Ui::killButton({width - 52, 4, 44, 32}))
    {
        quit();
    }

    // ---- menu on the left ----
    float contentX = 0;
    if (menuOpen)
    {
        float menuWidth = std::clamp(width * 0.25f, 250.0f, 340.0f);
        Rectangle menuArea = {0, TitleBarHeight, menuWidth, height - TitleBarHeight};

        bool showCreate = screen.menuHasCreate() and !controller.active();
        SidePanelResult menu = drawSidePanel(menuArea, showCreate, screen.activeMenuItem());

        DrawLineEx({menuWidth, TitleBarHeight}, {menuWidth, height}, 2.0f, Ui::Cyan);
        contentX = menuWidth;

        if (menu.closeMenu)
        {
            menuOpen = false;
        }
        if (menu.chosen != MenuChoice::None and screen.onMenuChoice(*this, menu.chosen))
        {
            menu.chosen = MenuChoice::None; // the screen dealt with it
        }

        switch (menu.chosen)
        {
        case MenuChoice::Current:
            navigate(ScreenId::Current);
            break;
        case MenuChoice::Logs:
            navigate(ScreenId::Logs);
            break;
        case MenuChoice::SaveFiles:
            navigate(ScreenId::SaveFiles);
            break;
        case MenuChoice::Create:
            navigate(ScreenId::Create);
            break;
        case MenuChoice::None:
            break;
        }
    }
    else if (Ui::menuButton({8, TitleBarHeight + 6, 44, 36}))
    {
        menuOpen = true;
    }

    // ---- active window ----
    Rectangle content = {contentX, TitleBarHeight, width - contentX, height - TitleBarHeight};

    if (screen.hasCloseButton())
    {
        if (Ui::closeWindowButton({width - 60, TitleBarHeight + 6, 48, 36}))
        {
            navigate(screen.closeTarget());
        }
    }

    if (screen.padded())
    {
        float left = menuOpen ? 20.0f : 64.0f; // keep clear of the open-menu icon
        screen.draw(*this, {content.x + left, content.y + 56, content.width - left - 24, content.height - 56 - 20});
    }
    else
    {
        screen.draw(*this, content);
    }

    // ---- frame around the whole window ----
    DrawRectangleLinesEx({0, 0, width, height}, 3.0f, Ui::Cyan);
}
