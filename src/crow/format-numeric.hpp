#pragma once

#include "crow/format-spec.hpp"
#include "crow/types.hpp"
#include <string>

namespace Crow::Detail {

    void expand_formatted_number(std::string& numstr, const FormatSpec& spec);

}
