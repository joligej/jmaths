# this file contains the installation path and bit architecture of the target system
# it was created by the preconfigure script
build_settings_file_name = build_settings.mk
build_settings_file_dir = ./
build_settings_file = $(build_settings_file_dir)$(build_settings_file_name)

-include $(build_settings_file)

# the names of the library and header to be installed
lib_name = jmaths
lib_file_name = lib$(lib_name).a
lib_file_dir = $(build_dir)
lib_file = $(lib_file_dir)$(lib_file_name)

header_file_name = jmaths.hpp
header_file_dir = $(build_dir)
header_file = $(header_file_dir)$(header_file_name)
#LibName = jmaths
#LibHeader = jmaths.hpp

source_dir = src/
build_dir = build/
config_dir = config/
test_dir = unit_testing/

$(shell mkdir -p "$(build_dir)")

#new:
dependencies_file_name = custom_dependencies.hpp
dependencies_file_dir = $(config_dir)
dependencies_file = $(dependencies_file_dir)$(dependencies_file_name)

default_dependencies_file_name = dependencies.hpp
default_dependencies_file_dir = $(source_dir)
default_dependencies_file = $(default_dependencies_file_dir)$(default_dependencies_file_name)

config_source_file_name = configure.cpp
config_source_file_dir = $(config_dir)
config_source_file = $(config_source_file_dir)$(config_source_file_name)

config_program_name = configure
config_program_dir = $(config_dir)
config_program = $(config_program_dir)$(config_program_name)

user_settings_file_name = user_settings.cfg
user_settings_file_dir = $(config_dir)
user_settings_file = $(user_settings_file_dir)$(user_settings_file_name)

unity_source_file_name = unity.cpp
unity_source_file_dir = $(build_dir)
unity_source_file = $(unity_source_file_dir)$(unity_source_file_name)

unity_obj_file_name = unity.o
unity_obj_file_dir = $(build_dir)
unity_obj_file = $(unity_obj_file_dir)$(unity_obj_file_name)

test_source_file_name = unit_test.cpp
test_source_file_dir = $(test_dir)
test_source_file = $(test_source_file_dir)$(test_source_file_name)

test_program_name = unit_test
test_program_dir = $(test_dir)
test_program = $(test_program_dir)$(test_program_name)

dependencies_script_name = dependencies
dependencies_script_dir = ./
dependencies_script = $(dependencies_script_dir)$(dependencies_script_name)

default_values_list_64_file_name = defaults_list.cfg
default_values_list_64_file_dir = $(config_dir)
default_values_list_64_file = $(default_values_list_64_file_dir)$(default_values_list_64_file_name)

default_values_list_32_file_name = defaults_list32.cfg
default_values_list_32_file_dir = $(config_dir)
default_values_list_32_file = $(default_values_list_32_file_dir)$(default_values_list_32_file_name)

implementation_header_file_name = jmaths.hpp
implementation_header_file_dir = $(source_dir)
implementation_header_file = $(implementation_header_file_dir)$(implementation_header_file_name)

header_objs = $(implementation_header_file) $(user_settings_file) $(dependencies_file)
source_files = jmaths_N.cpp jmaths_Z.cpp jmaths_Q.cpp jmaths_calc.cpp jmaths_misc.cpp jmaths_error.cpp jmaths_literals.cpp
source_objs = $(addprefix $(source_dir), $(source_files))

ifeq ($(bit_architecture), 64)
	default_values_list_file = $(default_values_list_64_file)
else
	default_values_list_file = $(default_values_list_32_file)
endif

cc = clang++
compiler_version = -std=c++2b
compiler_warnings = -Wall -Werror -Wextra -Wpedantic -Wpessimizing-move
ifeq ($(cc), clang++)
	compiler_warnings += -Wreturn-std-move -Wdefaulted-function-deleted
endif
compiler_release = -O3 -march=native -flto
compiler_debug = -g

