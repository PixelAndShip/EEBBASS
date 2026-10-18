#include "simulation_controller.h"
#include <algorithm>
#include <chrono>

namespace
{
    constexpr double MaxStepsPerSecond = 100000;
    constexpr double FrameBudgetSeconds = 0.012; // simulation time allowed per rendered frame
}

SimulationController::~SimulationController()
{
    kill();
}

void SimulationController::startVisual(const EnvironmentSettings &s)
{
    kill();

    agentSize = s.agentSize;
    plantSize = s.plantSize;

    visual = std::make_unique<Environment>(
        s.identifier,
        s.radiation,
        s.maxAgents,
        s.maxPlants,
        s.cullPercentage,
        s.iterationStart,
        s.maxCultivateIterations,
        s.maxCycles,
        s.carbonCount,
        s.maxBrainDepth,
        s.maxBrainChildNodes,
        s.brainRootNodes,
        s.borderWidth,
        s.borderHeight);

    visual->start();
    setStepsPerSecond(s.stepsPerSecond);
    pendingSteps = 0;
    currentMode = Mode::Visual;
}

void SimulationController::startDefault()
{
    EnvironmentSettings defaults;
    defaults.identifier = 5000;
    startVisual(defaults);
}

bool SimulationController::startFromSave(const std::string &path, double stepsPerSecond)
{
    kill();

    try
    {
        std::unique_ptr<Environment> loaded = std::make_unique<Environment>(path);
        if (loaded->spider == nullptr)
        {
            return false;
        }
        visual = std::move(loaded);
    }
    catch (...)
    {
        return false;
    }

    visual->start();
    setStepsPerSecond(stepsPerSecond);
    pendingSteps = 0;
    currentMode = Mode::Visual;
    return true;
}

void SimulationController::startMulti(const EnvironmentSettings &s)
{
    kill();

    agentSize = s.agentSize;
    plantSize = s.plantSize;

    multi = std::make_unique<SimManager>(
        s.environmentCount,
        s.radiation,
        s.maxAgents,
        s.maxPlants,
        s.cullPercentage,
        s.iterationStart,
        s.maxCultivateIterations,
        s.maxCycles,
        s.carbonCount,
        s.maxBrainDepth,
        s.maxBrainChildNodes,
        s.brainRootNodes,
        s.borderWidth,
        s.borderHeight);

    multi->startSimulation();
    currentMode = Mode::Multi;
}

void SimulationController::setStepsPerSecond(double stepsPerSecond)
{
    speed = std::clamp(stepsPerSecond, 0.0, MaxStepsPerSecond);
}

void SimulationController::update(double deltaSeconds)
{
    if (currentMode != Mode::Visual or !visual)
    {
        return;
    }

    // a long stall (window dragged, debugger) must not turn into a burst of steps
    pendingSteps += std::min(deltaSeconds, 0.1) * speed;

    auto begin = std::chrono::steady_clock::now();
    while (pendingSteps >= 1.0)
    {
        visual->step();
        pendingSteps -= 1.0;
        totalSteps++;

        std::chrono::duration<double> spent = std::chrono::steady_clock::now() - begin;
        if (spent.count() > FrameBudgetSeconds)
        {
            // could not keep up: drop the backlog instead of falling ever further behind
            pendingSteps = 0;
            break;
        }
    }
}

void SimulationController::kill()
{
    // destroying the manager stops and joins its threads first
    multi.reset();
    visual.reset();
    currentMode = Mode::None;
    pendingSteps = 0;
}

bool SimulationController::multiFinished() const
{
    return currentMode == Mode::Multi and multi and !multi->isRunning();
}
