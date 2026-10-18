// "Current Environment": live description on top, the simulation below.

#include "screens.h"
#include "src/frontend/components/environment_window.h"
#include "src/frontend/components/ui.h"
#include "src/frontend/services/interface.h"
#include <algorithm>
#include <cstdio>

namespace
{
    constexpr float DescriptionHeight = 128;

    const char *stateName(EnvironmentState state)
    {
        switch (state)
        {
        case EnvironmentState::Running:
            return "running";
        case EnvironmentState::Paused:
            return "cultivating next generation";
        case EnvironmentState::Finished:
            return "finished";
        case EnvironmentState::Observed:
            return "observed";
        }
        return "";
    }

    class CurrentScreen : public Screen
    {
    public:
        bool hasCloseButton() const override { return false; } // draws its own two buttons
        bool padded() const override { return false; }
        MenuChoice activeMenuItem() const override { return MenuChoice::Current; }

        void draw(Interface &app, Rectangle area) override
        {
            SimulationController &sim = app.simulation();

            if (!sim.active())
            {
                Rectangle message = {area.x + (app.menuOpen ? 20.0f : 64.0f), area.y + 56, area.width - 90, 80};
                Ui::paragraph("No environment is active.\nOpen Create Environment in the menu to start one.", message, Ui::TextSize, Ui::Dim);
                return;
            }

            // ---- description panel (updated live), closable with the white line ----
            float top = area.y;
            float textLeft = area.x + (app.menuOpen ? 16.0f : 64.0f);

            if (descriptionOpen)
            {
                if (Ui::closeWindowButton({area.x + area.width - 60, area.y + 6, 48, 36}))
                {
                    descriptionOpen = false;
                }

                std::string lines = describe(sim);
                Ui::paragraph(lines, {textLeft, area.y + 10, area.width - (textLeft - area.x) - 70, DescriptionHeight - 14},
                              Ui::TextSize, Ui::White);

                top = area.y + DescriptionHeight;
                DrawLineEx({area.x, top}, {area.x + area.width, top}, 2.0f, Ui::Cyan);
            }
            else if (Ui::menuButton({area.x + area.width - 108, area.y + 6, 48, 36}))
            {
                descriptionOpen = true;
            }

            // ---- X: kill the process ----
            if (Ui::killButton({area.x + area.width - 56, top + 6, 44, 36}))
            {
                sim.kill();
                app.navigate(ScreenId::Create);
                return;
            }

            // ---- the simulation itself ----
            Rectangle view = {area.x + 12, top + 50, area.width - 24, area.y + area.height - (top + 50) - 12};
            if (view.width < 20 or view.height < 20)
            {
                return;
            }

            if (sim.mode() == SimulationController::Mode::Visual)
            {
                drawEnvironmentWorld(*sim.environment(), view);
            }
            else if (sim.mode() == SimulationController::Mode::Multi)
            {
                drawMultiProgress(*sim.manager(), view, GetTime());
            }
        }

    private:
        bool descriptionOpen = true;

        static std::string describe(SimulationController &sim)
        {
            char text[512];

            if (sim.mode() == SimulationController::Mode::Visual)
            {
                Environment &env = *sim.environment();
                std::snprintf(text, sizeof text,
                              "Environment %d - %s\n"
                              "Iteration %d / %d    Cycle %d    Speed %.0f steps per second\n"
                              "Agents %d    Plants %d\n"
                              "World %d x %d    Radiation %.2f    Cull %.2f",
                              env.getIdentifier(), stateName(env.getState()),
                              env.getIteration(), env.getMaxCultivateIteration(), env.getCompletedCycles(), sim.stepsPerSecond(),
                              env.getAgentCount(), env.getPlantCount(),
                              env.getWorldWidth(), env.getWorldHeight(), env.getRadiation(), env.getCullPercentage());
            }
            else
            {
                SimManager &mgr = *sim.manager();
                int total = mgr.getEnvironmentCount();
                int done = mgr.getFinishedCount();
                int cycles = 0;
                int maxCycles = 0;
                for (Environment *env : mgr.getEnvironments())
                {
                    cycles += env->getCompletedCycles();
                    maxCycles = env->getMaxCycle();
                }
                std::snprintf(text, sizeof text,
                              "%d environments simulating in parallel\n"
                              "Finished %d / %d\n"
                              "Cycles completed %d / %d\n"
                              "Every environment writes its save file when it ends.",
                              total, done, total, cycles, total * (maxCycles + 1));
            }
            return text;
        }
    };
}

std::unique_ptr<Screen> makeCurrentScreen() { return std::make_unique<CurrentScreen>(); }
