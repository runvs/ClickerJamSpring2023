#include "purchased_objects.hpp"
#include <game_properties.hpp>

PurchasedObjects::PurchasedObjects(BankInterface& bank, std::vector<PurchaseInfo> const& infos)
    : m_bank { bank }
    , m_infos { infos }
{
}

void PurchasedObjects::doCreate()
{
    for (auto const& i : m_infos) {
        m_purchasedObjects[i.name] = std::make_shared<PurchasedObject>(m_bank, i);
        m_purchasedObjects[i.name]->setGameInstance(getGame());
        m_purchasedObjects[i.name]->create();
    }
}

void PurchasedObjects::doUpdate(float const elapsed)
{
    for (auto& po : m_purchasedObjects) {
        po.second->update(elapsed);
    }
}

void PurchasedObjects::doDraw() const
{
    for (auto const& po : m_purchasedObjects) {
        po.second->draw();
    }
}

void PurchasedObjects::addObject(std::string const& name)
{
    if (m_purchasedObjects.count(name) != 1) {
        throw std::invalid_argument { "Invalid object in PurchasedObjects" };
    }
    m_purchasedObjects[name]->buyOne();

    m_inputPerSecondCached = api::API {};
    for (auto const& po : m_purchasedObjects) {
        m_inputPerSecondCached = m_inputPerSecondCached + po.second->getInputPerSecond();
    }
}
api::API PurchasedObjects::getInputPerSecond() const { return m_inputPerSecondCached; }
std::map<std::string, std::shared_ptr<PurchasedObject>> PurchasedObjects::getObjects() const
{
    return m_purchasedObjects;
}
void PurchasedObjects::clean()
{
    for (auto& po : m_purchasedObjects) {
        po.second->clean();
    }
}
