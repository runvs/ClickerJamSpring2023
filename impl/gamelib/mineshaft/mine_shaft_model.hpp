
#ifndef CLICKERJAMSPRING2023_MINE_SHAFT_MODEL_HPP
#define CLICKERJAMSPRING2023_MINE_SHAFT_MODEL_HPP

#include "arbitrary_precision_int/arbitrary_precision_int.hpp"
class MineShaftModel {
public:
    MineShaftModel();

    api::API getCurrentDepth();
    int getNumberOfMinedLayers();
    void descend();

private:
    api::API m_current_depth;
    int m_number_of_layers_mined;
};

#endif // CLICKERJAMSPRING2023_MINE_SHAFT_MODEL_HPP
