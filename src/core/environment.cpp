#include "environment.h"
#include <vector>
void testConstruct(std::string fileName)
{
    std::cout << "1 - Starting construct\n";

    std::cout << "2 - Filename: ["
              << fileName
              << "]\n";

    std::cout << "3 - Current directory: ["
              << std::filesystem::current_path().string()
              << "]\n";

    std::filesystem::path fullPath =
        std::filesystem::absolute(fileName);

    std::cout << "4 - Absolute path: ["
              << fullPath.string()
              << "]\n";

    std::cout << "5 - Exists: "
              << std::filesystem::exists(fullPath)
              << "\n";

    std::cout << "6 - Before ifstream\n";

    std::ifstream file(fullPath);

    std::cout << "7 - After ifstream\n";

    if (!file.is_open())
    {
        std::cout << "8 - FAILED TO OPEN\n";
        return;
    }

    std::cout << "9 - FILE OPENED\n";

    std::string line;

    while (std::getline(file, line))
    {
        std::cout << "LINE: ["
                  << line
                  << "]\n";
    }

    std::cout << "10 - Finished reading\n";

    file.close();
}

Environment::Environment(int id, float eRad, int maxAC, int maxPC, float cullP, int iT, int maxIT, int maxCYCLE, int cb, int maxBL, int maxBCN, int rootNodesCount, int borderW, int borderH)
{

    identifier = id;
    radiation = eRad;
    maxAgentCount = maxAC;
    maxPlantCount = maxPC;
    cullPercentage = cullP;
    iteration = iT;
    maxCultivateIteration = maxIT;
    maxCycle = maxCYCLE;
    carbon_count = cb;
    custom = false;

    maxRootNodes = rootNodesCount;
    maxBrainLevel = maxBL;
    maxBrainChildNodes = maxBCN;
    borderWidth = borderW;
    borderHeight = borderH;

    spider = new Spider(eRad, rootNodesCount, maxBL, maxBCN);

    rootNodeDist = std::uniform_int_distribution<>(0, maxRootNodes);
    insideX = std::uniform_int_distribution<>(0, borderWidth);
    insideY = std::uniform_int_distribution<>(0, borderHeight);
}
Environment::Environment(std::string saveFile)
{
    std::cout << "Opening file";
    testConstruct(saveFile);
    constructEnvironment(saveFile);
    rootNodeDist = std::uniform_int_distribution<>(0, maxRootNodes);
    insideX = std::uniform_int_distribution<>(0, borderWidth);
    insideY = std::uniform_int_distribution<>(0, borderHeight);
    custom = true;
}

Environment::~Environment()
{

    logEnvironment();

    delete spider;
}

EnvironmentState Environment::getState() const
{
    return environmentState;
}

void Environment::setState(EnvironmentState iES)
{
    environmentState = iES;
}

void Environment::manageSimulation()
{

    if (custom)
    {
        startSimulation(0, maxPlantCount);
    }
    else
    {
        startSimulation(maxAgentCount, maxPlantCount);
    }

    int cycle = 0;
    // std::string windowName = "Environment" + std::to_string(identifier);
    // InitWindow(800, 800, windowName.c_str());
    // SetTargetFPS(60);
    int AgentsCount = 0;
    int PlantsCount = 0;
    // std::string text = "";
    iteration = 0;
    environmentState = EnvironmentState::Running;
    while (cycle <= maxCycle)
    {

        AgentsCount = (int)spider->Agents.size();
        PlantsCount = (int)spider->Plants.size();
        // text = std::to_string(AgentsCount) + "|" + std::to_string(PlantsCount) + "|" + std::to_string(iteration);
        if (iteration >= maxCultivateIteration)
        {

            environmentState = EnvironmentState::Paused;
            cultivateSimulation(maxAgentCount);
            iteration = 0;
            environmentState = EnvironmentState::Running;
            cycle += 1;
        }

        manageMoment();
        // BeginDrawing();
        // ClearBackground(BLACK);
        // makeWindow();
        // DrawText(text.c_str(), 20, 20, 30, WHITE);
        // EndDrawing();
        iteration++;
    }
    environmentState = EnvironmentState::Finished;
}

