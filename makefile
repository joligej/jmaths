LibName = jmaths
LibHeader = jmaths.hpp
Architecture = $(shell getconf LONG_BIT)

SourceDir = src/
BuildDir = build/
ConfigDir = config/

$(shell mkdir -p $(BuildDir))

HeaderName = $(BuildDir)$(LibHeader)
ConfigScript = $(ConfigDir)configure.cpp
ConfigProgramName = configure
ConfigProgram = $(ConfigDir)$(ConfigProgramName)
UserSettings = $(ConfigDir)user_settings.cfg
UnitySource = $(BuildDir)unity.cpp
UnityBuild = $(BuildDir)unity.o
DefaultUserSettings64 = $(ConfigDir)default_user_settings.cfg
DefaultUserSettings32 = $(ConfigDir)default_user_settings32.cfg
DefaultsList64 = $(ConfigDir)defaults_list.cfg
DefaultsList32 = $(ConfigDir)defaults_list32.cfg

LibFileName = $(BuildDir)lib$(LibName).a

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

HeaderObjs = $(SourceDir)jmaths.hpp $(UserSettings)
ImplObjs = $(addprefix $(SourceDir), jmaths_N.cpp jmaths_Z.cpp jmaths_Q.cpp jmaths_calc.cpp jmaths_misc.cpp jmaths_error.cpp jmaths_literals.cpp)

.PHONY: all fresh clean build install configure unity library header

all:
	@$(MAKE) configure && cmp -s $(UserSettings) $(DefaultUserSettings) || (cd $(ConfigDir) && ./$(ConfigProgramName) DEFAULT) && $(MAKE) build
	
fresh:
	@$(MAKE) clean && $(MAKE) all

clean:
	@echo "Cleaning files..."
	@rm -f $(ConfigProgram) $(UserSettings) $(LibFileName) $(HeaderName) $(UnitySource) $(UnityBuild)
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

$(ConfigProgram): $(ConfigScript)
	@echo "Creating a configuration program..."
	@$(CC) $(CompileParms) -DUSER_SETTINGS="../$(UserSettings)" -DDEFAULT_USER_SETTINGS="../$(DefaultUserSettings)" -DDEFAULTS_LIST="../$(DefaultsList)" $< -o $@
	@echo "Configuration program created"
	
$(UserSettings): $(ConfigProgram)
	@cd $(ConfigDir) && ./$(ConfigProgramName)

$(LibFileName): $(UnityBuild)
	@echo "Archiving the library..."
	@ar rcs $@ $^
	@echo "Library archived succesfully"
	
$(HeaderName): $(HeaderObjs)
	@echo "Creating a header..."
	@$(CC) $(CompileVersion) $< -E -o $@
	@echo "Header created succesfully"
	
$(UnitySource): $(ImplObjs) $(HeaderObjs)
	@rm -f $@
	@echo "Creating unity file..."
	@for SourceFile in $(ImplObjs); do cat $$SourceFile >> $@; done
	@echo "Unity file created"
	
$(UnityBuild): $(UnitySource)
	@echo "Compiling the library..."
	@$(CC) $(CompileParms) $< -c -o $@
	@echo "Library compiled succesfully"
	
configure: $(ConfigProgram)

unity: $(UnityBuild)
	
library: $(LibFileName)

header: $(HeaderName)
