#include <iostream>
#include "data_management/sim_manager.h"

#include "test.h"

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
    int borderX;
    int borderY;
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
int main()
{
    std::string fileName = "";
    std::string simTypeChoiceStr = "";
    std::cout << "Enter simulation type: ";
    std::cin >> simTypeChoiceStr;
    if (simTypeChoiceStr == "v")
    {
        visualSim();
        return 0;
    }
    else if (simTypeChoiceStr == "c")
    {
        std::cout << "Enter custom brain file name: ";
        std::cin >> fileName;
        Agent *customA = new Agent(fileName, true, 4000, 200, 200, 0.6, {200, 200, 255, 255}, 300, 300, 4);
        customA->getBrain().logBrain();
        return 0;
    }
    //  Agent(std::string fileName, bool iP, int iEID, float iH, float iE, float iPD, UnitColor iC, int ix, int iy, float iS);
    std::string eC = "";
    std::cout << "Enter simulation environment count: ";
    std::cin >> eC;
    try
    {

        int iEC = std::stoi(eC);
        std::cout << "Starting simulation with " << eC << " environments!";
        SimManager sm = SimManager(iEC);
        sm.runSimulation();
    }
    catch (...)
    {
        std::cout << "Custom environment count failed, starting with default 8 environments!";
        SimManager sm = SimManager();
        sm.runSimulation();
    }

    // Agent a = Agent(e.radiation, e.gen, e.dist, e.maxBrainChildNodes, e.maxBrainLevel);
    // Agent b = Agent(e.radiation, e.gen, e.dist, e.maxBrainChildNodes, e.maxBrainLevel);
    // Agent c = Agent(e.radiation, e.gen, e.dist, e.maxBrainChildNodes, e.maxBrainLevel);
    // Plant p = Plant();

    // a.setX(240);
    // b.setX(50);
    // c.setX(111);
    // e.spider->PastAgents.insert({a.getCoords(), &a});
    // e.spider->PastAgents.insert({b.getCoords(), &b});
    // e.spider->PastAgents.insert({c.getCoords(), &c});

    // e.spider->PastPlants.insert({"26_25", &p});
    // for (auto a : e.spider->PastAgents)
    // {
    //     std::cout << "|||" << a.first << "|||" << "\n";
    //     for (InputNode *i : a.second->getBrain().getInputNodes())
    //     {
    //         Test::printBrain(i);
    //     }
    // }
    // e.manageMoment();

    // e.makeWindow();

    // Test::getType(&p);
    // Test::getType(&a);
    // Test::proximityCheck(&a);
}
