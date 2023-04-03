#include "bank.hpp"
api::API Bank::getCurrentMoney() { return m_money; }
void Bank::spendMoney(api::API const& amount) { m_money = m_money - amount; }
bool Bank::canAffordAmount(api::API const& amount) { return api::compare(m_money, amount) >= 0; }
