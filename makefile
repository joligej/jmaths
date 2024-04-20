LibName = jmaths
HeaderName = header.hpp
Architecture = $(shell getconf LONG_BIT)

UserSettings = user_settings.cfg
UnitySource = unity.cpp
UnityBuild = unity.o
DefaultUserSettings64 = default_user_settings.cfg
DefaultUserSettings32 = default_user_settings32.cfg
DefaultsList64 = defaults_list.cfg
DefaultsList32 = defaults_list32.cfg

LibFileName = lib$(LibName).a

ifeq ($(Architecture), 64)
	DefaultUserSettings = $(DefaultUserSettings64)
	DefaultsList = $(DefaultsList64)
else
	DefaultUserSettings = $(DefaultUserSettings32)
	DefaultsList = $(DefaultsList32)
endif

CC = clang++
CompileVersion = -std=c++2b
CompileWarnings = -Wall -Werror -Wextra -Wpedantic -Wreturn-std-move -Wpessimizing-move -Wdefaulted-function-deleted
CompileOptimisation = -O3 -march=native -flto

CompileParms = $(CompileVersion) $(CompileWarnings) $(CompileOptimisation)

HeaderObjs = jmaths.hpp jmaths_tmpl.cpp $(UserSettings) jmaths_def.cfg jmaths_undef.cfg
ImplObjs = jmaths_N.cpp jmaths_Z.cpp jmaths_Q.cpp jmaths_calc.cpp jmaths_misc.cpp jmaths_error.cpp jmaths_literals.cpp

.PHONY: all fresh clean build install unity library header

all:
	@$(MAKE) configure && cmp -s $(UserSettings) $(DefaultUserSettings) || ./configure DEFAULT && $(MAKE) build
	
fresh:
	@$(MAKE) clean && $(MAKE) all

clean:
	@echo "Cleaning files..."
	@rm -f configure $(UserSettings) *.o $(LibFileName) $(HeaderName) $(UnitySource)
	@echo "Files cleaned succesfully"
	
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

configure: configure.cpp
	@echo "Creating a configuration program..."
	@$(CC) $(CompileParms) -DUSER_SETTINGS=$(UserSettings) -DDEFAULT_USER_SETTINGS=$(DefaultUserSettings) -DDEFAULTS_LIST=$(DefaultsList) $< -o $@
	@echo "Configuration program created"
	
$(UserSettings): configure
	@./configure

$(LibFileName): $(UnityBuild)
	@echo "Archiving the library..."
	@ar rcs $@ $^
	@echo "Library archived succesfully"
	
$(HeaderName): $(HeaderObjs)
	@echo "Creating a header..."
	@$(CC) $(CompileVersion) $< -E > $@
	@echo "Header created succesfully"
	
$(UnitySource): $(ImplObjs) $(HeaderObjs)
	@rm -f $@
	@echo "Creating unity file..."
	@for SourceFile in $(ImplObjs); do cat $$SourceFile >> $@; done
	@echo "Unity file created"
	
$(UnityBuild): $(UnitySource)
	@echo "Compiling the library..."
	@$(CC) $(CompileParms) $< -c
	@echo "Library compiled succesfully"
	
unity: $(UnityBuild)
	
library: $(LibFileName)

header: $(HeaderName)
