#include <expected>

#define TRY_EXPECTED_VOID(expr)                    \
    {                                              \
        auto _tmp_{expr};                          \
        if (!_tmp_.has_value()) {                  \
            return std::unexpected{_tmp_.error()}; \
        }                                          \
    }

#define __CONCAT__(a, b) a##b

#define __TRY_EXPECTED(var, expr, uniq)                          \
    auto __CONCAT__(_tmp_, uniq){expr};                          \
    if (!__CONCAT__(_tmp_, uniq).has_value()) {                  \
        return std::unexpected{__CONCAT__(_tmp_, uniq).error()}; \
    }                                                            \
    var = std::move(__CONCAT__(_tmp_, uniq).value());

#define TRY_EXPECTED(var, expr) __TRY_EXPECTED(var, expr, __COUNTER__)
