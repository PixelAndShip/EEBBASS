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

IMPORTANT NOTE!

Behaviors refers to the brain + input node + output node structure.

#### Units

The dynamic variables upon which behavior experimentation is conducted. Contains Agents and Plants.

#### Agent

Subsection of Units, primary variable, on which behaviors are applied and adjusted.

#### Plant

Static source of energy for Agents, provides continuous opportunity for further life time.

#### Brain

Houses root nodes for linked list type Node behavior management.

#### Input node

Saves sensor node variables, from which node functions derive further actions.

#### Output node

Saves output variables from which node functions derive further actions.

#### Node functions 

Provided information from nodes and spider, enacts specified changes on the environments units.

#### Environment

Enacts cyclic simulation, to cultivate agent behaviors and their life cycles.

#### Spider

Manages linking between agents, behaviors and environment.

#### Data types

Globally accessable data types, used in environment customization.

#### Simulation management

Provides multi threading support for multiple environment simulations.

#### Interface

Not yet developed, plans for windowed user interface with the system are being considered.

#### Debug tools

Provides DEBUG_LOG for source code debugging.

---

### How It Works


```

```



---

### Configuration
 <!-- int identifier;
    float radiation;
    int iteration;
    int maxCultivateIteration;
    int maxCycle;
    int carbon_count;
    int maxRootNodes;
    int maxBrainLevel;
    int maxBrainChildNodes;
    bool custom;

    EnvironmentState environmentState = EnvironmentState::Finished; -->
#### Environment variables
| Variable    | Description            
| ----------- | ----------------------- 
| `identifier`| Environment identifer, used for save files, automatically set
| `radiation` |  Chance of behavior mutation, valued between 0.0 and 1.0
| `iteration` | Current cycle iteration, each iteration is considered a moment
| `max cultivation iterations` | Upon iteration reaching this set value, cultivation begins.
| `max cultivation iterations` | Upon iteration reaching this set value, cultivation begins.

---

### File Format

Environment save files are used in .txt format.

---




### Multithreading

If applicable, document:

* Which parts of the program use threads
* How threads are created
* What each thread is responsible for
* How synchronization is handled
* Which objects are shared between threads
* Any thread-safety considerations

---

### User Interface



---

### Saving and Loading

Explain:

* Where save files are stored
* How save files are created
* How environments are loaded
* What information is stored
* What information is not stored
* How an existing save can be viewed or resumed

---

### Debugging / Logging

Explain the project's logging system.

```text
logs/
└── debug.log
```

Describe:

* How logging is enabled
* Where logs are stored
* What information is logged
* Any performance considerations
* Whether logs should be committed to Git

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
