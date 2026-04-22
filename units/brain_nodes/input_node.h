#pragma once
#include <vector>
#include <memory>
#include "output_node.h"

#include <any>

class InputNode
{
private:
    float weight;
    char unit;
    float setAmount;
    UnitColor unitColor;
    unsigned int key = 255;
    std::vector<InputNode *> inputNodes;
    OutputNode *outputNode;

public:
    ~InputNode();
    void setWeight(float iW);
    void setUnit(char iU);
    void setSetAmount(float iSA);
    void setUnitColor(UnitColor iC);
    void setKey(unsigned int iK);
    void appendInputNode(InputNode *iN);
    void insertInputNodeAt(int index, InputNode *iN);
    void deleteInputNode(int index);
    void setOutputNode(OutputNode *iON);

    float getWeight();
    char getUnit();
    float getSetAmount();
    UnitColor getUnitColor();
    unsigned int getKey();
    const std::vector<InputNode *> &getInputNodes() const;
    OutputNode *getOutputNode();
};
