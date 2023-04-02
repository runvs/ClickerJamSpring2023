//
// Created by Toni on 02/04/2023.
//

#ifndef CLICKERJAMSPRING2023_TROLLFACE_HPP
#define CLICKERJAMSPRING2023_TROLLFACE_HPP

#include <game_object.hpp>
#include <sprite.hpp>
#include <memory>

class TrollFace : public jt::GameObject {
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    std::shared_ptr<jt::Sprite> m_troll_face;
};

#endif // CLICKERJAMSPRING2023_TROLLFACE_HPP
