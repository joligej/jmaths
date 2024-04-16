LibName = jmaths

USER_SETTINGS = config_file.cfg

CC = clang++

CompileVersion = -std=c++2b

CompileWarnings = -Wall -Werror -Wextra -Wpedantic -Wreturn-std-move -Wpessimizing-move -Wdefaulted-function-deleted

CompileOptimisation = -O3 -march=native -flto

CompileParms = $(CompileVersion) $(CompileWarnings) $(CompileOptimisation)

HeaderObjs = jmaths.hpp jmaths_tmpl.cpp user_settings.cfg jmaths_def.cfg jmaths_undef.cfg

ImplObjs = jmaths_N.cpp jmaths_Z.cpp jmaths_Q.cpp jmaths_calc.cpp jmaths_misc.cpp jmaths_error.cpp jmaths_literals.cpp

.PHONY: clean

all:
	@$(MAKE) configure && ./configure && $(MAKE) library
	

configure: configure.cpp
	@echo "Creating a configuration program..."
	@$(CC) $(CompileVersion) $(CompileOptimisation) -DUSER_SETTINGS="\"$(USER_SETTINGS)\"" configure.cpp -o configure
	@echo "Configuration program created"

jmaths.o: $(ImplObjs) $(HeaderObjs)
	@echo "Compiling the library..."
	@$(CC) $(CompileParms) jmaths.cpp -c
	@echo "Library compiled succesfully"

clean:
	@rm -f *.o configure $(USER_SETTINGS)

library: jmaths.o
	@echo "Archiving the library..."
	@ar rcs lib$(LibName).a jmaths.o
	@echo "Library archived succesfully"
