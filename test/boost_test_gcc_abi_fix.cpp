// Boost.Test ABI Compatibility Fix for GCC 15
// This file provides implementations for Boost.Test symbols that are missing
// due to ABI incompatibility between GCC 15 and Clang-compiled Boost

#include <boost/test/unit_test.hpp>
#include <boost/test/tools/detail/print_helper.hpp>

// Provide implementations for the print_log_value specializations
// These are declared in Boost headers but not exported from the Clang-compiled library

namespace boost {
namespace test_tools {
namespace tt_detail {

// Implementation for bool
void print_log_value<bool>::operator()(std::ostream& os, bool t) {
    os << std::boolalpha << t;
}

// Implementation for unsigned char
void print_log_value<unsigned char>::operator()(std::ostream& os, unsigned char t) {
    os << static_cast<int>(t);
}

// Implementation for char
void print_log_value<char>::operator()(std::ostream& os, char t) {
    os << t;
}

} // namespace tt_detail
} // namespace test_tools

namespace unit_test {
namespace ut_detail {

// Provide implementation for normalize_test_case_name
std::string normalize_test_case_name(basic_cstring<char const> name) {
    std::string result;
    result.reserve(name.size());

    bool prev_is_lower = false;
    bool prev_is_upper = false;

    for (size_t i = 0; i < name.size(); ++i) {
        char c = name[i];

        if (c == '_') {
            if (!result.empty() && result.back() != '_') {
                result += '_';
            }
        } else if (std::isupper(c)) {
            if (prev_is_lower || (prev_is_upper && i + 1 < name.size() && std::islower(name[i + 1]))) {
                if (!result.empty() && result.back() != '_') {
                    result += '_';
                }
            }
            result += static_cast<char>(std::tolower(c));
            prev_is_upper = true;
            prev_is_lower = false;
        } else {
            result += c;
            prev_is_lower = std::islower(c);
            prev_is_upper = false;
        }
    }

    return result;
}

} // namespace ut_detail
} // namespace unit_test
} // namespace boost

