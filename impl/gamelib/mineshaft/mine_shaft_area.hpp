
#ifndef CLICKERJAMSPRING2023_MINE_SHAFT_AREA_HPP
#define CLICKERJAMSPRING2023_MINE_SHAFT_AREA_HPP

#include "arbitrary_precision_int/arbitrary_precision_int.hpp"
#include "shape.hpp"
#include <circular_buffer.hpp>
#include <color/color.hpp>
#include <game_object.hpp>
#include <sprite.hpp>

class MineShaftArea : public jt::GameObject {
public:
    MineShaftArea(std::function<void(api::API const&)> callback);

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    std::function<void(api::API const&)> m_callback;
    std::shared_ptr<jt::Shape> m_shape;
};

#endif // CLICKERJAMSPRING2023_MINE_SHAFT_AREA_HPP
