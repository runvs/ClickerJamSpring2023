
#include "serialization.hpp"
void api::to_json(nlohmann::json& j, api::API const& n)
{
    j = nlohmann::json { { "value", n.get_data() } };
}
void api::from_json(nlohmann::json const& j, api::API& b)
{
    // load money
    std::vector<std::uint8_t> data;
    j.at("value").get_to(data);
    b = api::API { data };
}
void to_json(nlohmann::json& j, Bank const& b)
{
    j = nlohmann::json { { "money", b.getCurrentMoney() } };
}
void from_json(nlohmann::json const& j, Bank& b)
{
    // reset money to 0
    auto const current = b.getCurrentMoney();
    b.spendMoney(current);

    // load money
    api::API money;
    j.at("money").get_to(money);
    b.receiveMoney(money);
}

void to_json(nlohmann::json& j, PurchaseInfo const& i)
{
    j = nlohmann::json {
        // clang-format off
        { "index", i.index },
        { "name", i.name },
        { "timerMax", i.timerMax },
        { "objectsPerLine", i.objectsPerLine },
        { "initialCost", i.initialCost },
        { "animationFile", i.animationFile },
        { "animationMenu", i.animationNameMenu },
        { "animationPurchased", i.animationNamePurchased },
        { "income", i.income }
        // clang-format on
    };
}
void from_json(nlohmann::json const& j, PurchaseInfo& i)
{
    j.at("index").get_to(i.index);
    j.at("name").get_to(i.name);
    j.at("timerMax").get_to(i.timerMax);
    j.at("objectsPerLine").get_to(i.objectsPerLine);
    j.at("initialCost").get_to(i.initialCost);
    j.at("animationFile").get_to(i.animationFile);
    j.at("animationMenu").get_to(i.animationNameMenu);
    j.at("animationPurchased").get_to(i.animationNamePurchased);
    j.at("income").get_to(i.income);
}

void to_json(nlohmann::json& j, PurchasedObjects const& pos)
{
    auto const objects = pos.getObjects();
    for (auto const& kvp : objects) {
        j[kvp.first] = kvp.second->getNumberOfBoughtObjects();
    }
}
