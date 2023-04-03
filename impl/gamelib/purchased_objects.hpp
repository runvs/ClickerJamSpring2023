#ifndef CLICKERJAMSPRING2023_PURCHASED_OBJECTS_HPP
#define CLICKERJAMSPRING2023_PURCHASED_OBJECTS_HPP

#include "animation.hpp"
#include "bank_interface.hpp"
#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <game_object.hpp>

class PurchasedObjects : public jt::GameObject {
public:
    explicit PurchasedObjects(BankInterface& bank);
    void addMiner();

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    mutable std::shared_ptr<jt::Animation> m_miner;

    int m_numberOfMiners { 0 };
    void drawMiners() const;

    std::vector<float> m_minerIncomeTimers;
    float m_minerIncomeTimerMax = 1.5f;
    BankInterface& m_bank;
};

#endif // CLICKERJAMSPRING2023_PURCHASED_OBJECTS_HPP
