# EEBBASS (Environmentally engineered behavior based agent simulation system)

## 1. Short Description

> This is an open source solo project, in which different behaviors are engineered for different environments via evolution based on natural selection. The program provides several modes of operation and customization of agents, environments and visualization of the simulation. <br>
> The main goals for the project are to: <br>
>1.Gain further knowledge and understanding of C++ systems. <br>
>2.Explore individual unit evolution systems.<br>
>3.Gain further knowledge and understanding of Git systems.
---

## 2. Important Notes

### Requirements

Both windows and linux versions use statically linked libraries, so no additional resources should be needed for use.

### Before Running

* Make sure your current directory supports directory creation and file creation. This is needed to save and use save files for environment loading.
* This application uses raylib to visualize simulations, raylib may need to be installed as a library to recompile the source code.
* Save files are overwritten every time the corresponding Environment runs a new simulation, so make sure to save the desired Environment data elsewhere, before launching a new simulation.
* Multithreading option may clog up the CPU cores.
* To use the custom environment option, make sure to have your usable save file in the current working directory, otherwise the current implementation will not be able to read it.
* To make your own Environment save file, please refer to the section 'How It Works', subsection 'Environment simulation derived from save file' for data writing protocols.
* main_linux is compiled using g++.

### Known Issues

* Unknown/unobtainable custom save file entry causes a core dump (crash).
* Invalid use case and variable inputs cause Segmentation fault (crash).
---

## 3. In-Depth Documentation

### Project Structure

```
Project/
├── dbgtools/
│   ├── debug.cpp
│   ├── debug.h
├── external/
├── src/
│   ├── core/
│   |   ├── environment.cpp
│   |   ├── environment.h
│   |   ├── spider.cpp
│   |   ├── spider.h
│   ├── data_management/
│   |   ├── data_types.h
│   |   ├── sim_manager.cpp
│   |   ├── sim_manager.h
│   ├── ui/
│   |   ├── interface.cpp
│   |   ├── interface.h
│   ├── units/
│   |   ├── brain_nodes/
│   |   |   ├── input_node.cpp
│   |   |   ├── input_node.h
│   |   |   ├── node_functions.cpp
│   |   |   ├── node_functions.h
│   |   |   ├── output_node.h
│   |   ├── agent.cpp
│   |   ├── agent.h
│   |   ├── brain.cpp
│   |   ├── brain.h
│   |   ├── plant.cpp
│   |   ├── plant.h
├── test/
│   ├── test.h
├── main.cpp
├── EEBBASS_linux
├── EEBBASS_AS
├── EEBBASS_windows.exe
├── LICENSE
└── README.md
```

IMPORTANT NOTE!

File directory EEBASS is created upon activating the main executable, in which subdirectories environments/ and logs/ are also created. environments/ houses environment save files, logs contain DEBUG_LOG() outputs.



---

### Core Components


#### Input Node

Saves sensory input configuration. Hosts child Input Node pointers and Output Node pointer. Attributes:
<br>
* float weight - chance of activating, set between 0.0 and 1.0
* float setAmount - value from 0 to 255, used in Agent variable input.
* unsigned int key - corresponding src/data_management/data_types.h Senses from getSenses input key, which is later used to verify Node Function.
* UnitColor unitColor - used to identify target, check UnitColor struct in src/data_management/data_types.h for more information.
* std::vector<InputNode *> inputNodes - child Input Node pointers, used to form tree structure. Length is capped to settable max Child Nodes.
* OutputNode *outputNode - leaf Output Node, used in Agent variable output.

#### Output Node 

Saves Agent's output configuration. Attributes:
<br>
* float weight - chance of activating, set between 0.0 and 1.0
* float energyCost - value from 0 to 255, used in Agent variable output.
* unsigned int key - corresponding src/data_management/data_types.h Actions from getActions input key, which is later used to verify Node Function.
* UnitColor unitColor - used to identify target, check UnitColor struct in src/data_management/data_types.h for more information.

#### Node Functions

Enacts change on the hosting Agent or target Agent / Plant, based on the Input and Output Node configurations.

#### Brain 

Hosts root Input Node pointers. The brain structure is a tree with leaf Output Node pointers, settable max child Input Nodes and max tree depth.

#### Agent

Main unit of the environment, hosts the Brain. Hosts logging and saving functions. Attributes:
<br>
* bool processed - flag for moment processing, ensures agent can only enact one Output Node Node Function per moment.
* int env_identifier - environment that the Agent inhabits.
* float health - current health.
* float energy - current energy.
* float plantDiet - coefficient used for biting functions in Node Functions, set from 0.0 to 1.0.
* UnitColor agentColor - current UnitColor.
* int x, y - current position in Environment.
* float speed - current speed, used to sort actions in a moment between Agents.
* Brain brain - current brain.

#### Plant

Source of energy for Agents. Attributes:
<br>
* int env_identifier - environment that the Plant inhabits.
* float health - current health.
* UnitColor plantColor - current UnitColor.
* int x, y - current position in Environment.

