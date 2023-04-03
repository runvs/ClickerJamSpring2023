#ifndef CLICKERJAMSPRING2023_PURCHASE_INFO_HPP
#define CLICKERJAMSPRING2023_PURCHASE_INFO_HPP

#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <string>

struct PurchaseInfo {
    std::string name;
    api::API initialCost;
    std::string animation_file;
    std::string animation_name;
};

#endif // CLICKERJAMSPRING2023_PURCHASE_INFO_HPP
