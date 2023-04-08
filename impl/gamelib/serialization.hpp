#ifndef CLICKERJAMSPRING2023_SERIALIZATION_HPP
#define CLICKERJAMSPRING2023_SERIALIZATION_HPP

#include <bank.hpp>
#include <nlohmann.hpp>
#include <purchased_objects.hpp>

namespace api {
void to_json(nlohmann::json& j, const api::API& n);
void from_json(const nlohmann::json& j, api::API& b);
} // namespace api

void to_json(nlohmann::json& j, const Bank& b);
void from_json(const nlohmann::json& j, Bank& b);

void to_json(nlohmann::json& j, const PurchasedObjects& b);

#endif // CLICKERJAMSPRING2023_SERIALIZATION_HPP
