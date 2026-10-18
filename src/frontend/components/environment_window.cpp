#include "environment_window.h"
#include "ui.h"
#include <algorithm>
#include <cmath>

void drawEnvironmentWorld(Environment &environment, Rectangle area)
{
    float worldW = (float)std::max(1, environment.getWorldWidth());
    float worldH = (float)std::max(1, environment.getWorldHeight());

    float scale = std::min(area.width / worldW, area.height / worldH);
    float drawW = worldW * scale;
    float drawH = worldH * scale;
    float originX = area.x + (area.width - drawW) / 2;
    float originY = area.y + (area.height - drawH) / 2;

    DrawRectangleLinesEx({originX - 1, originY - 1, drawW + 2, drawH + 2}, 1.0f, {0, 110, 110, 255});

    Spider *spider = environment.spider;
    if (spider)
    {
        float plantRadius = std::max(1.0f, plantSize * scale);
        float agentRadius = std::max(1.0f, agentSize * scale);

        for (auto &[coords, plant] : spider->Plants)
        {
            if (!plant)
            {
                continue;
            }
            UnitColor c = plant->getPlantColor();
            DrawCircleV({originX + plant->getX() * scale, originY + plant->getY() * scale},
                        plantRadius,
                        {(unsigned char)c.red,
                         (unsigned char)c.green,
                         (unsigned char)c.blue,
                         (unsigned char)c.transparency});
        }

        for (auto &[coords, agent] : spider->Agents)
        {
            if (!agent)
            {
                continue;
            }
            UnitColor c = agent->getAgentColor();
            DrawCircleV({originX + agent->getX() * scale, originY + agent->getY() * scale},
                        agentRadius,
                        {(unsigned char)(c.red * 2),
                         (unsigned char)(c.green * 2),
                         (unsigned char)(c.blue * 2),
                         (unsigned char)(c.transparency * 2)});
        }
    }
}

void drawMultiProgress(SimManager &manager, Rectangle area, double time)
{
    std::vector<Environment *> &envs = manager.getEnvironments();
    int count = (int)envs.size();
    if (count == 0)
    {
        return;
    }

    // leave room for the caption below the tiles
    float captionHeight = 40;
    Rectangle grid = {area.x, area.y, area.width, std::max(40.0f, area.height - captionHeight)};

    float tile = 84;
    float gap = 16;
    int columns = std::max(1, (int)((grid.width + gap) / (tile + gap)));
    columns = std::min(columns, count);
    int rows = (count + columns - 1) / columns;

    // shrink the tiles if the rows do not fit
    float needed = rows * tile + (rows - 1) * gap;
    if (needed > grid.height)
    {
        tile = std::max(28.0f, (grid.height - (rows - 1) * gap) / rows);
    }

    float gridW = columns * tile + (columns - 1) * gap;
    float gridH = rows * tile + (rows - 1) * gap;
    float startX = grid.x + (grid.width - gridW) / 2;
    float startY = grid.y + (grid.height - gridH) / 2;

    for (int i = 0; i < count; i++)
    {
        Environment *env = envs[i];
        int column = i % columns;
        int row = i / columns;
        Rectangle box = {startX + column * (tile + gap), startY + row * (tile + gap), tile, tile};

        bool finished = env->getState() == EnvironmentState::Finished;
        float progress = (float)env->getCompletedCycles() / (float)(env->getMaxCycle() + 1);
        progress = std::clamp(progress, 0.0f, 1.0f);
        if (finished and env->getCompletedCycles() > 0)
        {
            progress = std::max(progress, 1.0f);
        }

        // fill rises from the bottom as cycles complete
        float fillHeight = (box.height - 6) * progress;
        DrawRectangleRec({box.x + 3, box.y + box.height - 3 - fillHeight, box.width - 6, fillHeight}, {0, 150, 150, 255});

        DrawRectangleLinesEx(box, 2.0f, finished ? Ui::Cyan : Ui::White);

        if (!finished)
        {
            // a small rotating arc shows the environment is still working
            Vector2 center = {box.x + box.width / 2, box.y + box.height / 2};
            float angle = (float)std::fmod(time * 220.0 + i * 35.0, 360.0);
            DrawRing(center, tile * 0.16f, tile * 0.22f, angle, angle + 110, 16, Ui::White);
        }
        else
        {
            Ui::textCentered("done", box, Ui::SmallTextSize, Ui::White);
        }

        Ui::text(std::to_string(env->getIdentifier()), box.x + 5, box.y + 3, Ui::SmallTextSize, Ui::White);
    }

    int finishedCount = manager.getFinishedCount();
    std::string caption = std::to_string(count - finishedCount) + " of " + std::to_string(count) +
                          " environments are still simulating their rounds";
    Rectangle captionArea = {area.x, area.y + area.height - captionHeight, area.width, captionHeight};
    Ui::textCentered(caption, captionArea, Ui::TextSize, Ui::White);
}
