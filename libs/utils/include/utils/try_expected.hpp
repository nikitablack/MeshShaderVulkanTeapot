#include <expected>

#define TRY_EXPECTED_VOID(expr)                    \
    {                                              \
        auto _tmp_{expr};                          \
        if (!_tmp_.has_value()) {                  \
            return std::unexpected{_tmp_.error()}; \
        }                                          \
    }

#define TRY_EXPECTED(var, expr)                    \
    {                                              \
        auto _tmp_{expr};                          \
        if (!_tmp_.has_value()) {                  \
            return std::unexpected{_tmp_.error()}; \
        }                                          \
        var = std::move(_tmp_.value());            \
    }
