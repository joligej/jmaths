LibName = jmaths
LibFileName = lib$(LibName).a
HeaderName = header.hpp

UserSettings = user_settings.cfg
DefaultUserSettings64 = default_user_settings.cfg
DefaultUserSettings32 = default_user_settings32.cfg
DefaultUserSettings = $(DefaultUserSettings64)
DefaultsList64 = defaults_list.cfg
DefaultsList32 = defaults_list32.cfg

ifeq ($(DefaultUserSettings), $(DefaultUserSettings64))
	DefaultsList = $(DefaultsList64)
else
	DefaultsList = $(DefaultsList32)
endif

CC = clang++
CompileVersion = -std=c++2b
CompileWarnings = -Wall -Werror -Wextra -Wpedantic -Wreturn-std-move -Wpessimizing-move -Wdefaulted-function-deleted
CompileOptimisation = -O3 -march=native -flto

CompileParms = $(CompileVersion) $(CompileWarnings) $(CompileOptimisation)

HeaderObjs = jmaths.hpp jmaths_tmpl.cpp $(UserSettings) jmaths_def.cfg jmaths_undef.cfg
ImplObjs = jmaths_N.cpp jmaths_Z.cpp jmaths_Q.cpp jmaths_calc.cpp jmaths_misc.cpp jmaths_error.cpp jmaths_literals.cpp

.PHONY: all fresh clean build install

all:
	@$(MAKE) configure && cmp -s $(UserSettings) $(DefaultUserSettings) || ./configure DEFAULT && $(MAKE) build
	
fresh:
	@$(MAKE) clean && $(MAKE) all

configure: configure.cpp
	@echo "Creating a configuration program..."
	@$(CC) $(CompileVersion) $(CompileOptimisation) -DUSER_SETTINGS="\"$(UserSettings)\"" -DDEFAULT_USER_SETTINGS="\"$(DefaultUserSettings)\"" -DDEFAULTS_LIST="\"$(DefaultsList)\"" $< -o $@
	@echo "Configuration program created"
	
$(UserSettings): configure
	@./configure

jmaths.o: $(ImplObjs) $(HeaderObjs)
	@echo "Compiling the library..."
	@$(CC) $(CompileParms) jmaths.cpp -c
	@echo "Library compiled succesfully"

clean:
	@rm -f configure $(UserSettings) *.o $(LibFileName) $(HeaderName)

library: jmaths.o
	@echo "Archiving the library..."
	@ar rcs $(LibFileName) jmaths.o
	@echo "Library archived succesfully"
	
header: $(HeaderObjs)
	@echo "Creating a header..."
	@$(CC) $(CompileVersion) $< -E > $(HeaderName)
	@echo "Header created succesfully"
	
build:
	@echo "Building..."
	@$(MAKE) library && $(MAKE) header
	@echo "Build succesful"
	@echo "Library ready for installation"
	
install:
	@echo "Starting installation..."
	@:
	@echo "Installation succesful"
	@echo "Library ready for use"
