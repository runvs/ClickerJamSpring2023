
#ifndef CLICKERJAMSPRING2023_ROCK_TILE_HPP
#define CLICKERJAMSPRING2023_ROCK_TILE_HPP

#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <color/color.hpp>
#include <game_object.hpp>
#include <shape.hpp>
#include <sprite.hpp>
#include <tweens/tween_interface.hpp>
#include <functional>

class RockLayer : public jt::GameObject {
public:
    RockLayer(std::uint64_t hardness, jt::Color const& color, float initialLayerOffset,
        std::function<void(std::shared_ptr<jt::TweenInterface>)> const& addTweenCallback,
        bool isSky = false);

    void progressAmount(std::uint64_t progress);

    bool isMined() const;
    void ascend();
    void flash();

    jt::Rectf getArea() const;

    std::uint64_t getHardness() const;
    std::uint64_t getProgress() const;

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    std::uint64_t m_hardness;
    jt::Color m_color;
    std::uint64_t m_progress;
    float m_layer_offset;

    bool m_isSky;

    std::function<void(std::shared_ptr<jt::TweenInterface>)> m_addTweenCallback;

    std::shared_ptr<jt::Sprite> m_spriteRocks;
    std::shared_ptr<jt::Shape> m_shapeBackground;
    std::shared_ptr<jt::Shape> m_shapeMinedOverlay;

    std::function<void(api::API const&)> m_callback;
};

#endif // CLICKERJAMSPRING2023_ROCK_TILE_HPP
