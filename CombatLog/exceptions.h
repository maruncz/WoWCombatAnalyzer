#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <source_location>
#include <stdexcept>
#include <string>

class CombatLogParserException : public std::exception
{
public:
    CombatLogParserException(
        std::string msg,
        std::source_location loc = std::source_location::current());

    const char *what() const noexcept override;

    std::string getMessage() const;

    std::source_location getPlace() const;

private:
    std::string message;
    std::source_location place;
};

#endif // EXCEPTIONS_H
