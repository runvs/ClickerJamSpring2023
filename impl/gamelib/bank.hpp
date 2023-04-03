#ifndef CLICKERJAMSPRING2023_BANK_HPP
#define CLICKERJAMSPRING2023_BANK_HPP

#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <bank_interface.hpp>
class Bank : public BankInterface {
public:
    api::API getCurrentMoney() override;
    bool canAffordAmount(api::API const& amount) override;

    void spendMoney(api::API const& amount) override;
    void receiveMoney(api::API const& amount) override;

private:
    api::API m_money;
};

#endif // CLICKERJAMSPRING2023_BANK_HPP
