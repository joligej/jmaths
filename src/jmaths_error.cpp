#include "../src/jmaths.hpp"

namespace jmaths {

constexpr const char error::default_message_[] = "No error message provided!";

constexpr const char error::division_by_zero::default_message_[] = "You tried to divide by zero!";

constexpr const char error::invalid_base::default_message_[] = "You tried to use an invalid base!";

error::error() : error(default_message_) {}

error::error (const char * message) : message_(message) {};

const char * error::what() const noexcept {
	return message_;
}

error::division_by_zero::division_by_zero() : error(default_message_) {}

error::division_by_zero::division_by_zero (const char * message) : error(message) {};

error::invalid_base::invalid_base() : error(default_message_) {}

error::invalid_base::invalid_base (const char * message) : error(message) {};

} // /namespace jmaths
