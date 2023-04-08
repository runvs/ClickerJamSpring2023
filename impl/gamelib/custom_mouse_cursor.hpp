#ifndef CLICKERJAMSPRING2023_CUSTOM_MOUSE_CURSOR_HPP
#define CLICKERJAMSPRING2023_CUSTOM_MOUSE_CURSOR_HPP

#include <animation.hpp>
#include <game_object.hpp>

class CustomMouseCursor : public jt::GameObject {
public:
    void playAnimation(std::string const& animName);
private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    std::shared_ptr<jt::Animation> m_cursorAnimation;
};

#endif // CLICKERJAMSPRING2023_CUSTOM_MOUSE_CURSOR_HPP
