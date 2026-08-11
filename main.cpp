#include <iostream>
#include "data_management/sim_manager.h"

#include "test.h"

#include <filesystem>

void visualSim()
{
    float eRad;
    int iT;
    int maxCultivate;
    int maxCycle;
    int cb;
    int maxBL;
    int maxBCN;
    int rootNodesCount;
    std::cout << "Enter radiation (0.0 - 1.0): ";
    std::cin >> eRad;
    std::cout << "Enter iteration start (number): ";
    std::cin >> iT;
    std::cout << "Enter max cultivation iterations (number): ";
    std::cin >> maxCultivate;
    std::cout << "Enter max total cycles (number): ";
    std::cin >> maxCycle;
    std::cout << "Enter carbon count (number) (not implemented yet): ";
    std::cin >> cb;
    std::cout << "Enter max brain depth (1+): ";
    std::cin >> maxBL;
    std::cout << "Enter max brain child nodes count (1+): ";
    std::cin >> maxBCN;
    std::cout << "Enter brain root nodes count (1+): ";
    std::cin >> rootNodesCount;
    std::cout << "Enter visual border width (2+): ";
    std::cin >> borderWidth;
    std::cout << "Enter visual border height (2+): ";
    std::cin >> borderHeight;
    std::cout << "Enter agent size (1+): ";
    std::cin >> agentSize;
    std::cout << "Enter plant size (1+): ";
    std::cin >> plantSize;
    Environment env = Environment(3000, eRad, iT, maxCultivate, maxCycle, cb, maxBL, maxBCN, rootNodesCount, borderWidth, borderHeight);
    env.manageVisualizedSimulation();
}

void multiThreadSim()
{
    float eRad;
    int iT;
    int maxCultivate;
    int maxCycle;
    int cb;
    int maxBL;
    int maxBCN;
    int rootNodesCount;
    int eC;
    std::cout << "Enter simulation environment count: ";
    std::cin >> eC;
    std::cout << "Enter radiation (0.0 - 1.0): ";
    std::cin >> eRad;
    std::cout << "Enter iteration start (number): ";
    std::cin >> iT;
    std::cout << "Enter max cultivation iterations (number): ";
    std::cin >> maxCultivate;
    std::cout << "Enter max total cycles (number): ";
    std::cin >> maxCycle;
    std::cout << "Enter carbon count (number) (not implemented yet): ";
    std::cin >> cb;
    std::cout << "Enter max brain depth (1+): ";
    std::cin >> maxBL;
    std::cout << "Enter max brain child nodes count (1+): ";
    std::cin >> maxBCN;
    std::cout << "Enter brain root nodes count (1+): ";
    std::cin >> rootNodesCount;
    std::cout << "Enter visual border width (2+): ";
    std::cin >> borderWidth;
    std::cout << "Enter visual border height (2+): ";
    std::cin >> borderHeight;
    std::cout << "Enter agent size (1+): ";
    std::cin >> agentSize;
    std::cout << "Enter plant size (1+): ";
    std::cin >> plantSize;

    std::cout << "Starting simulation with " << eC << " environments!";
    SimManager sm = SimManager(eC, eRad, iT, maxCultivate, maxCycle, cb, maxBL, maxBCN, rootNodesCount, borderWidth, borderHeight);
    sm.runSimulation();
}

int main()
{

    std::filesystem::path currentDir = std::filesystem::current_path();
    std::filesystem::create_directories(currentDir / "ES" / "logs");
    std::filesystem::current_path(currentDir / "ES");

    char simTypeChoiceStr;
    std::cout << "Enter simulation type: ";
    std::cin >> simTypeChoiceStr;

    switch (simTypeChoiceStr)
    {
    case 'v':
    {
        visualSim();
        return 0;
    }

    case 'c':
    {
        std::string fileName = "";
        std::cout << "Enter custom brain file name: ";
        std::cin >> fileName;
        Agent *customA = new Agent(fileName, true, 4000, 200, 200, 0.6, {200, 200, 255, 255}, 300, 300, 4);
        customA->logAgent();
        return 0;
    }
    case 'm':
    {
        multiThreadSim();
        return 0;
    }
    default:
    {
        std::cout << "Custom environment count failed, starting with default 8 environments!";
        SimManager sm = SimManager();
        sm.runSimulation();
        break;
    }
    }
}
