

#pragma once
#include <raylib.h>
#include <string>
#include <vector>

namespace Ui
{

    inline const Color Cyan = {0, 255, 255, 255};
    inline const Color White = {255, 255, 255, 255};
    inline const Color Black = {0, 0, 0, 255};
    inline const Color Dim = {160, 160, 160, 255};
    inline const Color Hover = {38, 38, 38, 255};
    inline const Color Error = {255, 96, 96, 255};

    inline constexpr float TextSize = 20.0f;
    inline constexpr float SmallTextSize = 16.0f;

    struct Input
    {
        Vector2 mouse = {0, 0};
        bool leftPressed = false;
        bool leftDown = false;
        float wheel = 0;
        std::string typed;
        bool backspace = false;
        bool enter = false;
        bool tab = false;
        bool shift = false;

        void poll();
        void clearEvents();
    };

    void init();
    void shutdown();
    Input &input();

    Vector2 measure(const std::string &text, float size = TextSize);
    void text(const std::string &text, float x, float y, float size = TextSize, Color color = White);
    void textCentered(const std::string &text, Rectangle area, float size = TextSize, Color color = White);
    std::vector<std::string> wrap(const std::string &text, float maxWidth, float size = TextSize);

    float paragraph(const std::string &text, Rectangle area, float size = TextSize, Color color = White);
    std::string ellipsize(const std::string &text, float maxWidth, float size = TextSize);

    bool hovered(Rectangle area);
    bool clicked(Rectangle area);

    void frame(Rectangle area, float thickness = 2.0f);
    bool button(Rectangle area, const std::string &label, bool enabled = true);
    void descriptionBox(Rectangle area, const std::string &text);

    bool closeWindowButton(Rectangle area);
    bool menuButton(Rectangle area);
    bool killButton(Rectangle area);
    bool fullscreenButton(Rectangle area);

    void beginClip(Rectangle area);
    void endClip();
}
