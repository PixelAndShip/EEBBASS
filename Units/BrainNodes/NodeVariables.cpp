#include <unordered_map>
#include <string>


const std::unordered_map<int,std::string> Senses = {
    {0,"See"},
    {1,"EnergyCount"},
    {2,"SeeColor"}
};

const std::unordered_map<int,std::string> Actions = {
    {0,"MoveLeft"},
    {1,"MoveRight"},
    {2,"MoveUp"},
    {3,"MoveDown"}
};