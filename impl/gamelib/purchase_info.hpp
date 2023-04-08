#ifndef CLICKERJAMSPRING2023_PURCHASE_INFO_HPP
#define CLICKERJAMSPRING2023_PURCHASE_INFO_HPP

#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <input/keyboard/keyboard_defines.hpp>
#include <functional>
#include <string>

struct PurchaseInfo {
    std::string name { "" };
    api::API initialCost { api::from_uint64(0u) };
    std::string animationFile { "" };
    std::string animationNameMenu { "" };
    std::string animationNamePurchased { "" };
    std::function<void(api::API const& cost)> purchaseCallback { nullptr };
    int objectsPerLine { 25 };
    float timerMax { 1.5f };
    api::API income { api::from_uint64(1u) };
    jt::KeyCode keyCode { jt::KeyCode::Num1 };

    // arguments that can be filled automatically
    int index { 0 };
};

#endif // CLICKERJAMSPRING2023_PURCHASE_INFO_HPP
