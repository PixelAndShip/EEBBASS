# EEBASS (Environmentally engineered behavior based agent simulation system)

## 1. Short Description




> This is an open source solo project, in which different behaviors are engineered for different environments via evolution based on natural selection. The program provides several modes of operation and customization of agents, environments and visualization of the simulation. <br>
> The main goals for the project are to: <br>
>1.Gain further knowledge and understanding of C++ systems. <br>
>2.Explore individual unit evolution systems.
>3.Gain further knowledge and understanding of Git systems.
---

## 2. Important Notes

### Requirements

Provided via using ldd (List Dynamic Dependencies):

* linux-vdso.so.1 
* libstdc++.so.6 
* libm.so.6
* libgcc_s.so.1
* libc.so.6

### Before Running

* Make sure your current directory supports directory creation and file creation. This is needed to save and use save files for environment loading.
* This application uses raylib to visualize simulations, raylib may need to be installed as a library to recompil the source code.
* Multithreading option may clog up the cpu cores.
* To use custom environment option, make sure to have your usable save file in the current working directory, otherwise the current implementation will not be able to read it.
* main_linux is compiled using g++.

### Known Issues

* Uknown/unobtainable custom save file entry causes a core dump (crash).


---

## 3. In-Depth Documentation

### Project Structure

```
Project/
├── dbgtools/
│   ├── debug.cpp
│   ├── debug.h
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
├── main
└── README.md
```

IMPORTANT NOTE!

File directory EEBASS is created upon activating the main executable, in which subdirectories *environments* and *logs* are also created. *environments* houses environment save files, logs contain DEBUG_LOG() outputs.



---

### Core Components


#### Input Node

Saves sensory input configuration. Hosts child Input Node pointers and Output Node pointer. Attributes:
<br>
* float weight - chance of activating, set between 0.0 and 1.0
<br>
* float setAmount - value from 0 to 255, used in Agent variable input.
<br>
* unsigned int key - corresponding src/data_management/data_types.h Senses from getSenses input key, which is later used to verify Node Function.
<br>
* UnitColor unitColor - used to identify target, check UnitColor struct in src/data_management/data_types.h for more information.
<br>
* std::vector<InputNode *> inputNodes - child Input Node pointers, used to form tree structure. Length is capped to settable max Child Nodes.
<br>
* OutputNode *outputNode - leaf Output Node, used in Agent variable output.

#### Output Node 

Saves Agent's output configuration. Attributes:
<br>
* float weight - chance of activating, set between 0.0 and 1.0
<br>
* float energyCost - value from 0 to 255, used in Agent variable output.
<br>
* unsigned int key - corresponding src/data_management/data_types.h Actions from getActions input key, which is later used to verify Node Function.
<br>
* UnitColor unitColor - used to identify target, check UnitColor struct in src/data_management/data_types.h for more information.

#### Node Functions

Enacts change on the hosting Agent or target Agent / Plant, based on the Input and Output Node configurations.

#### Brain 

Hosts root Input Node pointers. The brain structure is a tree with leaf Output Node pointers, settable max child Input Nodes and max tree depth.

#### Agent

Main unit of the environment, hosts the Brain. Attributes:
<br>
* bool processed - check for moment processing, makes sure agent can only enact one Output Node - Node Function.
<br>
* int env_identifier - environment that the Agent enhabits.
<br>
* float health - current health.
<br>
* float energy - current energy.
<br>
* float plantDiet - coefficient used for biting functions in Node Functions, set from 0.0 to 1.0.
<br>
* UnitColor agentColor - current UnitColor.
<br>
* int x, y - current position in Environment.
<br>
* float speed - current speed, used to sort actions in a moment between Agents.
<br>
* Brain brain - current brain.

#### Plant

Source of energy for Agents. Attributes:
<br>
* int env_identifier - environment that the Plant enhabits.
<br>
* float health - current health.
<br>
* UnitColor plantColor - current UnitColor.
<br>
* int x, y - current position in Environment.

#### Environment




#### Debug tools

Provides DEBUG_LOG for source code debugging.

---

### How It Works


```
Save File Environment:
Default Visual Environment:
Terminal configured Visual Environment:
Multithreaded Environments:
```



---




### Development Notes

Document implementation decisions, design choices, and things developers should be aware of when modifying the project.

Examples:

* Why a particular data structure was chosen
* Why certain classes own certain objects
* Threading decisions
* Random number generation
* Memory ownership
* Important dependencies between classes

---

### Future Improvements

* 


---

### Credits / References



---

### License


```
MIT License

Copyright (c) 2026 EEBASS

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
