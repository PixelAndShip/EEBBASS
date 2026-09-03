/*
Last time edited: 03.09.2026
Last edit summary: adding documentation
Existing issues:
* no input safety
Potential additions:
*/

#include <iostream>
#include "src/data_management/sim_manager.h"

void visualSim()
{
    float eRad, cull;
    int maxA, maxP, iT, maxCultivate, maxCycle, cb, maxBL, maxBCN, rootNodesCount, fps;
    std::cout << "Enter environment frames per second 1+: ";
    std::cin >> fps;
    std::cout << "Enter radiation (0.0 - 1.0): ";
    std::cin >> eRad;
    std::cout << "Enter maximum amount of Agents: ";
    std::cin >> maxA;
    std::cout << "Enter maximum amount of Plants: ";
    std::cin >> maxP;
    std::cout << "Enter cull percentage (0.0 - 1.0): ";
    std::cin >> cull;
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
    Environment env = Environment(3000, eRad, maxA, maxP, cull, iT, maxCultivate, maxCycle, cb, maxBL, maxBCN, rootNodesCount, borderWidth, borderHeight);
    env.manageVisualizedSimulation(fps);
}

void defaultVisual()
{
    Environment env = Environment();
    env.manageVisualizedSimulation();
}

void multiThreadSim()
{
    float eRad, cull;
    int maxA, maxP, iT, maxCultivate, maxCycle, cb, maxBL, maxBCN, rootNodesCount, eC;
    std::cout << "Enter simulation environment count: ";
    std::cin >> eC;
    std::cout << "Enter radiation (0.0 - 1.0): ";
    std::cin >> eRad;
    std::cout << "Enter maximum amount of Agents: ";
    std::cin >> maxA;
    std::cout << "Enter maximum amount of Plants: ";
    std::cin >> maxP;
    std::cout << "Enter cull percentage (0.0 - 1.0): ";
    std::cin >> cull;
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
    SimManager sm = SimManager(eC, eRad, maxA, maxP, cull, iT, maxCultivate, maxCycle, cb, maxBL, maxBCN, rootNodesCount, borderWidth, borderHeight);
    sm.runSimulation();
}

void customEnvironment()
{
    std::string saveFileStr;
    int fps;
    std::cout << "Enter environment save file name: ";
    std::cin >> saveFileStr;
    std::cout << "Enter environment frames per second: ";
    std::cin >> fps;
    try
    {
        Environment env = Environment(saveFileStr);

        env.manageVisualizedSimulation(fps);
    }
    catch (...)
    {
        std::cout << "An error has been encountered, use global function DEBUG_LOG to debug code";
    }
}

void Helper()
{
    std::cout << "EEBBASS current version supports these inputs:\n";
    std::cout << "f to exit application;\n";
    std::cout << "d to run default visual simulation;\n";
    std::cout << "c to enter custom environment file for visualized simulation;\n";
    std::cout << "v to manually customize visualized simulation variables;\n";
    std::cout << "m to multithreaded customized environments simulation;\n";
    std::cout << "h to call helper function\n";
    std::cout << "Thank you for using this application, if you have any inquiries, please refer to the github repository hosted on:\nhttps://github.com/PixelAndShip/EEBBASS\n";
}
// Environmentally engineered behavior based agent simulation system
int main()
{

    std::filesystem::path currentDir = std::filesystem::current_path();
    std::filesystem::create_directories(currentDir / "EEBBASS" / "logs");
    std::filesystem::create_directories(currentDir / "EEBBASS" / "environments");
    std::filesystem::current_path(currentDir / "EEBBASS");

    char simTypeChoiceStr;
    std::cout << "Enter use case, h for help: ";
    std::cin >> simTypeChoiceStr;

    while (simTypeChoiceStr != 'f')
    {
        try
        {
            switch (simTypeChoiceStr)
            {
            case 'h':
            {
                Helper();
                break;
            }
            case 'c':
            {
                customEnvironment();
                break;
            }
            case 'd':
            {
                defaultVisual();
                break;
            }
            case 'v':
            {
                visualSim();
                break;
            }
            case 'm':
            {
                multiThreadSim();
                break;
            }
            default:
            {
                std::cout << "Invalid input! Enter h for help!";
                break;
            }
            }
            std::cout << "Enter use case: ";
            std::cin >> simTypeChoiceStr;
        }
        catch (...)
        {
            std::cout << "An error has been encountered, use global function DEBUG_LOG to debug code";
            std::cout << " Enter use case: ";
            std::cin >> simTypeChoiceStr;
        }
    }
}
