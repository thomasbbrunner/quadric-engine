
#pragma once

#include "print.hpp"

#include <stdexcept>
#include <string>

namespace quad
{
class fatal_error : public std::runtime_error
{
public:
    fatal_error(std::string message,
                std::string function_name = "",
                std::string details = "")
        : std::runtime_error(message)
    {
        qe::print::error(message, function_name, details);
    }
};
} // namespace quad
