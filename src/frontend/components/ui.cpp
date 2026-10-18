#include "ui.h"
#include <cmath>
#include <cstring>

namespace Ui
{
    namespace
    {
        Input g_input;
        Font g_font = {};
        bool g_customFont = false;

        float spacingFor(float size)
        {

            return g_customFont ? 0.0f : size / 10.0f;
        }

        Color hoverColor(bool isHovered)
        {
            return isHovered ? Cyan : White;
        }
    }

    void Input::poll()
    {
        mouse = GetMousePosition();
        leftPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        leftDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        wheel = GetMouseWheelMove();

        typed.clear();
        int c = 0;
        while ((c = GetCharPressed()) > 0)
        {
            if (c >= 32 and c < 127)
            {
                typed.push_back((char)c);
            }
        }

        bool ctrl = IsKeyDown(KEY_LEFT_CONTROL) or IsKeyDown(KEY_RIGHT_CONTROL);
        if (ctrl and IsKeyPressed(KEY_V))
        {
            const char *clip = GetClipboardText();
            if (clip)
            {
                for (const char *p = clip; *p; p++)
                {
                    if (*p >= 32 and *p < 127)
                    {
                        typed.push_back(*p);
                    }
                }
            }
        }

        backspace = IsKeyPressed(KEY_BACKSPACE) or IsKeyPressedRepeat(KEY_BACKSPACE);
        enter = IsKeyPressed(KEY_ENTER) or IsKeyPressed(KEY_KP_ENTER);
        tab = IsKeyPressed(KEY_TAB);
        shift = IsKeyDown(KEY_LEFT_SHIFT) or IsKeyDown(KEY_RIGHT_SHIFT);
    }

    void Input::clearEvents()
    {
        leftPressed = false;
        wheel = 0;
        typed.clear();
        backspace = enter = tab = false;
    }

    Input &input() { return g_input; }

    void init()
    {
        const char *candidates[] = {
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "/usr/share/fonts/dejavu/DejaVuSans.ttf",
            "/usr/share/fonts/TTF/DejaVuSans.ttf",
            "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
            "/usr/share/fonts/liberation/LiberationSans-Regular.ttf",
            "C:/Windows/Fonts/segoeui.ttf",
            "C:/Windows/Fonts/arial.ttf",
            "/System/Library/Fonts/Supplemental/Arial.ttf",
            "/Library/Fonts/Arial.ttf"};

        for (const char *path : candidates)
        {
            if (FileExists(path))
            {
                Font f = LoadFontEx(path, 40, nullptr, 0);
                if (f.texture.id != 0 and f.glyphCount > 0)
                {
                    SetTextureFilter(f.texture, TEXTURE_FILTER_BILINEAR);
                    g_font = f;
                    g_customFont = true;
                    return;
                }
            }
        }
        g_font = GetFontDefault();
        g_customFont = false;
    }

    void shutdown()
    {
        if (g_customFont)
        {
            UnloadFont(g_font);
            g_customFont = false;
        }
        g_font = {};
    }

    Vector2 measure(const std::string &t, float size)
    {
        return MeasureTextEx(g_font, t.c_str(), size, spacingFor(size));
    }

    void text(const std::string &t, float x, float y, float size, Color color)
    {
        DrawTextEx(g_font, t.c_str(), {std::floor(x), std::floor(y)}, size, spacingFor(size), color);
    }

    void textCentered(const std::string &t, Rectangle area, float size, Color color)
    {
        Vector2 m = measure(t, size);
        text(t, area.x + (area.width - m.x) / 2, area.y + (area.height - m.y) / 2, size, color);
    }

    std::vector<std::string> wrap(const std::string &t, float maxWidth, float size)
    {
        std::vector<std::string> lines;
        size_t pos = 0;
        while (pos <= t.size())
        {
            size_t nl = t.find('\n', pos);
            std::string paragraph = t.substr(pos, nl == std::string::npos ? std::string::npos : nl - pos);

            std::string current;
            size_t p = 0;
            while (p < paragraph.size())
            {
                size_t sp = paragraph.find(' ', p);
                std::string word = paragraph.substr(p, sp == std::string::npos ? std::string::npos : sp - p);
                std::string candidate = current.empty() ? word : current + " " + word;
                if (!current.empty() and measure(candidate, size).x > maxWidth)
                {
                    lines.push_back(current);
                    current = word;
                }
                else
                {
                    current = candidate;
                }
                p = (sp == std::string::npos) ? paragraph.size() : sp + 1;
            }
            lines.push_back(current);

            if (nl == std::string::npos)
            {
                break;
            }
            pos = nl + 1;
        }
        return lines;
    }

