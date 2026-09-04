/*
Last time edited: 04.09.2026
Last edit summary: reworked user input handling
Existing issues:
*
Potential additions:
*/

#include <iostream>
#include <filesystem>

#include "src/data_management/sim_manager.h"
#include "dbgtools/user_input.h"

void visualSim()
{
    float float_input[2] = {};
    int int_input[10] = {};

    getUserInput(
        float_input,
        2,
        int_input,
        10,
        UserInputCase::Visual);

    Environment env = Environment(
        3000,
        float_input[eRad],
        int_input[maxA],
        int_input[maxP],
        float_input[cull],
        int_input[iT],
        int_input[maxCultivate],
        int_input[maxCycle],
        int_input[cb],
        int_input[maxBL],
        int_input[maxBCN],
        int_input[rootNodesCount],
        borderWidth,
        borderHeight);

    env.manageVisualizedSimulation(int_input[fps]);
}

void defaultVisual()
{
    Environment env = Environment();

    env.manageVisualizedSimulation();
}

void multiThreadSim()
{
    float float_input[2] = {};
    int int_input[10] = {};

    int eC;

    getValidInt(
        &eC,
        0,
        1, 32, "Enter simulation environment count: ");

    getUserInput(
        float_input,
        2,
        int_input,
        10,
        UserInputCase::Multithread);

    getValidUInt(
        &borderWidth,
        2,
        UINT_MAX,
        "Enter visual border width (2+): ");

    getValidUInt(
        &borderHeight,
        2,
        UINT_MAX,
        "Enter visual border height (2+): ");

    getValidUInt(
        &agentSize,
        1,
        UINT_MAX,
        "Enter agent size (1+): ");

    getValidUInt(
        &plantSize,
        1,
        UINT_MAX,
        "Enter plant size (1+): ");

    std::cout << "Starting simulation with "
              << eC
              << " environments!\n";

    SimManager sm = SimManager(
        eC,
        float_input[eRad],
        int_input[maxA],
        int_input[maxP],
        float_input[cull],
        int_input[iT],
        int_input[maxCultivate],
        int_input[maxCycle],
        int_input[cb],
        int_input[maxBL],
        int_input[maxBCN],
        int_input[rootNodesCount],
        borderWidth,
        borderHeight);

    sm.runSimulation();
}

void customEnvironment()
{
    std::string saveFileStr;
    int fps;

    std::cout << "Enter environment save file name: ";
    std::cin >> saveFileStr;

    getValidInt(
        &fps, 0, 1, 60, "Enter environment frames per second: ");
    std::cout << fps;
    try
    {
        Environment env = Environment(saveFileStr);

        env.manageVisualizedSimulation(fps);
    }
    catch (...)
    {
        std::cout
            << "An error has been encountered, use global function "
               "DEBUG_LOG to debug code\n";
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
    std::cout
        << "Thank you for using this application, if you have any inquiries, "
           "please refer to the github repository hosted on:\n"
        << "https://github.com/PixelAndShip/EEBBASS\n";
}

// Environmentally engineered behavior based agent simulation system
int main()
{
    std::filesystem::path currentDir =
        std::filesystem::current_path();

    std::filesystem::create_directories(
        currentDir / "EEBBASS" / "logs");

    std::filesystem::create_directories(
        currentDir / "EEBBASS" / "environments");

    std::filesystem::current_path(
        currentDir / "EEBBASS");

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
                std::cout
                    << "Invalid input! Enter h for help!\n";
                break;
            }
            }

            std::cout << "Enter use case: ";
            std::cin >> simTypeChoiceStr;
        }
        catch (...)
        {
            std::cout
                << "An error has been encountered, use global function "
                   "DEBUG_LOG to debug code\n";

            std::cout << "Enter use case: ";
            std::cin >> simTypeChoiceStr;
        }
    }
}
