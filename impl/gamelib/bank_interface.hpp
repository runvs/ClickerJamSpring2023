#ifndef CLICKERJAMSPRING2023_BANK_INTERFACE_HPP
#define CLICKERJAMSPRING2023_BANK_INTERFACE_HPP

#include <arbitrary_precision_int/arbitrary_precision_int.hpp>

class BankInterface {
public:
    virtual ~BankInterface() = default;

    virtual api::API getCurrentMoney() = 0;
    virtual bool canAffordAmount(api::API const& amount) = 0;
    virtual void spendMoney(api::API const& amount) = 0;
};

#endif // CLICKERJAMSPRING2023_BANK_INTERFACE_HPP
