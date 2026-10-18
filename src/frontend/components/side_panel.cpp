#include "side_panel.h"
#include "ui.h"
#include <algorithm>
#include <string>

SidePanelResult drawSidePanel(Rectangle area, bool showCreate, MenuChoice active)
{
    SidePanelResult result;

    // white line at the top right closes the menu
    if (Ui::closeWindowButton({area.x + area.width - 60, area.y + 6, 48, 36}))
    {
        result.closeMenu = true;
    }

    struct Item
    {
        MenuChoice choice;
        const char *label;
    };
    const Item items[] = {
        {MenuChoice::Current, "Current Environment"},
        {MenuChoice::Logs, "Environment Logs"},
        {MenuChoice::SaveFiles, "Environment Save files"},
        {MenuChoice::Create, "Create Environment"}};

    int count = showCreate ? 4 : 3;
    float top = area.y + 60;
    float pitch = std::min(82.0f, (area.height - 70) / 4.0f);
    float buttonHeight = std::min(52.0f, pitch - 8);
    float buttonWidth = area.width - 36;

    for (int i = 0; i < count; i++)
    {
        std::string label = items[i].label;
        if (items[i].choice == active)
        {
            label += "...";
        }
        Rectangle box = {area.x + 18, top + i * pitch, buttonWidth, buttonHeight};
        if (Ui::button(box, label))
        {
            result.chosen = items[i].choice;
        }
    }
    return result;
}
