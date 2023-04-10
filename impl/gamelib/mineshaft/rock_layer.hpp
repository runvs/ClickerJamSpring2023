
#ifndef CLICKERJAMSPRING2023_ROCK_TILE_HPP
#define CLICKERJAMSPRING2023_ROCK_TILE_HPP

#include "shape.hpp"
#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <color/color.hpp>
#include <game_object.hpp>
#include <sprite.hpp>

class RockLayer : public jt::GameObject {
public:
    RockLayer(int hardness, jt::Color const& color, float initialLayerOffset, bool isSky = false);

    void progressAmount(int progress);

    bool isMined() const;
    void ascend();
    void flash();

    jt::Rectf getArea() const;

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    int m_hardness;
    jt::Color m_color;
    int m_progress;
    float m_layer_offset;

    bool m_isSky;

    std::shared_ptr<jt::Sprite> m_spriteRocks;
    std::shared_ptr<jt::Shape> m_shapeBackground;
    std::shared_ptr<jt::Shape> m_shapeMiddle;

    std::function<void(api::API const&)> m_callback;

    // TODO: replace color with sprite
    // mutable std::shared_ptr<jt::Sprite> m_sprite;
};

#endif // CLICKERJAMSPRING2023_ROCK_TILE_HPP
