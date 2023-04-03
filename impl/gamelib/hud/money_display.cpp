#include "money_display.hpp"

MoneyDisplay::MoneyDisplay(
    jt::Text::Sptr text, std::string const& prefix, std::string const& postfix)
    : m_scoreText { text }
    , m_textPrefix { prefix }
    , m_textPostfix { postfix }
{
}

void MoneyDisplay::notify(api::API value)
{
    m_scoreText->setText(m_textPrefix + value.to_exp_string() + m_textPostfix);
}
