#pragma once
#include "sim_manager.h"
#include <raylib.h>
class Interface
{

public:
    SimManager *simulation;

    void DisplayHomeScreen();

    void DisplayEnvironments(); // sidebar of selectable environments and their status (1. in multithread simulation 2. viewable)

    void DisplayActiveScreen(); // prob input case char, to check which type to display on main (save file, or draw simulation)

    void DisplaySimulation();

    void DisplayEnvironmentStats();

    void DisplaySaveFile(); // add view, edit and save functionality to ES/ directory .txt files
};

/*
    1. Home screen:
    _________________________________________________________________________________________________________________________
    | List of environments:       |   Active screen (selected environment details, opened text file, visual environment)     |
    |                             |                                                                                          |
    |_____________________________|__________________________________________________________________________________________|

*/