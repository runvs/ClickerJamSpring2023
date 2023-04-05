
#include "mine_shaft_model.hpp"
MineShaftModel::MineShaftModel()
{
    m_current_depth = api::from_uint64(0u);
    m_number_of_layers_mined = 0;
}

api::API MineShaftModel::getCurrentDepth() { return m_current_depth; }

void MineShaftModel::descent()
{
    m_current_depth = m_current_depth + api::from_uint64(17u) * api::from_uint64(1u);
    // TODO: if player reaches MAX_INT win game? Alternatively just wrap around to 0 or something...
    m_number_of_layers_mined++;
}
int MineShaftModel::getNumberOfMinedLayers() { return m_number_of_layers_mined; }