#### Environment

Agent and Plant container, used for hosting environmentally locked variables, enacting simulation algorithms and enacting saving and logging protocols for Environment, Agent and Agent Brain data. Attributes:
<br>
* int identifier - unique environment identifier, used for logging and save files, values from 0+.
* float radiation - chance of Agent attribute or Brain structure, Input Node or Output Node attribute mutation, values from 0.0 to 1.0.
* int iteration - current cycle iteration.
* int maxCultivateIteration - max cycle iterations, once reached, a new cycle begins.
* int maxCycle - max amount of cycles in a simulation, once reached, the simulation ends.
* int carbon_count - not yet implemented.
* int maxRootNodes - max amount of Brain root Input Nodes.
* int maxBrainLevel - max depth of Brain Input Node child nodes.
* int maxBrainChildNodes - max amount of each Input Node child Input Nodes.
* bool custom - used for simulation configuration, if custom is true, only the Agents inputed from a save file will be created and cultivated.
* EnvironmentState environmentState - not yet implemented.

#### Spider

The most important mechanism - it ties environment simulation to each Agent and their Brain nodes. Manages each moment (iteration) by retrieving each Agent's actionable Output Node (via sensory Input Nodes in the Agent's Brain) and enacting the appropriate Node Functions. Attributes:
<br>
* std::unordered_map<std::string, Agent *> Agents - pointers to all Environment bound Agents.
* std::unordered_map<std::string, Plant *> Plants - pointers to all Environment bound Plants.
* std::vector<std::pair<std::string, OutputNode *>> actionQueue - used for sorting actions by Agent Speed attribute.
* std::vector<std::pair<std::string, Agent *>> pendingBirths - used for creating new Agents via Split function enacted by existing Agents.
* float radiation - chance of Agent attribute or Brain structure, Input Node or Output Node attribute mutation, values from 0.0 to 1.0.
* int maxRootNodes - max amount of Brain root Input Nodes.
* int maxBrainLevel - max depth of Brain Input Node child nodes.
* int maxBrainChildNodes - max amount of each Input Node child Input Nodes.

#### Data types

Hosts global variables and functions.

#### Sim Manager

Enacts multithreading functionality.

#### Interface

Not yet implemented, future designs point toward expanding windowed user interfaces.

#### Debug tools

Provides DEBUG_LOG() for source code debugging.

---

### How It Works

```
The Core Loop:

The program can run in 2 modes, Visualized and Not Visualized.

1) Visualized:

Called via terminal 'd' for default values, 'v' for custom values entered via terminal.

An Environment instance calls manageVisualizedSimulation(), in which 1) startSimulation() is called to create Agents and Plants, 2) A while loop, conditional upon window being opened, manageMoment() and Environment instanced attribute Spider class spider - manageSubMoment() are called each iteration to manage Unit stats and Agent behaviors. When iteration reaches maxCultivateIteration, cultivateSimulation() is called to get rid of the weaker scoring Agents and create new Agents to promote behavior diversity. If there are no survivors after one cycle, the maxCultivateIteration is decreased by 1; if instead survivors are found, it is increased by 1.

2) Not Visualized:

A Sim Manager instance creates specified amount of Environment instances for which threads are mapped and manageSimulation() is callled. manageSimulation() enacts the same loop as manageVisualizedSimulation(), as mentioned in 1) Visualized:, but does not create raylib windows. The Environment loop and simulation end when the specified amount of maxCycle is reached.

```

```
Spider:

Main function of Spider is manageSubMoment, that is divided into 7 parts:
1) A loop through Agents, that acquires their momentary action OutputNode pointer via getAction(), that is pushed back in actionQueue.
2) actionQueue is sorted by their corresponding Agent speed attributes, highest to lowest.
3) Actions are enacted via manageAction(), which calls corresponding Node Function, as well as managed Agent processed attributes are set to true.
4) For all Agents who changed their x and y attributes, their coordinates are saved in a temporary vector called 'moves'.
5) A loop through moves updates Agents map with new coordinates.
6) A loop through Agents to be created.
7) Resetting all Agent processed to false.

```


