#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <source_location>
#include <stdexcept>
#include <string>

class CombatLogParserException : public std::exception
{
public:
    CombatLogParserException(const CombatLogParserException&) = delete;
    CombatLogParserException(CombatLogParserException&&)      = delete;
    CombatLogParserException& operator=(const CombatLogParserException&)
        = delete;
    CombatLogParserException& operator=(CombatLogParserException&&) = delete;
    CombatLogParserException(
        std::string msg,
        std::source_location loc = std::source_location::current());
    ~CombatLogParserException() override = default;

    [[nodiscard]] const char* what() const noexcept override;

    [[nodiscard]] std::string getMessage() const;

    [[nodiscard]] std::source_location getPlace() const;

private:
    std::string message;
    std::source_location place;
};

#endif // EXCEPTIONS_H
