#include <unordered_map>
#include <string>


const std::unordered_map<int,std::string> Senses = {
    {0,"SeeSomething"},
    {1,"SeeColor"},
    {2,"SeeInRangeSomething"},
    {3,"SeeInRangeColor"}
};

const std::unordered_map<int,std::string> Actions = {
    {0,"MoveLeft"},
    {1,"MoveRight"},
    {2,"MoveUp"},
    {3,"MoveDown"},
    {4,"SpinLeft"},
    {5,"SpinRight"},
    {6,"Bite"},
    
};