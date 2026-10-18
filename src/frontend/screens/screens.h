#pragma once
#include <memory>
#include "screen.h"

// one factory per window of the design, implemented in the screens_*.cpp files
std::unique_ptr<Screen> makeTutorialScreen();
std::unique_ptr<Screen> makeFileListScreen(bool logs); // logs = false: save files
std::unique_ptr<Screen> makeCreateScreen();
std::unique_ptr<Screen> makeVariablesScreen(bool multiple);
std::unique_ptr<Screen> makeFromSaveScreen();
std::unique_ptr<Screen> makeCurrentScreen();
