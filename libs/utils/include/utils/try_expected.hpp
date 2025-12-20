#include <expected>

#define CONCAT(a, b) a##b

#define TRY_EXPECTED_VOID_IMPL(expr, uniq)                   \
    auto CONCAT(_tmp_, uniq) = expr;                         \
    if (!CONCAT(_tmp_, uniq).has_value()) {                  \
        return std::unexpected{CONCAT(_tmp_, uniq).error()}; \
    }

#define TRY_EXPECTED_VOID(expr) TRY_EXPECTED_VOID_IMPL(expr, __COUNTER__)