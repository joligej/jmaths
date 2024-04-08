LibName = libjmaths.a

CC = clang++

CompileParms = -std=c++2b -Wall -Werror -Wextra -Wpedantic -Wreturn-std-move -Wpessimizing-move -Wdefaulted-function-deleted -O3 -march=native -flto

HeaderObjs = jmaths.hpp jmaths_tmpl.cpp user_settings.cfg jmaths_def.cfg jmaths_undef.cfg

ImplObjs = jmaths_N.cpp jmaths_Z.cpp jmaths_Q.cpp jmaths_calc.cpp jmaths_misc.cpp jmaths_error.cpp jmaths_literals.cpp

jmaths.o: $(ImplObjs) $(HeaderObjs)
	$(CC) $(CompileParms) jmaths.cpp -c

clean:
	rm -f *.o jmaths

library: jmaths.o
	ar rcs $(LibName) jmaths.o
