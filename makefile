BuildSettings = build_settings.mk

-include $(BuildSettings)

LibName = jmaths
LibHeader = jmaths.hpp

SourceDir = src/
BuildDir = build/
ConfigDir = config/
TestDir = unit_testing/

$(shell mkdir -p "$(BuildDir)")

HeaderDependencies = $(ConfigDir)custom_dependencies.hpp
DefaultHeaderDependencies = $(SourceDir)dependencies.hpp
HeaderName = $(BuildDir)$(LibHeader)
ConfigScript = $(ConfigDir)configure.cpp
ConfigProgramName = configure
ConfigProgram = $(ConfigDir)$(ConfigProgramName)
DependenciesProgram = ./dependencies
UserSettings = $(ConfigDir)user_settings.cfg
UnitySource = $(BuildDir)unity.cpp
UnityBuild = $(BuildDir)unity.o
TestSource = $(TestDir)unit_test.cpp
TestProgramName = unit_test
TestProgram = $(TestDir)$(TestProgramName)
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
CompileWarnings = -Wall -Werror -Wextra -Wpedantic -Wpessimizing-move
ifeq ($(CC), clang++)
	CompileWarnings += -Wreturn-std-move -Wdefaulted-function-deleted
endif
CompileOptimisation = -O3 -march=native -flto
CompileDebug = -g

CompileParms = $(CompileVersion) $(CompileWarnings) $(CompileOptimisation)

HeaderObjs = $(SourceDir)jmaths.hpp $(UserSettings) $(HeaderDependencies)
ImplObjs = $(addprefix $(SourceDir), jmaths_N.cpp jmaths_Z.cpp jmaths_Q.cpp jmaths_calc.cpp jmaths_misc.cpp jmaths_error.cpp jmaths_literals.cpp)

.PHONY: all fresh clean build install uninstall configure unity library header test

all:
	@./preconfigure DEFAULT && $(MAKE) configure && cmp -s "$(UserSettings)" "$(DefaultUserSettings)" || (cd "$(ConfigDir)" && ./"$(ConfigProgramName)" DEFAULT) && cmp -s "$(HeaderDependencies)" "$(DefaultHeaderDependencies)" || ($(DependenciesProgram) DEFAULT) && $(MAKE) build
	
fresh:
	@$(MAKE) clean && $(MAKE) all

clean:
	@echo "Cleaning files..."
	@rm -f "$(ConfigProgram)" "$(UserSettings)" "$(HeaderDependencies)" "$(LibFileName)" "$(HeaderName)" "$(UnitySource)" "$(UnityBuild)" "$(BuildSettings)" "$(TestProgram)"
	@echo "Files cleaned successfully"
	
build:
	@echo "Building..."
	@$(MAKE) library && $(MAKE) header
	@echo "Build successful"
	@echo "Library ready for installation"
	
install:
	@echo "Starting installation..."
	@mkdir -p $(InstallDir) && mv $(LibFileName) $(HeaderName) $(InstallDir)
	@echo "Installation successful"
	@echo "Library installed to: $(InstallDir)"
	@echo "Library ready for use"
	
uninstall:
	@echo "Uninstalling..."
	@rm -rf $(InstallDir)
	@echo "Uninstalling successful"
	@echo "Library removed from system"
	
debug: $(UnitySource)
	@echo "Compiling the library in debug mode..."
	@$(CC) $(CompileVersion) $(CompileWarnings) $(CompileDebug) "$<" -c -o "$(UnityBuild)"
	@echo "Library compiled successfully"
	@echo "Archiving the library..."
	@ar rcs "$(LibFileName)" "$(UnityBuild)"
	@echo "Library archived successfully"

	
$(UserSettings): $(ConfigProgram)
	@cd "$(ConfigDir)" && ./"$(ConfigProgramName)"

$(ConfigProgram): $(ConfigScript)
	@echo "Creating a configuration program..."
	@$(CC) $(CompileParms) -DUSER_SETTINGS="../$(UserSettings)" -DDEFAULT_USER_SETTINGS="../$(DefaultUserSettings)" -DDEFAULTS_LIST="../$(DefaultsList)" "$<" -o "$@"
	@echo "Configuration program created"
	
$(HeaderDependencies):
	@$(DependenciesProgram)
	
$(UnitySource): $(ImplObjs) $(HeaderObjs)
	@rm -f "$@"
	@echo "Creating unity file..."
	@for SourceFile in $(ImplObjs); do cat "$$SourceFile" >> "$@"; done
	@echo "Unity file created"
	
$(UnityBuild): $(UnitySource)
	@echo "Compiling the library..."
	@$(CC) $(CompileParms) -DNDEBUG "$<" -c -o "$@"
	@echo "Library compiled successfully"

$(LibFileName): $(UnityBuild)
	@echo "Archiving the library..."
	@ar rcs "$@" "$^"
	@echo "Library archived successfully"
	
$(HeaderName): $(HeaderObjs)
	@echo "Creating a header..."
	@cat "$(HeaderDependencies)" > "$@"
	@$(CC) $(CompileVersion) -nostdinc++ -nostdinc -DPREPROCESSING_HEADER "$<" -E -P >> "$@"
	@echo "Header created successfully"

$(TestProgram): $(TestSource)
	@./preconfigure DEFAULT && $(MAKE) configure && cmp -s "$(UserSettings)" "$(DefaultUserSettings)" || (cd "$(ConfigDir)" && ./"$(ConfigProgramName)" DEFAULT) && cmp -s "$(HeaderDependencies)" "$(DefaultHeaderDependencies)" || ($(DependenciesProgram) DEFAULT) && $(MAKE) header && $(MAKE) debug
	@echo "Compiling test program..."
	@$(CC) $(CompileParms) "$<" -o "$@"
	@echo "Test program compiled successfully"
	
configure: $(ConfigProgram)

unity: $(UnityBuild)
	
library: $(LibFileName)

header: $(HeaderName)

test: $(TestProgram)
