#ifndef CLICKERJAMSPRING2023_PURCHASE_INFO_HPP
#define CLICKERJAMSPRING2023_PURCHASE_INFO_HPP

#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <functional>
#include <string>

struct PurchaseInfo {
    std::string name { "" };
    api::API initialCost { api::from_uint64(0u) };
    std::string animation_file { "" };
    std::string animation_name { "" };
    std::function<void(api::API const& cost)> purchaseCallback { nullptr };

    // arguments that can be filled automatically
    int index { 0 };
};

#endif // CLICKERJAMSPRING2023_PURCHASE_INFO_HPP
