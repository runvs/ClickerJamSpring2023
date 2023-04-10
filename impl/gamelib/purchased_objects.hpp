#ifndef CLICKERJAMSPRING2023_PURCHASED_OBJECTS_HPP
#define CLICKERJAMSPRING2023_PURCHASED_OBJECTS_HPP

#include "animation.hpp"
#include "bank_interface.hpp"
#include "purchased_object.hpp"
#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <game_object.hpp>
#include <purchase_info.hpp>
#include <tweens/tween_interface.hpp>
#include <vector>

class PurchasedObjects : public jt::GameObject {
public:
    PurchasedObjects(BankInterface& bank, std::vector<PurchaseInfo> const& infos,
        std::function<void(std::shared_ptr<jt::TweenInterface>)> const& addTweenCallback);
    void addObject(std::string const& name);

    api::API getInputPerSecond() const;

    // Note: Just for serialization/deserialization
    std::map<std::string, std::shared_ptr<PurchasedObject>> getObjects() const;

    void clean();

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    BankInterface& m_bank;
    std::vector<PurchaseInfo> m_infos;
    std::function<void(std::shared_ptr<jt::TweenInterface>)> m_addTweenCallback;

    std::map<std::string, std::shared_ptr<PurchasedObject>> m_purchasedObjects;

    api::API m_inputPerSecondCached;
};

#endif // CLICKERJAMSPRING2023_PURCHASED_OBJECTS_HPP
