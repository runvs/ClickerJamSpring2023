
#ifndef CLICKERJAMSPRING2023_MINE_SHAFT_AREA_HPP
#define CLICKERJAMSPRING2023_MINE_SHAFT_AREA_HPP

#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <audio/sound/sound_interface.hpp>
#include <circular_buffer.hpp>
#include <color/color.hpp>
#include <game_object.hpp>
#include <game_properties.hpp>
#include <mineshaft/mine_shaft_model.hpp>
#include <mineshaft/rock_layer.hpp>
#include <shape.hpp>
#include <sprite.hpp>
#include <tweens/tween_interface.hpp>
#include <functional>

class MineShaftArea : public jt::GameObject {
public:
    MineShaftArea(MineShaftModel& model, std::function<void(api::API const&)> callback,
        std::function<void(std::shared_ptr<jt::TweenInterface>)> const& addTweenCallback);

    std::shared_ptr<RockLayer> getActiveLayer();

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    std::function<void(api::API const&)> m_callback;
    MineShaftModel& m_mine_shaft_model;
    std::shared_ptr<jt::Shape> m_background_shape;
    jt::CircularBuffer<std::shared_ptr<RockLayer>, 17> m_rock_layers;
    std::function<void(std::shared_ptr<jt::TweenInterface>)> m_addTweenCallback;

    std::shared_ptr<jt::SoundInterface> m_descentSound;
    void handleMouseClicks();
    void cycleLayers();
    void descend();
    void flashActiveLayer();
};

#endif // CLICKERJAMSPRING2023_MINE_SHAFT_AREA_HPP
