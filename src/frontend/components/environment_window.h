
#pragma once
#include <raylib.h>
#include "src/backend/core/environment.h"
#include "src/backend/data_management/sim_manager.h"

void drawEnvironmentWorld(Environment &environment, Rectangle area);

void drawMultiProgress(SimManager &manager, Rectangle area, double time);
