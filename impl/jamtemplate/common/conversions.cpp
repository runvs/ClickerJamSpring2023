#include "conversions.hpp"

jt::Vector2f jt::Conversion::vec(tson::Vector2f const& v) { return jt::Vector2f { v.x, v.y }; }

jt::Vector2f jt::Conversion::vec(tson::Vector2i const& v)
{
    return jt::Vector2f { static_cast<float>(v.x), static_cast<float>(v.y) };
}
