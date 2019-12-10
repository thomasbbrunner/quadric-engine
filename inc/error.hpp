
#pragma once

#include "print.h"

#include <exception>
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
        quad::print::error(message, function_name, details);
    }
};
} // namespace quad