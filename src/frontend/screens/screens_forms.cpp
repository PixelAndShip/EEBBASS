// "Enter Environment variables" (custom visual / multiple) and
// "Enter Environment save file name" windows.

#include "screens.h"
#include "src/frontend/components/input_form.h"
#include "src/frontend/components/ui.h"
#include "src/frontend/services/interface.h"
#include <climits>

namespace
{
    constexpr float HeaderHeight = 40;

    class VariablesScreen : public Screen
    {
    public:
        explicit VariablesScreen(bool multipleEnvironments) : multiple(multipleEnvironments)
        {
            build();
        }

        ScreenId closeTarget() const override { return ScreenId::Create; }
        bool menuHasCreate() const override { return false; }

        void draw(Interface &app, Rectangle area) override
        {
            Ui::text("Enter Environment variables:", area.x, area.y, Ui::TextSize, Ui::White);

            Rectangle formArea = {area.x, area.y + HeaderHeight, area.width, area.height - HeaderHeight};
            if (form.draw(formArea))
            {
                // process ends: the variables are complete, the environment starts
                EnvironmentSettings s = read();
                if (multiple)
                {
                    app.simulation().startMulti(s);
                }
                else
                {
                    app.simulation().startVisual(s);
                }
                build(); // fresh defaults for the next time
                app.navigate(ScreenId::Current);
            }
        }

    private:
        bool multiple;
        InputForm form;

        void build()
        {
            EnvironmentSettings d;
            form.clear();
            if (multiple)
            {
                form.addInteger("Environment count", 1, 32, d.environmentCount);
            }
            form.addDecimal("Radiation", 0.0, 1.0, "0.5");
            form.addDecimal("Cull percentage", 0.0, 1.0, "0.25");
            form.addInteger("Maximum amount of Agents", 0, INT_MAX, d.maxAgents);
            form.addInteger("Maximum amount of Plants", 0, INT_MAX, d.maxPlants);
            form.addInteger("Iteration start", 0, INT_MAX, d.iterationStart);
            form.addInteger("Max cultivation iterations", 0, INT_MAX, d.maxCultivateIterations);
            form.addInteger("Max total cycles", 0, INT_MAX, d.maxCycles);
            form.addInteger("Carbon count (not used yet)", 0, INT_MAX, d.carbonCount);
            form.addInteger("Max brain depth", 1, INT_MAX, d.maxBrainDepth);
            form.addInteger("Max brain child nodes", 1, INT_MAX, d.maxBrainChildNodes);
            form.addInteger("Brain root nodes", 1, INT_MAX, d.brainRootNodes);
            if (!multiple)
            {
                form.addInteger("Simulation steps per second", 1, 10000, (long long)d.stepsPerSecond);
            }
            form.addInteger("Visual border width", 2, INT_MAX, d.borderWidth);
            form.addInteger("Visual border height", 2, INT_MAX, d.borderHeight);
            form.addInteger("Agent size", 1, INT_MAX, d.agentSize);
            form.addInteger("Plant size", 1, INT_MAX, d.plantSize);
        }

        EnvironmentSettings read() const
        {
            EnvironmentSettings s;
            size_t i = 0;
            if (multiple)
            {
                s.environmentCount = form.integer(i++);
            }
            s.radiation = form.decimal(i++);
            s.cullPercentage = form.decimal(i++);
            s.maxAgents = form.integer(i++);
            s.maxPlants = form.integer(i++);
            s.iterationStart = form.integer(i++);
            s.maxCultivateIterations = form.integer(i++);
            s.maxCycles = form.integer(i++);
            s.carbonCount = form.integer(i++);
            s.maxBrainDepth = form.integer(i++);
            s.maxBrainChildNodes = form.integer(i++);
            s.brainRootNodes = form.integer(i++);
            if (!multiple)
            {
                s.stepsPerSecond = form.integer(i++);
            }
            s.borderWidth = form.integer(i++);
            s.borderHeight = form.integer(i++);
            s.agentSize = form.integer(i++);
            s.plantSize = form.integer(i++);
            return s;
        }
    };

    class FromSaveScreen : public Screen
    {
    public:
        FromSaveScreen() { build(); }

        ScreenId closeTarget() const override { return ScreenId::Create; }
        bool menuHasCreate() const override { return false; }

        void onEnter(Interface &app) override
        {
            (void)app;
            available.clear();
            for (const FileEntry &file : FileService::list(FileService::SaveDirectory))
            {
                available += (available.empty() ? "" : ", ") + file.name;
            }
        }

        void draw(Interface &app, Rectangle area) override
        {
            Ui::text("Enter Environment save file name:", area.x, area.y, Ui::TextSize, Ui::White);

            float listHeight = 0;
            if (!available.empty())
            {
                listHeight = Ui::paragraph("Save files in the " + std::string(FileService::SaveDirectory) + " folder: " + available,
                                           {area.x, area.y + HeaderHeight, area.width, 80}, Ui::SmallTextSize, Ui::Dim) +
                             12;
            }

            Rectangle formArea = {area.x, area.y + HeaderHeight + listHeight, area.width, area.height - HeaderHeight - listHeight};
            if (form.draw(formArea))
            {
                std::string path = FileService::resolveSaveFile(form.text(0));
                if (path.empty())
                {
                    form.setMessage("Save file not found: " + form.text(0));
                    form.focusField(0);
                }
                else if (!app.simulation().startFromSave(path, form.integer(1)))
                {
                    form.setMessage("The save file could not be loaded: " + path);
                    form.focusField(0);
                }
                else
                {
                    build();
                    app.navigate(ScreenId::Current);
                }
            }
        }

    private:
        InputForm form;
        std::string available;

        void build()
        {
            form.clear();
            form.addText("Save file name");
            form.addInteger("Simulation steps per second", 1, 10000, 10);
        }
    };
}

std::unique_ptr<Screen> makeVariablesScreen(bool multiple) { return std::make_unique<VariablesScreen>(multiple); }
std::unique_ptr<Screen> makeFromSaveScreen() { return std::make_unique<FromSaveScreen>(); }