void Environment::manageVisualizedSimulation()
{

    if (custom)
    {
        startSimulation(0, 50);
    }
    else
    {
        startSimulation(maxAgentCount, maxPlantCount);
    }

    std::string windowName = "Environment" + std::to_string(identifier);
    int X = borderWidth * agentSize * 2;
    int Y = borderHeight * agentSize * 2;
    InitWindow(X, Y, windowName.c_str());
    SetTargetFPS(5);
    int AgentsCount = 0;
    int PlantsCount = 0;
    std::string agentCountSTR, plantCountSTR, iterationSTR;
    iteration = 0;
    environmentState = EnvironmentState::Running;
    while (!WindowShouldClose())
    {

        AgentsCount = (int)spider->Agents.size();
        PlantsCount = (int)spider->Plants.size();
        agentCountSTR = "Agent count: " + std::to_string(AgentsCount);
        plantCountSTR = "Plant count: " + std::to_string(PlantsCount);
        iterationSTR = "Iteration: " + std::to_string(iteration);
        if (iteration >= maxCultivateIteration)
        {

            environmentState = EnvironmentState::Paused;
            cultivateSimulation(maxAgentCount);
            iteration = 0;
            environmentState = EnvironmentState::Running;
        }

        manageMoment();
        BeginDrawing();
        ClearBackground(BLACK);
        makeWindow();
        DrawText(agentCountSTR.c_str(), 20, 20, 30, YELLOW);
        DrawText(plantCountSTR.c_str(), 20, 50, 30, GREEN);
        DrawText(iterationSTR.c_str(), 20, 80, 30, BLUE);
        EndDrawing();
        iteration++;
    }
    environmentState = EnvironmentState::Finished;
    CloseWindow();
}

void Environment::cultivateSimulation(int targetPop)
{

    std::vector<std::pair<std::string, float>> rankedAgents;

    for (auto &[coords, agent] : spider->Agents)
    {
        if (agent == nullptr)
            continue;

        float fitness = agent->getHealth() + agent->getEnergy();
        rankedAgents.push_back({coords, fitness});
    }

    std::sort(rankedAgents.begin(), rankedAgents.end(),
              [](const auto &a, const auto &b)
              {
                  return a.second > b.second;
              });

    int originalPopulation = rankedAgents.size();
    int survivors = originalPopulation / 2;
    if (survivors > 0)
    {
        maxCultivateIteration++;
    }
    else
    {
        maxCultivateIteration--;
    }
    for (int i = 0; i < survivors; i++)
    {
        Agent *agent = spider->Agents[rankedAgents[i].first];

        if (agent != nullptr)
        {

            agent->logAgent(iteration);
            agent->setHealth(100);
            agent->setEnergy(100);
        }
    }

    for (int i = survivors; i < originalPopulation; i++)
    {
        auto it = spider->Agents.find(rankedAgents[i].first);

        if (it != spider->Agents.end())
        {
            delete it->second;
            spider->Agents.erase(it);
        }
    }
    int plantTargetPop = 100 - (int)spider->Plants.size();
    int agentTargetPop = targetPop - survivors;
    if (plantTargetPop <= 0)
    {
        plantTargetPop = 50;
    }
    if (targetPop - survivors <= 0)
    {
        agentTargetPop = 100;
    }
    if (custom)
    {
        startSimulation(
            0, plantTargetPop);
    }
    else
    {
        startSimulation(agentTargetPop, plantTargetPop);
    }
}

