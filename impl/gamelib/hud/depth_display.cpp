#include "depth_display.hpp"

DepthDisplay::DepthDisplay(
    jt::Text::Sptr text, std::string const& prefix, std::string const& postfix)
    : m_scoreText { text }
    , m_textPrefix { prefix }
    , m_textPostfix { postfix }
{
}

void DepthDisplay::notify(api::API value)
{
    m_scoreText->setText(m_textPrefix + value.to_exp_string() + m_textPostfix);
}