compile_parms_release = $(compiler_version) $(compiler_warnings) $(compiler_release)
compile_parms_debug = $(compiler_version) $(compiler_warnings) $(compiler_debug)

define compile
	echo "Compiling $(5) in $(4) mode..."
	$(cc) $(3) "$(1)" -c -o "$(2)"
	echo "Successfully compiled $(5)"
endef

define compile_to_exec
	echo "Creating $(4) program..."
	$(cc) $(3) "$(1)" -o "$(2)"
	echo "Successfully created $(4) program"
endef

define archive
	echo "Archiving the library..."
	ar rcs "$(2)" "$(1)"
	echo "Library archived successfully"
endef

.PHONY: all fresh clean build install uninstall configure unity library header test

all:
	@./preconfigure DEFAULT
	@$(MAKE) $(config_program)
	@$(config_program) DEFAULT
	@$(dependencies_script) DEFAULT
	@$(MAKE) build
	
fresh:
	@$(MAKE) clean && $(MAKE) all

clean:
	@echo "Cleaning files..."
	@rm -rf "$(config_program)" "$(user_settings_file)" "$(dependencies_file)" "$(lib_file)" "$(header_file)" "$(unity_source_file)" "$(unity_obj_file)" "$(build_settings_file)" "$(test_program)" "$(test_program).dSYM"
	@echo "Files cleaned successfully"
	
build:
	@echo "Building..."
	@$(MAKE) library && $(MAKE) header
	@echo "Build successful"
	@echo "Library ready for installation"
	
install:
	@echo "Starting installation..."
	@mkdir -p $(install_dir) && mv $(lib_file) $(header_file) $(install_dir)
	@echo "Installation successful"
	@echo "Library installed to: $(install_dir)"
	@echo "Library ready for use"
	
uninstall:
	@echo "Uninstalling..."
	@rm -rf $(install_dir)
	@echo "Uninstalling successful"
	@echo "Library removed from system"
	
debug: $(unity_source_file)
	@$(call compile,$<,$(unity_obj_file),$(compile_parms_debug),"debug","unity file")
	@$(call archive,$(unity_obj_file),$(lib_file))
	
$(user_settings_file): $(config_program)
	@cd "$(config_program_dir)" && ./"$(config_program_name)"

$(config_program): $(config_source_file)
	@$(call compile_to_exec,$<,$@,$(compile_parms_release) -DUSER_SETTINGS="$(user_settings_file)" -DDEFAULTS_LIST="../$(default_values_list_file)","configuration")
	
$(dependencies_file): $(dependencies_script)
	@$(dependencies_script)
	
$(unity_source_file): $(source_objs)
	@rm -f "$@"
	@echo "Creating $@..."
	@for source_file in $^; do cat "$$source_file" >> "$@"; done
	@echo "Successfully created $@"
	
$(unity_obj_file): $(unity_source_file)
	@$(call compile,$<,$@,$(compile_parms_release) -DNDEBUG,"release")

$(lib_file): $(unity_obj_file)
	@$(call archive,$^,$@)
	
$(header_file): $(header_objs)
	@echo "Creating $@..."
	@cat "$(dependencies_file)" > "$@"
	@echo "Preprocessing $<..."
	@$(cc) $(compiler_version) -nostdinc++ -nostdinc -DPREPROCESSING_HEADER "$<" -E -P >> "$@"
	@echo "Successfully created $@"

$(test_program): $(test_source_file)
	@./preconfigure DEFAULT && $(MAKE) configure && (cd "$(config_program_dir)" && ./"$(config_program_name)" DEFAULT) && ($(dependencies_script) DEFAULT) && $(MAKE) header && $(MAKE) debug
	@$(call compile_to_exec,$<,$@,$(compile_parms_debug),"test")
	
configure: $(config_program)

unity: $(unity_obj_file)
	
library: $(lib_file)

header: $(header_file)

test: $(test_program)
