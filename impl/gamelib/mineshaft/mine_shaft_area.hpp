
#ifndef CLICKERJAMSPRING2023_MINE_SHAFT_AREA_HPP
#define CLICKERJAMSPRING2023_MINE_SHAFT_AREA_HPP

#include "arbitrary_precision_int/arbitrary_precision_int.hpp"
#include "rock_layer.hpp"
#include "shape.hpp"
#include <circular_buffer.hpp>
#include <color/color.hpp>
#include <game_object.hpp>
#include <game_properties.hpp>
#include <sprite.hpp>

class MineShaftArea : public jt::GameObject {
public:
    MineShaftArea(std::function<void(api::API const&)> callback);

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    std::function<void(api::API const&)> m_callback;
    std::shared_ptr<jt::Shape> m_background_shape;
    mutable jt::CircularBuffer<std::shared_ptr<RockLayer>, 17> m_rock_layers;
};

#endif // CLICKERJAMSPRING2023_MINE_SHAFT_AREA_HPP
