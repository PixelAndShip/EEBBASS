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

### Known Issues

* Uknown/unobtainable custom save file entry causes a core dump (crash).

### Important Behaviour

* Explain anything that may not be immediately obvious.
* Mention file locations, save behaviour, threading, performance considerations, etc.
* Mention anything that could cause unexpected behaviour if modified.

---

## 3. In-Depth Documentation

### Project Structure

```
Project/
├── src/
│   ├── main.cpp
│   ├── ...
│
├── include/
│   ├── ...
│
├── saves/
│   ├── ...
│
├── logs/
│   ├── ...
│
└── README.md
```

Describe the purpose of the important directories and files.

---

### Core Components

#### Component 1

Explain what the component does, what its responsibilities are, and how it interacts with other components.

#### Component 2

Explain its purpose and important implementation details.

#### Component 3

Explain its purpose and important implementation details.

---

### How It Works

Describe the overall program flow.

For example:

```text
Program starts
      ↓
Initialize simulation
      ↓
Create environments
      ↓
Create agents
      ↓
Start simulation
      ↓
Agents perform actions
      ↓
Environment updates
      ↓
Save / observe environment
      ↓
Next simulation cycle
```

Explain each stage in more detail below.

---

### Configuration

Document important configuration variables and what they control.

| Variable    | Description             | Example |
| ----------- | ----------------------- | ------- |
| `variable1` | Description of variable | `100`   |
| `variable2` | Description of variable | `0.5`   |
| `variable3` | Description of variable | `true`  |

---

### File Format

If the project uses custom save/configuration files, document their format here.

```text
Example file format:

...
...
...
```

Explain each section and value.

---

### Simulation / Program Logic

Describe the important algorithms and logic used by the project.

#### Step 1 — Initialization

Explain what happens when the program starts.

#### Step 2 — Processing

Explain how the main processing/simulation loop works.

#### Step 3 — Updates

Explain how objects, environments, agents, or other components are updated.

#### Step 4 — Saving / Loading

Explain how data is saved and loaded.

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

Document the available screens, controls, buttons, keyboard inputs, and their behaviour.

| Input   | Action |
| ------- | ------ |
| `W`     | Action |
| `S`     | Action |
| `Enter` | Action |
| `Esc`   | Action |

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

* [ ] Improvement 1
* [ ] Improvement 2
* [ ] Improvement 3
* [ ] Improvement 4

---

### Credits / References

List external libraries, resources, algorithms, tutorials, or other projects used.

* Library / Resource 1
* Library / Resource 2
* Reference 3

---

### License

Specify the project's license here.

Example:

```text
This project is licensed under the MIT License.
```