void Environment::managePlantCount(int count, float cullingCount)
{
    int goalPlantCount = count;
    if (spider->Plants.size() > goalPlantCount)
    {
        int cL = spider->Plants.size() * cullingCount;

        std::vector<std::string> ids;

        for (auto &[id, plant] : spider->Plants)
        {
            if (plant != nullptr)
            {
                ids.push_back(id);
            }
        }

        std::shuffle(
            ids.begin(),
            ids.end(),
            gen);

        for (int i = 0; i < cL and i < ids.size(); i++)
        {
            auto it = spider->Plants.find(ids[i]);

            if (it != spider->Plants.end())
            {
                delete it->second;
                spider->Plants.erase(it);
            }
        }
    }
}

void Environment::manageAgentCount(int count, float cullingCount)
{
    int goalAgentCount = count;
    if (spider->Agents.size() > goalAgentCount)
    {
        int cL = spider->Agents.size() * cullingCount;

        std::vector<std::string> ids;

        for (auto &[id, agent] : spider->Agents)
        {
            if (agent != nullptr)
            {
                ids.push_back(id);
            }
        }

        std::shuffle(
            ids.begin(),
            ids.end(),
            gen);

        for (int i = 0; i < cL and i < ids.size(); i++)
        {
            auto it = spider->Agents.find(ids[i]);

            if (it != spider->Agents.end())
            {
                delete it->second;
                spider->Agents.erase(it);
            }
        }
    }
}

void Environment::manageMoment()
{

    Agent *agent = nullptr;

    for (auto it = spider->Agents.begin();
         it != spider->Agents.end();)
    {
        agent = it->second;

        if (agent == nullptr)
        {
            it = spider->Agents.erase(it);
            continue;
        }

        float oldHealth = agent->getHealth();
        float oldEnergy = agent->getEnergy();
        float oldSpeed = agent->getSpeed();

        agent->setHealth(oldHealth - 1);
        agent->setEnergy(oldEnergy - 1);
        agent->setSpeed(oldSpeed - 1);

        if (agent->getEnergy() <= 0.1)
        {
            agent->setEnergy(0);
            agent->setHealth(
                agent->getHealth() - 1);
        }
        if (agent->getSpeed() <= 0.1)
        {
            agent->setSpeed(0);
        }
        if (agent->getHealth() <= 0.1)
        {
            delete agent;
            it = spider->Agents.erase(it);
        }
        else
        {
            agent->updateColor();
            ++it;
        }
    }
    Plant *plant = nullptr;
    for (auto pl = spider->Plants.begin();
         pl != spider->Plants.end();)
    {
        plant = pl->second;

        if (plant == nullptr)
        {
            pl = spider->Plants.erase(pl);
            continue;
        }

        float oldPlantHealth = plant->getHealth();

        plant->setHealth(
            oldPlantHealth - 1);

        if (plant->getHealth() <= 0.1)
        {

            delete plant;

            pl = spider->Plants.erase(pl);
        }
        else
        {
            plant->updateColor();
            ++pl;
        }
    }

    spider->manageSubMoment();
    manageAgentCount(maxAgentCount, cullPercentage);
    managePlantCount(maxPlantCount, cullPercentage);
}

// void Environment::manageSubMoment(std::string coords)
// {
//     DEBUG_LOG("Managing sub moment for "
//               << coords);

//     if (coords == "")
//     {
//         DEBUG_LOG("Invalid empty coordinates");
//         return;
//     }

//     if (spider->proximateCoords.find(coords) ==
//         spider->proximateCoords.end())
//     {
//         DEBUG_LOG("Coordinates not initialized in proximity map");

//         spider->proximateCoords.clear();

//         spider->proximateCoords[coords] = true;

//         DEBUG_LOG("Setting proximities");

//         spider->setProximities(coords);

//         DEBUG_LOG("Managing Spider sub moment");

//         spider->manageSubMoment();
//     }
//     else
//     {
//         DEBUG_LOG("Coordinates already processed");
//     }

