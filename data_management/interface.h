#pragma once
#include "sim_manager.h"
class Interface
{

public:
    SimManager *simulation;

    void DisplayHomeScreen();

    void DisplayEnvironments();

    void DisplayActiveScreen(); // prob input case char
};

/*
    1. Home screen:
    _________________________________________________________________________________________________________________________
    | List of environments:       |   Active screen (selected environment details, opened text file, visual environment)     |
    |                             |                                                                                          |
    |_____________________________|__________________________________________________________________________________________|

*/