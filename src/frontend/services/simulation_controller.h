

#pragma once
#include <memory>
#include <string>
#include "src/backend/core/environment.h"
#include "src/backend/data_management/sim_manager.h"

struct EnvironmentSettings
{
    int environmentCount = 4; // multi only
    float radiation = 0.5f;
    float cullPercentage = 0.25f;
    int maxAgents = 200;
    int maxPlants = 100;
    int iterationStart = 0;
    int maxCultivateIterations = 115;
    int maxCycles = 100;
    int carbonCount = 0;
    int maxBrainDepth = 5;
    int maxBrainChildNodes = 2;
    int brainRootNodes = 7;
    int borderWidth = 700;
    int borderHeight = 700;
    int agentSize = 4;
    int plantSize = 3;
    double stepsPerSecond = 10; // visual only
    int identifier = 3000;
};

class SimulationController
{
public:
    enum class Mode
    {
        None,
        Visual,
        Multi
    };

    ~SimulationController();

    void startVisual(const EnvironmentSettings &settings);
    void startDefault(); // visual with default values

    bool startFromSave(const std::string &path, double stepsPerSecond);
    void startMulti(const EnvironmentSettings &settings);

    void update(double deltaSeconds);

    // Stop and discard whatever is running
    void kill();

    // true once a multi simulation ran every environment to its end
    bool multiFinished() const;

    Mode mode() const { return currentMode; }
    bool active() const { return currentMode != Mode::None; }
    Environment *environment() { return visual.get(); }
    SimManager *manager() { return multi.get(); }

    double stepsPerSecond() const { return speed; }
    void setStepsPerSecond(double stepsPerSecond);
    long long stepsTaken() const { return totalSteps; }

private:
    Mode currentMode = Mode::None;
    std::unique_ptr<Environment> visual;
    std::unique_ptr<SimManager> multi;
    double speed = 10;
    double pendingSteps = 0;
    long long totalSteps = 0;
};