//     DEBUG_LOG("Finished sub moment for "
//               << coords);
// }
void Environment::makeWindow()
{

    for (auto &[coords, agent] : spider->Agents)
    {
        if (!agent)
        {
            continue;
        }
        UnitColor color = agent->getAgentColor();

        DrawCircle(
            agent->getX(),
            agent->getY(),
            agentSize,
            {(unsigned char)(color.red * 2),
             (unsigned char)(color.green * 2),
             (unsigned char)(color.blue * 2),
             (unsigned char)(color.transparency * 2)});
    }
    for (auto &[coords, plant] : spider->Plants)
    {
        if (!plant)
        {
            continue;
        }
        UnitColor plC = plant->getPlantColor();
        DrawCircle(
            plant->getX(),
            plant->getY(),
            plantSize,
            {(unsigned char)plC.red,
             (unsigned char)plC.green,
             (unsigned char)plC.blue,
             (unsigned char)plC.transparency});
    }
}

void Environment::startSimulation(int agentCount, int plantCount)
{
    int AgentCount = agentCount - (int)spider->Agents.size();
    int PlantCount = plantCount - (int)spider->Plants.size();
    if (AgentCount <= 0)
    {
        AgentCount = 0;
    }
    bool inside = false;
    for (int i = 0; i < AgentCount; i++)
    {
        Agent *genesis =
            new Agent(
                identifier,
                radiation,
                maxBrainChildNodes,
                maxBrainLevel);

        generateAgentCoords(genesis);

        inside = genesis->getX() > 0 and genesis->getX() < (borderWidth * agentSize * 2) and genesis->getY() > 0 and genesis->getY() < (borderHeight * agentSize * 2);

        if (!inside)
        {

            delete genesis;
            continue;
        }

        spider->Agents[genesis->getCoords()] =
            genesis;
    }
    if (PlantCount <= 0)
    {
        return;
    }
    for (int j = 0; j < PlantCount; j++)
    {
        Plant *pl = new Plant(radiation, gen);
        generatePlantCoords(pl);

        inside = pl->getX() > 0 and pl->getX() < (borderWidth * agentSize * 2) and pl->getY() > 0 and pl->getY() < (borderHeight * agentSize * 2);

        if (!inside)
        {

            delete pl;
            continue;
        }
        spider->Plants[pl->getCoords()] = pl;
    }
}

void Environment::generateAgentCoords(Agent *ag)
{

    int cycles = 0;
    if (ag == nullptr)
    {

        return;
    }
    while (true)
    {
        if (cycles >= 300)
        {

            return;
        }

        int x = insideX(gen) * agentSize * 2;
        int y = insideY(gen) * agentSize * 2;

        bool occupied = true;
        std::string coords = std::to_string(x) + "_" + std::to_string(y);
        if (spider->Agents.find(coords) == spider->Agents.end() and spider->Plants.find(coords) == spider->Plants.end())
        {
            occupied = false;
        }

        if (occupied == false)
        {

            ag->setX(x);
            ag->setY(y);

            return;
        }
        cycles += 1;
    }
}

void Environment::generatePlantCoords(Plant *pl)
{

    int cycles = 0;
    if (pl == nullptr)
    {

        return;
    }
    while (true)
    {
        if (cycles >= 300)
        {
            return;
        }

        int x = insideX(gen) * agentSize * 2;
        int y = insideY(gen) * agentSize * 2;
        bool occupied = true;
        std::string coords = std::to_string(x) + "_" + std::to_string(y);
        if (spider->Agents.find(coords) == spider->Agents.end() and spider->Plants.find(coords) == spider->Plants.end())
        {
            occupied = false;
        }

        if (occupied == false)
        {

            pl->setX(x);
            pl->setY(y);

            return;
        }
        cycles += 1;
    }
}

