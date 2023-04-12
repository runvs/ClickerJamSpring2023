#ifndef JAMTEMPLATE_CONVERSIONS_HPP
#define JAMTEMPLATE_CONVERSIONS_HPP

#include <vector.hpp>
#include <tileson.h>

namespace jt {
namespace Conversion {

/// convert tson vector to jt vector
/// \param v the tson vector
/// \return the jt vector
jt::Vector2f vec(tson::Vector2f const& v);

/// convert jt vector to tson vector
/// \param v the jt vector
/// \return the tson vector
jt::Vector2f vec(tson::Vector2i const& v);

} // namespace Conversion
} // namespace jt

#endif // !JAMTEMPLATE_CONVERSIONS_HPP
