
#include "mine_shaft_model.hpp"
#include <game_properties.hpp>
#include <iostream>

MineShaftModel::MineShaftModel()
{
    m_currentDepth = api::from_uint64(0u);
    m_numberOfLayersMined = 0;
}

api::API MineShaftModel::getCurrentDepth() const { return m_currentDepth; }

void MineShaftModel::descend()
{
    m_currentDepth = m_currentDepth + api::from_uint64(GP::MineShaftDepthPerLayer());
    // TODO: if player reaches MAX_INT win game? Alternatively just wrap around to 0 or something...
    if (m_numberOfLayersMined != std::numeric_limits<std::uint64_t>::max()) {
        m_numberOfLayersMined++;
    }
}

std::uint64_t MineShaftModel::getNumberOfMinedLayers() { return m_numberOfLayersMined; }

void MineShaftModel::addMinedLayer(int amount) { m_numberOfLayersMined += amount; }

void MineShaftModel::setCurrentDepth(const api::API& depth) { m_currentDepth = depth; }
