#ifndef CLICKERJAMSPRING2023_PURCHASED_OBJECT_HPP
#define CLICKERJAMSPRING2023_PURCHASED_OBJECT_HPP

#include <animation.hpp>
#include <bank_interface.hpp>
#include <game_object.hpp>
#include <purchase_info.hpp>
#include <vector>

class PurchasedObject : public jt::GameObject {
public:
    PurchasedObject(BankInterface& bank, PurchaseInfo const& info);

    void buyOne();

    api::API getInputPerMinute() const;

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    BankInterface& m_bank;
    PurchaseInfo m_info;
    std::vector<float> m_timers;
    int m_numberOfObjects;

    mutable std::shared_ptr<jt::Animation> m_animation;
};

#endif // CLICKERJAMSPRING2023_PURCHASED_OBJECT_HPP