```
Environment Simulation Derived From A Save File:

Called via terminal with input 'c', the program requires another input for the save file name, with which the Environment is then constructed. The protocol for writing Environment data is as follows (text written inside () is only used in this explanatory case):

Environment
I(identifier - int value 0+ including)/R(radiation - float value from 0.0 to 1.0 including)/a(max amount of Agents - int value, from 1+ included)/p(max amount of Plants - int value, from 1+ included)/e(culling percentage - float value from 0.0 to 1.0 included)/C(max cultivate iterations - int value  from 1+ included)/Y(max cycle - int value from 1+ included)/c(carbon count - not yet implemented, any int value included)/O(max Agent Brain root Input Node count - int value from 1+ included)/L(max Brain child Input Node depth - int value from 1+ included)/N(max Brain Input Node child node Input Node count- int value from 1+ included)/W(environment border width - int value from agent size+ not included)/H(environment border height - int value from agent size+ not included)/A(agent size - int value from 1+ included)/P(plant size - int value from 1+ included)
Agents

A (Agent data)
P(processed - bool value)/I(environment id - int value, the exact same as identifier in Environment data)/H(health - int value)/E(energy - int value)/D(plant diet - float value from 0.0 to 1.0 included)/C(agent color - UnitColor value, inclosed by {})/X(agent coordinates x in environment - int value inside borders not included)/Y(agent coordinates y in environment - int value inside borders not included)/S(speed - float value)
B (Brain data) (IMPORTANT NOTE! All nodes are to follow each other via linear progression in coordinates, otherwise they will not be able to connect to non-existing nodes)
[1](coordinate in tree structure)+(+ indicates Input Node)W(node activation weight - float value from 0.0 to 1.0 included)/E(set amount - int value from 0 to 255 included)/K(sense key - int value see in src/data_management/data_types.h)/C(target color - UnitColor value, inclosed by {});
[1.1](coordinate in tree structure)-(- indicated Output Node)W(node activation weight - float value from 0.0 to 1.0 included)/E(energy cost - int value from 0 to 255 included)/K(action key - int value see in src/data_management/data_types.h)/C(target color - UnitColor value, inclosed by {});
=(end of current Agent data)

Example:

Environment
I0/R0.500000/a180/p70/e0.350000/C100/Y100/c1/O4/L3/N3/W200/H200/A2/P2
Agents
A
P0/I0/H99.000000/E99.000000/D0.590000/C{99,99,0,255}/X148/Y48/S0.000000
B
[1]+W0.480000/E24.000000/K10/C{99,128,184,104};
[2]+W0.120000/E101.000000/K13/C{255,71,86,218};
[2.1]+W0.200000/E153.000000/K11/C{239,233,251,4};
[2.1.1]+W0.230000/E126.000000/K1/C{195,73,255,228};
[2.1.1.1]-W0.260000/E1.000000/K4/C{253,166,13,178};
[2.1.2]+W0.120000/E113.000000/K4/C{3,217,22,67};
[2.1.2.1]-W0.850000/E19.000000/K3/C{104,85,119,103};
[2.1.3]+W0.130000/E21.000000/K12/C{51,68,250,241};
[2.1.3.1]-W0.190000/E24.000000/K1/C{114,91,28,225};
[2.2]+W0.710000/E224.000000/K0/C{101,130,34,177};
[2.2.1]-W0.140000/E7.000000/K1/C{10,169,187,177};
[2.3]+W0.600000/E89.000000/K6/C{81,58,148,115};
[2.3.1]-W0.050000/E3.000000/K8/C{26,107,77,88};
[3]+W0.970000/E27.000000/K4/C{163,63,50,180};
[3.1]-W0.240000/E23.000000/K2/C{174,240,5,245};
[4]+W0.200000/E48.000000/K1/C{112,32,111,187};
=
A
P0/I3/H99.000000/E99.000000/D0.250000/C{99,99,25,255}/X92/Y360/S25.000000
B
[1]+W0.320000/E187.000000/K6/C{64,237,88,228};
[1.1]-W0.420000/E36.000000/K4/C{205,233,14,96};
[2]+W0.400000/E5.000000/K11/C{128,238,171,217};
[3]+W0.140000/E8.000000/K13/C{15,73,104,102};
[3.1]-W0.350000/E7.000000/K7/C{22,237,47,206};
[4]+W0.950000/E102.000000/K4/C{246,127,179,217};
[4.1]-W0.690000/E40.000000/K8/C{179,248,143,234};
=

End of example.

When starting data is acquired, the Visualized core loop is enacted.

``` 

```
Multithreaded Environments:

Instanced Sim Manager, with inputed Environment attributes, maps threads for each Environment and calls Not Visualized core loop / manageSimulation().
```

```
Program shutdown:

When chosen simulation finishes or is closed, Environment destructors are called, which save Environment data and call Agent save logging and Brain logging functions.

```

---


### Development Notes

* Most core parts are saved as pointers, except Agent Brain attributes.
* Random number generation is done via global variables in src/data_management/data_types.h with inline std::random_device rd; inline std::mt19937 gen(rd());
* Most #include local dependencies are nested.
---

### Future Developments / Improvements

*  Carbon count and its processess.
*  Plant behavior.
*  Day and night cycle, could affect Plant behavior.
*  Windowed UI.
*  Source code documentation and clean up.
*  Input safety.
*  Each use case Time and Space complexity.

---
### Contact Information

Post issue on https://github.com/PixelAndShip/EEBBASS/issues.<br>
Post on https://github.com/PixelAndShip/EEBBASS/discussions.

---

### License


```
MIT License

Copyright (c) 2026 EEBBASS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
