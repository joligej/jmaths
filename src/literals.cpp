#include "headers/literals.hpp"
#include "headers/N.hpp"
#include "headers/Z.hpp"

#include "headers/def.hh"

namespace jmaths {

inline namespace literals {

N operator ""_N (const char * num_str) {
	FUNCTION_TO_LOG;

	return N(num_str);
}

Z operator ""_Z (const char * num_str) {
	FUNCTION_TO_LOG;

	return Z(num_str);
}

} // /namespace literals

} // /namespace jmaths

#include "headers/undef.hh"
