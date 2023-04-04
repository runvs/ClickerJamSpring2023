#include "bank.hpp"

api::API Bank::getCurrentMoney() const { return m_money; }

void Bank::spendMoney(api::API const& amount)
{
    // TODO add visual effects
    m_money = m_money - amount;
}
bool Bank::canAffordAmount(api::API const& amount) { return api::compare(m_money, amount) >= 0; }
void Bank::receiveMoney(api::API const& amount)
{
    // TODO add visual effects
    m_money = m_money + amount;
}
