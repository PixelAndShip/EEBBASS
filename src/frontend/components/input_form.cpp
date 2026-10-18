#include "input_form.h"
#include "ui.h"
#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <sstream>

namespace
{
    constexpr float RowHeight = 34;
    constexpr float RowGap = 10;
    constexpr float ColumnGap = 28;
    constexpr float MessageHeight = 56;

    std::string number(double v)
    {
        std::ostringstream out;
        out << v;
        return out.str();
    }
}

void InputForm::clear()
{
    fields.clear();
    focus = 0;
    scroll = 0;
    scrollToFocus = true;
    error.clear();
}

void InputForm::addInteger(const std::string &label, long long min, long long max, long long initial)
{
    fields.push_back({label + " (" + std::to_string(min) + " - " + (max >= 2147483647 ? std::string("max") : std::to_string(max)) + ")",
                      Kind::Integer, (double)min, (double)max, std::to_string(initial)});
}

void InputForm::addDecimal(const std::string &label, double min, double max, const std::string &initial)
{
    fields.push_back({label + " (" + number(min) + " - " + number(max) + ")",
                      Kind::Decimal, min, max, initial});
}

void InputForm::addText(const std::string &label, const std::string &initial)
{
    fields.push_back({label, Kind::Text, 0, 0, initial});
}

int InputForm::integer(size_t index) const
{
    return (int)std::strtoll(fields.at(index).value.c_str(), nullptr, 10);
}

float InputForm::decimal(size_t index) const
{
    return (float)std::strtod(fields.at(index).value.c_str(), nullptr);
}

const std::string &InputForm::text(size_t index) const
{
    return fields.at(index).value;
}

bool InputForm::validate(size_t index, std::string &problem) const
{
    const Field &f = fields[index];

    if (f.kind == Kind::Text)
    {
        if (f.value.find_first_not_of(' ') == std::string::npos)
        {
            problem = f.label + ": please enter a value";
            return false;
        }
        return true;
    }

    const char *start = f.value.c_str();
    char *end = nullptr;
    errno = 0;
    double parsed = 0;
    bool ok = !f.value.empty();

    if (ok and f.kind == Kind::Integer)
    {
        parsed = (double)std::strtoll(start, &end, 10);
    }
    else if (ok)
    {
        parsed = std::strtod(start, &end);
    }
    ok = ok and end != start and *end == '\0' and errno == 0;

    if (!ok)
    {
        problem = f.label + ": enter " + (f.kind == Kind::Integer ? "a whole number" : "a number");
        return false;
    }
    if (parsed < f.min or parsed > f.max)
    {
        problem = f.label + ": value is out of range";
        return false;
    }
    return true;
}

void InputForm::typeInto(Field &f, const std::string &typed)
{
    for (char c : typed)
    {
        bool digit = c >= '0' and c <= '9';
        switch (f.kind)
        {
        case Kind::Integer:
            if (!(digit or (c == '-' and f.value.empty())))
                continue;
            if (f.value.size() >= 11)
                continue;
            break;
        case Kind::Decimal:
            if (!(digit or (c == '.' and f.value.find('.') == std::string::npos) or (c == '-' and f.value.empty())))
                continue;
            if (f.value.size() >= 12)
                continue;
            break;
        case Kind::Text:
            if (f.value.size() >= 64)
                continue;
            break;
        }
        f.value.push_back(c);
    }
}

bool InputForm::draw(Rectangle area)
{
    if (fields.empty())
    {
        return false;
    }

    Ui::Input &in = Ui::input();
    bool completed = false;

    // ---- keyboard ----
    if (!in.typed.empty())
    {
        typeInto(fields[focus], in.typed);
        error.clear();
    }
    if (in.backspace and !fields[focus].value.empty())
    {
        fields[focus].value.pop_back();
        error.clear();
    }
    if (in.tab)
    {
        size_t n = fields.size();
        focus = in.shift ? (focus + n - 1) % n : (focus + 1) % n;
        scrollToFocus = true;
    }
    if (in.enter)
    {
        std::string problem;
        if (!validate(focus, problem))
        {
            error = problem;
        }
        else if (focus + 1 < fields.size())
        {
            focus++;
            scrollToFocus = true;
            error.clear();
        }
        else
        {
            // last field: everything has to be valid
            bool allValid = true;
            for (size_t i = 0; i < fields.size(); i++)
            {
                if (!validate(i, problem))
                {
                    focus = i;
                    scrollToFocus = true;
                    error = problem;
                    allValid = false;
                    break;
                }
            }
            if (allValid)
            {
                error.clear();
                completed = true;
            }
        }
    }

    // ---- layout ----
    int columns = area.width >= 760 ? 2 : 1;
    size_t rowsPerColumn = (fields.size() + columns - 1) / columns;
    float pitch = RowHeight + RowGap;
    Rectangle fieldArea = {area.x, area.y, area.width, std::max(40.0f, area.height - MessageHeight)};
    float columnWidth = (area.width - (columns - 1) * ColumnGap) / columns;
    float contentHeight = rowsPerColumn * pitch;
    float maxScroll = std::max(0.0f, contentHeight - fieldArea.height);

    if (Ui::hovered(fieldArea) and in.wheel != 0)
    {
        scroll -= in.wheel * pitch;
    }
    if (scrollToFocus)
    {
        float rowTop = (focus % rowsPerColumn) * pitch;
        if (rowTop < scroll)
            scroll = rowTop;
        if (rowTop + RowHeight > scroll + fieldArea.height)
            scroll = rowTop + RowHeight - fieldArea.height;
        scrollToFocus = false;
    }
    scroll = std::clamp(scroll, 0.0f, maxScroll);

    // ---- draw ----
    Ui::beginClip(fieldArea);
    for (size_t i = 0; i < fields.size(); i++)
    {
        size_t column = i / rowsPerColumn;
        size_t row = i % rowsPerColumn;
        float x = area.x + column * (columnWidth + ColumnGap);
        float y = area.y + row * pitch - scroll;

        float labelWidth = columnWidth * 0.62f;
        Rectangle input = {x + labelWidth + 8, y, columnWidth - labelWidth - 8, RowHeight};

        Ui::text(Ui::ellipsize(fields[i].label, labelWidth, Ui::SmallTextSize), x,
                 y + (RowHeight - Ui::SmallTextSize) / 2 - 1, Ui::SmallTextSize, Ui::White);

        if (Ui::clicked(input) and Ui::hovered(fieldArea))
        {
            focus = i;
            scrollToFocus = true;
        }

        bool focused = i == focus;
        DrawRectangleLinesEx(input, 2.0f, focused ? Ui::Cyan : Ui::White);

        std::string shown = fields[i].value;
        if (focused and std::fmod(GetTime(), 1.0) < 0.5)
        {
            shown += "|";
        }
        Ui::text(Ui::ellipsize(shown, input.width - 16, Ui::TextSize), input.x + 8,
                 input.y + (RowHeight - Ui::TextSize) / 2 - 1, Ui::TextSize, Ui::White);
    }
    Ui::endClip();

    // ---- message line ----
    Rectangle messageArea = {area.x, area.y + area.height - MessageHeight + 8, area.width, MessageHeight - 8};
    if (!error.empty())
    {
        Ui::paragraph(error, messageArea, Ui::SmallTextSize, Ui::Error);
    }
    else
    {
        Ui::paragraph("Enter: confirm field and continue (the last field starts the environment)   Tab: next field   Mouse wheel: scroll",
                      messageArea, Ui::SmallTextSize, Ui::Dim);
    }

    return completed;
}
