#pragma once

#include <iosfwd>
#include <string>

#include <sandbox/export.hpp>

namespace sandbox
{
  // Print a greeting for the specified name into the specified
  // stream. Throw std::invalid_argument if the name is empty.
  //
  SANDBOX_SYMEXPORT void
  say_hello (std::ostream&, const std::string& name);
}
