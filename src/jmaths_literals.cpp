#include "jmaths.hpp"

namespace jmaths {

inline namespace literals {

N operator ""_N (const char * num_str) {
	return N(num_str);
}

Z operator ""_Z (const char * num_str) {
	return Z(num_str);
}

} // /namespace literals

} // /namespace jmaths