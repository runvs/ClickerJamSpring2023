#ifndef CLICKERJAMSPRING2023_PURCHASED_OBJECT_HPP
#define CLICKERJAMSPRING2023_PURCHASED_OBJECT_HPP

#include <animation.hpp>
#include <bank_interface.hpp>
#include <game_object.hpp>
#include <purchase_info.hpp>
#include <text.hpp>
#include <tweens/tween_interface.hpp>
#include <vector>

class PurchasedObject : public jt::GameObject {
public:
    PurchasedObject(BankInterface& bank, PurchaseInfo const& info,
        std::function<void(std::shared_ptr<jt::TweenInterface>)> const& addTweenCallback);

    void buyOne();

    api::API getInputPerSecond() const;
    int getNumberOfBoughtObjects() const;
    void clean();

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
    void drawTooltip() const;

    BankInterface& m_bank;
    PurchaseInfo m_info;
    std::function<void(std::shared_ptr<jt::TweenInterface>)> m_addTweenCallback;

    int m_numberOfObjects { 0 };
    std::vector<float> m_timers {};

    float m_animRestartTimer { 0.0f };
    float m_animRestartTimerMax { 0.0f };

    mutable std::vector<std::shared_ptr<jt::Animation>> m_animations;
    std::shared_ptr<jt::Text> m_text;

    jt::Vector2f m_baseOffset;
    jt::Rectf m_rect;
    api::API m_incomePerSecond;
    void addNewAnimation();
};

#endif // CLICKERJAMSPRING2023_PURCHASED_OBJECT_HPP
