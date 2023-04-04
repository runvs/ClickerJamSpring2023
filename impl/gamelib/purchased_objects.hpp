#ifndef CLICKERJAMSPRING2023_PURCHASED_OBJECTS_HPP
#define CLICKERJAMSPRING2023_PURCHASED_OBJECTS_HPP

#include "animation.hpp"
#include "bank_interface.hpp"
#include "purchased_object.hpp"
#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <game_object.hpp>
#include <purchase_info.hpp>
#include <vector>

class PurchasedObjects : public jt::GameObject {
public:
    PurchasedObjects(BankInterface& bank, std::vector<PurchaseInfo> const& infos);
    void addObject(std::string const& name);

    api::API getInputPerSecond() const;

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    int m_numberOfMiners { 0 };

    std::vector<float> m_minerIncomeTimers;
    float m_minerIncomeTimerMax = 1.5f;
    BankInterface& m_bank;
    std::vector<PurchaseInfo> m_infos;
    std::map<std::string, std::shared_ptr<PurchasedObject>> m_purchasedObjects;

    api::API m_inputPerSecondCached;
};

#endif // CLICKERJAMSPRING2023_PURCHASED_OBJECTS_HPP
