

#pragma once
#include <map>
#include <memory>
#include <optional>
#include "src/frontend/components/ui.h"
#include "src/frontend/screens/screen.h"
#include "src/frontend/services/file_service.h"
#include "src/frontend/services/simulation_controller.h"

class Interface
{
public:
    Interface();
    ~Interface();

    // Opens the window and runs until the program is closed.
    int run();

    // The pieces run() is made of, usable on their own:
    bool init(int width = 1280, int height = 720);
    void frame(); // read input, advance the simulation, draw once
    void shutdown();
    bool wantsExit() const { return exitRequested; }

    // ---- used by the screens ----
    void navigate(ScreenId id); // takes effect at the start of the next frame
    ScreenId currentScreen() const { return current; }
    void quit() { exitRequested = true; }
    void toggleFullscreen();
    SimulationController &simulation() { return controller; }
    EditorLauncher &editor() { return textEditor; }

    bool menuOpen = true;

    // When true the caller fills Ui::input() itself instead of raylib's
    // mouse and keyboard (used to drive the interface from code).
    void useExternalInput(bool enabled) { externalInput = enabled; }

private:
    SimulationController controller;
    EditorLauncher textEditor;
    std::map<ScreenId, std::unique_ptr<Screen>> screens;
    ScreenId current = ScreenId::Tutorial;
    std::optional<ScreenId> pending;
    bool exitRequested = false;
    bool externalInput = false;
    bool windowOpen = false;

    void applyNavigation();
    void drawChrome();
};