void Environment::logEnvironment(std::string fileName)
{

    if (fileName == "")
    {
        fileName = "environments/Environment_" + std::to_string(identifier) + ".txt";
    }
    std::ofstream saveFile(fileName);
    std::string data = "Environment\n";
    data += 'I' + std::to_string(identifier);
    data += "/R" + std::to_string(radiation);
    data += "/a" + std::to_string(maxAgentCount);
    data += "/p" + std::to_string(maxPlantCount);
    data += "/e" + std::to_string(cullPercentage);
    data += "/C" + std::to_string(maxCultivateIteration);
    data += "/Y" + std::to_string(maxCycle);
    data += "/c" + std::to_string(carbon_count);
    data += "/O" + std::to_string(maxRootNodes);
    data += "/L" + std::to_string(maxBrainLevel);
    data += "/N" + std::to_string(maxBrainChildNodes);
    data += "/W" + std::to_string(borderWidth);
    data += "/H" + std::to_string(borderHeight);
    data += "/A" + std::to_string(agentSize);
    data += "/P" + std::to_string(plantSize);
    data += "\nAgents\n";
    saveFile << data;
    saveFile.close();
    for (auto &[coordinates, agent] : spider->Agents)
    {
        agent->logAgent(iteration, fileName);
    }
}

void Environment::constructEnvironment(std::string fileName)
{
    std::cout << "Starting construct";
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        return;
    }
    std::string line, environmentData, agentData, brainData;
    Agent *currentAgent = nullptr;
    bool processingAgentData = false;
    bool processingEnvironmentData = false;

    while (std::getline(file, line))
    {
        if (!line.empty() and line.back() == '\r')
        {
            line.pop_back();
        }
        if (processingEnvironmentData == true)
        {
            std::cout << "Environment start";
            processingEnvironmentData = false;
            environmentData = line;
            setCustomEnvironmentValues(environmentData);
            spider = new Spider(radiation, maxRootNodes, maxBrainLevel, maxBrainChildNodes);
        }
        else if (line == "Agents")
        {
            processingEnvironmentData = false;
        }
        else if (line == "Environment")
        {
            processingEnvironmentData = true;
        }
        else if (line == "A")
        {
            processingAgentData = true;
        }
        else if (line == "B")
        {
            processingAgentData = false;
        }
        else if (line == "=")
        {
            std::cout << "Agent start";
            currentAgent = new Agent(agentData, brainData, identifier);
            spider->Agents[currentAgent->getCoords()] = currentAgent;
            currentAgent->updateColor();
            currentAgent = nullptr;
            agentData.clear();
            brainData.clear();
        }
        else if (processingAgentData and !processingEnvironmentData)
        {
            agentData += line;
        }
        else if (!processingAgentData and !processingEnvironmentData)
        {
            brainData += line;
        }
    }
    file.close();
}

void Environment::setCustomEnvironmentValues(std::string data)
{
    // data += 'I' + std::to_string(identifier);
    // data += "/R" + std::to_string(radiation);
    // data += "/C" + std::to_string(maxCultivateIteration);
    // data += "/Y" + std::to_string(maxCycle);
    // data += "/c" + std::to_string(carbon_count);
    // data += "/L" + std::to_string(maxBrainLevel);
    // data += "/N" + std::to_string(maxBrainChildNodes);
    // data += "/W" + std::to_string(borderWidth);
    // data += "/H" + std::to_string(borderHeight);
    // data += "/A" + std::to_string(agentSize);
    // data += "/P" + std::to_string(plantSize);

    size_t pos = 0;

    while (pos < data.size())
    {
        char indicator = data[pos];
        switch (indicator)
        {
        case 'I':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            identifier = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'R':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            radiation = std::stof(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'a':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            maxAgentCount = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'p':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            maxPlantCount = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'e':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            cullPercentage = std::stof(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'C':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            maxCultivateIteration = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'Y':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            maxCycle = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'c':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            carbon_count = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'O':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            maxRootNodes = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'L':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            maxBrainLevel = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'N':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            maxBrainChildNodes = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'W':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            borderWidth = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'H':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            borderHeight = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'A':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            agentSize = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        case 'P':
        {
            size_t start = pos + 1;
            size_t end = data.find('/', start);

            if (end == std::string::npos)
            {
                end = data.size();
            }
            plantSize = std::stoi(data.substr(start, end - start));
            pos = end;
            break;
        }
        default:
        {
            pos++;
            break;
        }
        }
    }
}