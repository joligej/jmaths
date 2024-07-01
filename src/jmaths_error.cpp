#include "headers/jmaths.hpp"

#include "headers/jmaths_def.cfg"

namespace jmaths {

constexpr const char error::default_message_[] = "No error message provided!";

constexpr const char error::division_by_zero::default_message_[] = "You tried to divide by zero!";

constexpr const char error::invalid_base::default_message_[] = "You tried to use an invalid base!";

error::error() : error(default_message_) {
	FUNCTION_TO_LOG;
}

error::error (const char * message) : message_(message) {
	FUNCTION_TO_LOG;
};

const char * error::what() const noexcept {
	FUNCTION_TO_LOG;

	return message_;
}

error::division_by_zero::division_by_zero() : error(default_message_) {
	FUNCTION_TO_LOG;
}

error::division_by_zero::division_by_zero (const char * message) : error(message) {
	FUNCTION_TO_LOG;
};

error::invalid_base::invalid_base() : error(default_message_) {
	FUNCTION_TO_LOG;
}

error::invalid_base::invalid_base (const char * message) : error(message) {
	FUNCTION_TO_LOG;
};

} // /namespace jmaths

#include "headers/jmaths_undef.cfg"
