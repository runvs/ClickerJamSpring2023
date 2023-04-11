
#ifndef CLICKERJAMSPRING2023_MINE_SHAFT_AREA_HPP
#define CLICKERJAMSPRING2023_MINE_SHAFT_AREA_HPP

#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <audio/sound/sound_interface.hpp>
#include <circular_buffer.hpp>
#include <color/color.hpp>
#include <game_object.hpp>
#include <game_properties.hpp>
#include <hud/observer_interface.hpp>
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

    std::shared_ptr<RockLayer> getCurrentLayer() const;
    bool checkIfMouseIsOverArea() const;
    void setDescendHudObservers(
        std::shared_ptr<ObserverInterface<api::API const&>> moneyPerClickObserver,
        std::shared_ptr<ObserverInterface<api::API const&>> depthObserver);

    void progressMining(std::uint64_t value = 1);
    api::API getMoneyPerClick() const;
    void setMoneyPerClick(api::API const& value);

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    std::function<void(api::API const&)> m_onClickCallback;
    MineShaftModel& m_mine_shaft_model;
    std::shared_ptr<jt::Shape> m_background_shape;
    jt::CircularBuffer<std::shared_ptr<RockLayer>, 17> m_rock_layers;
    std::function<void(std::shared_ptr<jt::TweenInterface>)> m_addTweenCallback;

    std::shared_ptr<jt::SoundInterface> m_descentSound;
    api::API m_moneyPerClick;

    std::weak_ptr<ObserverInterface<api::API const&>> m_moneyPerClickObserver;
    std::weak_ptr<ObserverInterface<api::API const&>> m_depthObserver;

    std::shared_ptr<jt::Shape> m_shapeOverlay;

    void handleMouseClicks();
    void cycleLayers();
    void flashCurrentLayer();
    void updateHudObservers() const;

    void descend();

    void drawTooltip() const;
};

#endif // CLICKERJAMSPRING2023_MINE_SHAFT_AREA_HPP
