#include "exceptions.h"

CombatLogParserException::CombatLogParserException(std::string msg,
                                                   std::source_location loc)
    : message{std::move(msg)}, place{std::move(loc)}
{
}

const char *CombatLogParserException::what() const noexcept
{
    return message.c_str();
}

std::string CombatLogParserException::getMessage() const
{
    return message;
}

std::source_location CombatLogParserException::getPlace() const
{
    return place;
}
