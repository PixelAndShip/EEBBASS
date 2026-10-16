#pragma once
#include <vector>
#include "output_node.h"
#include <algorithm>
class InputNode
{
private:
    float weight = 1;
    float setAmount = 0;
    unsigned int key = 255;
    UnitColor unitColor = {255, 255, 255, 255};
    std::vector<InputNode *> inputNodes = {};
    OutputNode *outputNode = nullptr;

public:
    InputNode();
    InputNode(float iW, float setA, unsigned int iK, UnitColor iU);
    InputNode(InputNode *copyNode);
    ~InputNode();
    void setWeight(float iW);
    void setSetAmount(float iSA);
    void setUnitColor(UnitColor iC);
    void setKey(unsigned int iK);
    void appendInputNode(InputNode *iN);
    void insertInputNodeAt(int index, InputNode *iN);
    void deleteInputNode(int index);
    void setOutputNode(OutputNode *iON);
    float getWeight();
    float getSetAmount();
    UnitColor getUnitColor();
    unsigned int getKey();
    const std::vector<InputNode *> &getInputNodes() const;
    OutputNode *getOutputNode();
};
