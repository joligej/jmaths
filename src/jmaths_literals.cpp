#include "headers/jmaths_literals.hpp"
#include "headers/jmaths_N.hpp"
#include "headers/jmaths_Z.hpp"

#include "headers/jmaths_def.hh"

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

#include "headers/jmaths_undef.hh"
