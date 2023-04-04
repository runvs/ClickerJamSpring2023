
#ifndef CLICKERJAMSPRING2023_ROCK_TILE_HPP
#define CLICKERJAMSPRING2023_ROCK_TILE_HPP

#include "shape.hpp"
#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <color/color.hpp>
#include <game_object.hpp>
#include <sprite.hpp>

class RockLayer : public jt::GameObject {
public:
    RockLayer(const int hardness, const jt::Color color, const float initial_layer_offset = 0.0f);

    void progressAmount(int progress);

    bool isMined();
    void ascend();

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    int m_hardness;
    int m_progress;
    float m_layer_offset;
    std::shared_ptr<jt::Shape> m_shape;

    std::function<void(api::API const&)> m_callback;

    // TODO: replace color with sprite
    jt::Color m_color;
    // mutable std::shared_ptr<jt::Sprite> m_sprite;
};

#endif // CLICKERJAMSPRING2023_ROCK_TILE_HPP
