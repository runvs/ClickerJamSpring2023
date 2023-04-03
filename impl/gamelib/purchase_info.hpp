#ifndef CLICKERJAMSPRING2023_PURCHASE_INFO_HPP
#define CLICKERJAMSPRING2023_PURCHASE_INFO_HPP

#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <functional>
#include <string>

struct PurchaseInfo {
    std::string name { "" };
    api::API initialCost { api::from_uint64(0u) };
    std::string animation_file { "" };
    std::string animation_name_menu { "" };
    std::string animation_name_purchased { "" };
    std::function<void(api::API const& cost)> purchaseCallback { nullptr };
    int objectsPerLine { 25 };
    float timerMax { 1.5f };
    api::API income { api::from_uint64(1u) };

    // arguments that can be filled automatically
    int index { 0 };
};

#endif // CLICKERJAMSPRING2023_PURCHASE_INFO_HPP
