
#ifndef CLICKERJAMSPRING2023_MINE_SHAFT_MODEL_HPP
#define CLICKERJAMSPRING2023_MINE_SHAFT_MODEL_HPP

#include "arbitrary_precision_int/arbitrary_precision_int.hpp"
class MineShaftModel {
public:
    MineShaftModel();

    api::API getCurrentDepth() const;
    void setCurrentDepth(api::API const& depth);
    std::uint64_t getNumberOfMinedLayers();
    void addMinedLayer(int amount = 1);
    void descend();

private:
    api::API m_currentDepth;
    std::uint64_t m_numberOfLayersMined;
};

#endif // CLICKERJAMSPRING2023_MINE_SHAFT_MODEL_HPP
