//
#ifndef CLICKERJAMSPRING2023_DEPTH_DISPLAY_HPP
#define CLICKERJAMSPRING2023_DEPTH_DISPLAY_HPP

#include <arbitrary_precision_int/arbitrary_precision_int.hpp>
#include <hud/observer_interface.hpp>
#include <text.hpp>

class DepthDisplay : public ObserverInterface<api::API> {
public:
    explicit DepthDisplay(
        jt::Text::Sptr text, std::string const& prefix = "", std::string const& postfix = "");
    void notify(api::API value) override;

private:
    jt::Text::Sptr const m_scoreText;
    std::string m_textPrefix;
    std::string m_textPostfix;
};

#endif // CLICKERJAMSPRING2023_DEPTH_DISPLAY_HPP
