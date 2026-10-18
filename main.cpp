/*
Last time edited: 18.10.2026
Last edit summary: console menu replaced by the graphical frontend
Existing issues:
*
Potential additions:
*
*/

#include <filesystem>

#include "src/frontend/services/interface.h"

// Environmentally engineered behavior based agent simulation system
int main()
{
    std::filesystem::path currentDir =
        std::filesystem::current_path();

    std::filesystem::create_directories(
        currentDir / "EEBBASS" / "logs");

    std::filesystem::create_directories(
        currentDir / "EEBBASS" / "environments");

    std::filesystem::current_path(
        currentDir / "EEBBASS");

    Interface ui;
    return ui.run();
}
