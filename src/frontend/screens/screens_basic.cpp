// Tutorial, the two file lists (save files / logs) and the Create window.

#include "screens.h"
#include "src/frontend/components/ui.h"
#include "src/frontend/services/interface.h"
#include <algorithm>
#include <cstdio>

namespace
{

    class TutorialScreen : public Screen
    {
    public:
        bool hasCloseButton() const override { return false; }

        void draw(Interface &app, Rectangle area) override
        {
            (void)app;
            const char *tutorial =
                "Welcome to EEBBASS - Environmentally Engineered Behavior Based Agent Simulation System.\n"
                "\n"
                "Agents with neural network like brains live inside an environment. They look for plants to eat, "
                "compete with each other, and the ones that do best pass their behavior on to the next "
                "generation with a little random change. Over many cycles useful behavior is engineered "
                "by natural selection.\n"
                "\n"
                "Menu\n"
                "- Current Environment: watch the environment that is running right now, or the progress of several environments running together. Click it again while watching to hide or show the description on top.\n"
                "- Environment Logs: the log files written by environments.\n"
                "- Environment Save files: saved environments. Click a file to open it in a text editor.\n"
                "- Create Environment: start a default environment, a custom one, several at once or one from a save file.\n"
                "\n"
                "Window buttons\n"
                "- Three lines / one white line: open the menu / close the menu or window. Closing a window never stops a running environment.\n"
                "- White X inside a window: stops the process of that window. The environment writes its save file when it stops.\n"
                "- White rectangle in the top bar: full screen on and off. The X next to it closes the program.";

            Ui::paragraph(tutorial, area, Ui::TextSize, Ui::White);
        }
    };

    class FileListScreen : public Screen
    {
    public:
        explicit FileListScreen(bool showLogs) : logs(showLogs) {}

        MenuChoice activeMenuItem() const override
        {
            return logs ? MenuChoice::Logs : MenuChoice::SaveFiles;
        }

        void onEnter(Interface &app) override
        {
            (void)app;
            refresh();
            scroll = 0;
        }

        void draw(Interface &app, Rectangle area) override
        {
            // logs and saves change while environments run
            if (GetTime() - lastRefresh > 2.0)
            {
                refresh();
            }

            EditorLauncher &editor = app.editor();

            if (editor.running())
            {
                // "Open file with text editor": the process runs, the list waits for it
                Rectangle box = {area.x + area.width / 2 - 260, area.y + 60, 520, 110};
                Ui::descriptionBox(box, "Opened " + editor.file() + " in the text editor.\nClose the editor to come back to this list.");
                return;
            }

            if (!editor.error().empty())
            {
                Ui::paragraph(editor.error() + ". Set the EEBBASS_EDITOR environment variable to the editor you want to use.",
                              {area.x, area.y + area.height - 50, area.width, 50}, Ui::SmallTextSize, Ui::Error);
            }

            if (files.empty())
            {
                Ui::paragraph(std::string("No files found in the ") + directory() + " folder yet.",
                              area, Ui::TextSize, Ui::Dim);
                return;
            }

            float pitch = 84;
            float buttonWidth = std::min(320.0f, area.width * 0.45f);
            float boxWidth = std::min(320.0f, area.width - buttonWidth - 30);
            float visibleHeight = area.height - (editor.error().empty() ? 0 : 50);
            float maxScroll = std::max(0.0f, files.size() * pitch - visibleHeight);

            Rectangle listArea = {area.x, area.y, area.width, visibleHeight};
            if (Ui::hovered(listArea) and Ui::input().wheel != 0)
            {
                scroll -= Ui::input().wheel * pitch;
            }
            scroll = std::clamp(scroll, 0.0f, maxScroll);

            Ui::beginClip(listArea);
            for (size_t i = 0; i < files.size(); i++)
            {
                float y = area.y + i * pitch - scroll;
                if (y + 56 < area.y or y > area.y + visibleHeight)
                {
                    continue;
                }

                if (Ui::button({area.x, y, buttonWidth, 56}, files[i].name))
                {
                    editor.open(files[i].path);
                }
                Ui::descriptionBox({area.x + buttonWidth + 30, y, boxWidth, 56}, FileService::describe(files[i]));
            }
            Ui::endClip();
        }

    private:
        bool logs;
        std::vector<FileEntry> files;
        float scroll = 0;
        double lastRefresh = 0;

        const char *directory() const
        {
            return logs ? FileService::LogDirectory : FileService::SaveDirectory;
        }

        void refresh()
        {
            files = FileService::list(directory());
            lastRefresh = GetTime();
        }
    };

    class CreateScreen : public Screen
    {
    public:
        MenuChoice activeMenuItem() const override { return MenuChoice::Create; }

        void draw(Interface &app, Rectangle area) override
        {
            EnvironmentSettings defaults;
            char defaultText[256];
            std::snprintf(defaultText, sizeof defaultText,
                          "Starts one visual environment right away with default values (%d agents, %d plants, radiation %.1f).",
                          defaults.maxAgents, defaults.maxPlants, defaults.radiation);

            struct Row
            {
                const char *label;
                std::string description;
            };
            const Row rows[] = {
                {"Create Default Visual Environment", defaultText},
                {"Create Custom Visual Environment", "Choose every variable yourself, then watch the environment live."},
                {"Create multiple environments", "Choose the variables once and simulate several environments in parallel, without visuals. Each one writes a save file."},
                {"Create Visual Environment from Save file", "Continue an environment from one of its save files in a visual simulation."}};

            float pitch = std::min(104.0f, area.height / 4.0f);
            float buttonWidth = std::min(340.0f, area.width * 0.45f);
            float boxWidth = std::min(460.0f, area.width - buttonWidth - 30);

            for (int i = 0; i < 4; i++)
            {
                float y = area.y + i * pitch;
                float h = std::min(72.0f, pitch - 14);
                if (Ui::button({area.x, y, buttonWidth, h}, rows[i].label))
                {
                    switch (i)
                    {
                    case 0:
                        app.simulation().startDefault();
                        app.navigate(ScreenId::Current);
                        break;
                    case 1:
                        app.navigate(ScreenId::CustomVariables);
                        break;
                    case 2:
                        app.navigate(ScreenId::MultiVariables);
                        break;
                    case 3:
                        app.navigate(ScreenId::FromSave);
                        break;
                    }
                }
                Ui::descriptionBox({area.x + buttonWidth + 30, y, boxWidth, h}, rows[i].description);
            }
        }
    };
}

std::unique_ptr<Screen> makeTutorialScreen() { return std::make_unique<TutorialScreen>(); }
std::unique_ptr<Screen> makeFileListScreen(bool logs) { return std::make_unique<FileListScreen>(logs); }
std::unique_ptr<Screen> makeCreateScreen() { return std::make_unique<CreateScreen>(); }
