#ifndef DAS_CORE_EXCEPTIONS_UNEXPECTEDENUMEXCEPTION_H
#define DAS_CORE_EXCEPTIONS_UNEXPECTEDENUMEXCEPTION_H

#include <das/Utils/Config.h>
#include <cstdint>
#include <stdexcept>
#include <string_view>

DAS_UTILS_NS_BEGIN

struct UnexpectedEnumException : public std::runtime_error
{
    using Base = std::runtime_error;

    explicit(false)
        UnexpectedEnumException(const std::string_view u8_enum_value);
    explicit UnexpectedEnumException(const std::int64_t enum_value);
    explicit UnexpectedEnumException(const std::uint64_t enum_value);

    template <class Enum>
    static auto FromEnum(const Enum enum_value) -> UnexpectedEnumException
    {
        UnexpectedEnumException result{static_cast<std::int64_t>(enum_value)};
        return result;
    }
};

DAS_UTILS_NS_END

#endif // DAS_CORE_EXCEPTIONS_UNEXPECTEDENUMEXCEPTION_H