    float paragraph(const std::string &t, Rectangle area, float size, Color color)
    {
        float lineHeight = size * 1.35f;
        float y = area.y;
        for (const std::string &line : wrap(t, area.width, size))
        {
            if (y + lineHeight > area.y + area.height + 0.5f)
            {
                break;
            }
            text(line, area.x, y, size, color);
            y += lineHeight;
        }
        return y - area.y;
    }

    std::string ellipsize(const std::string &t, float maxWidth, float size)
    {
        if (measure(t, size).x <= maxWidth)
        {
            return t;
        }
        std::string cut = t;
        while (!cut.empty() and measure(cut + "...", size).x > maxWidth)
        {
            cut.pop_back();
        }
        return cut + "...";
    }

    // -------------------------------------------------------------- widgets

    bool hovered(Rectangle area)
    {
        return CheckCollisionPointRec(g_input.mouse, area);
    }

    bool clicked(Rectangle area)
    {
        return g_input.leftPressed and hovered(area);
    }

    void frame(Rectangle area, float thickness)
    {
        DrawRectangleLinesEx(area, thickness, Cyan);
    }

    bool button(Rectangle area, const std::string &label, bool enabled)
    {
        bool isHovered = enabled and hovered(area);
        if (isHovered)
        {
            DrawRectangleRounded(area, 0.35f, 10, Hover);
        }
        Color color = enabled ? White : Dim;
        DrawRectangleRoundedLinesEx(area, 0.35f, 10, 2.0f, isHovered ? Cyan : color);

        float size = TextSize;
        float inner = area.width - 24;
        std::vector<std::string> lines = wrap(label, inner, size);
        if (lines.size() > 2)
        {
            size = SmallTextSize;
            lines = wrap(label, inner, size);
        }
        float lineHeight = size * 1.25f;
        float y = area.y + (area.height - lineHeight * lines.size()) / 2;
        for (const std::string &line : lines)
        {
            Vector2 m = measure(line, size);
            text(line, area.x + (area.width - m.x) / 2, y, size, color);
            y += lineHeight;
        }
        return enabled and clicked(area);
    }

    void descriptionBox(Rectangle area, const std::string &t)
    {
        DrawRectangleLinesEx(area, 2.0f, White);
        Rectangle inner = {area.x + 12, area.y + 8, area.width - 24, area.height - 16};
        std::vector<std::string> lines = wrap(t, inner.width, SmallTextSize);
        float lineHeight = SmallTextSize * 1.3f;
        float y = area.y + (area.height - lineHeight * lines.size()) / 2;
        for (const std::string &line : lines)
        {
            Vector2 m = measure(line, SmallTextSize);
            text(line, area.x + (area.width - m.x) / 2, y, SmallTextSize, White);
            y += lineHeight;
        }
    }

    bool closeWindowButton(Rectangle area)
    {
        bool h = hovered(area);
        Rectangle bar = {area.x + (area.width - 28) / 2, area.y + (area.height - 5) / 2, 28, 5};
        DrawRectangleRec(bar, hoverColor(h));
        return clicked(area);
    }

    bool menuButton(Rectangle area)
    {
        bool h = hovered(area);
        float x = area.x + (area.width - 26) / 2;
        float y = area.y + (area.height - 19) / 2;
        for (int i = 0; i < 3; i++)
        {
            DrawRectangleRec({x, y + i * 7.0f, 26, 5}, hoverColor(h));
        }
        return clicked(area);
    }

    bool killButton(Rectangle area)
    {
        bool h = hovered(area);
        Color c = hoverColor(h);
        float cx = area.x + area.width / 2;
        float cy = area.y + area.height / 2;
        float r = 8;
        DrawLineEx({cx - r, cy - r}, {cx + r, cy + r}, 3.0f, c);
        DrawLineEx({cx - r, cy + r}, {cx + r, cy - r}, 3.0f, c);
        return clicked(area);
    }

    bool fullscreenButton(Rectangle area)
    {
        bool h = hovered(area);
        Rectangle box = {area.x + (area.width - 26) / 2, area.y + (area.height - 17) / 2, 26, 17};
        DrawRectangleLinesEx(box, 3.0f, hoverColor(h));
        return clicked(area);
    }

    void beginClip(Rectangle area)
    {
        BeginScissorMode((int)area.x, (int)area.y, (int)area.width, (int)area.height);
    }

    void endClip()
    {
        EndScissorMode();
    }
}
