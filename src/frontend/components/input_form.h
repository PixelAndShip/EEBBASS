#pragma once
#include <raylib.h>
#include <string>
#include <vector>

class InputForm
{
public:
    enum class Kind
    {
        Integer,
        Decimal,
        Text
    };

    struct Field
    {
        std::string label;
        Kind kind = Kind::Text;
        double min = 0;
        double max = 0;
        std::string value;
    };

    void clear();
    void addInteger(const std::string &label, long long min, long long max, long long initial);
    void addDecimal(const std::string &label, double min, double max, const std::string &initial);
    void addText(const std::string &label, const std::string &initial = "");

    // Draws the form and handles typing. Returns true on the frame the form
    // is completed with every field valid.
    bool draw(Rectangle area);

    int integer(size_t index) const;
    float decimal(size_t index) const;
    const std::string &text(size_t index) const;
    size_t size() const { return fields.size(); }

    // shown under the fields (validation problem or hint)
    void setMessage(const std::string &message) { error = message; }

    // move the keyboard focus to a field (e.g. back to the one that was wrong)
    void focusField(size_t index)
    {
        if (index < fields.size())
        {
            focus = index;
            scrollToFocus = true;
        }
    }

private:
    std::vector<Field> fields;
    size_t focus = 0;
    float scroll = 0;
    bool scrollToFocus = true;
    std::string error;

    bool validate(size_t index, std::string &problem) const;
    void typeInto(Field &field, const std::string &typed);
};
