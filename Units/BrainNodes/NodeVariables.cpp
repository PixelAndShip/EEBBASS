#include <unordered_map>
#include <string>


const std::unordered_map<int,std::string> Senses = {
    {0,"SeeSomething"},
    {1,"SeeColor"},
    {2,"SeeInRangeSomething"},
    {3,"SeeInRangeColor"},
    {4,"EnergyCountAboveSet"},
    {5,"EnergyCountBelowSet"},
    {6,"HealthCountAboveSet"},
    {7,"HealthCountBelowSet"},
    {8,"cHealtAboveSet"},
    {9,"cHealthBelowSet"},
    {10,"cSpeedAboveSet"},
    {11,"cSpeedBelowSet"}
};

const std::unordered_map<int,std::string> Actions = {
    {0,"MoveLeft"},
    {1,"MoveRight"},
    {2,"MoveUp"},
    {3,"MoveDown"},
    {4,"SpinLeft"},
    {5,"SpinRight"},
    {6,"Bite"},
    {7,"Split"},
    {8,"ChangeCHealth"},
    {9,"ChangeCSpeed"},
    {10,"ExpendEnergyOnHealth"},
    {11,"ExpendEnergyOnSpeed"}
      
};