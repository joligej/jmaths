install_dir ?= "/usr/local/jmaths/"
pth_extension ?= .gch

# the names of the library and header to be installed
lib_name = jmaths
lib_file_name = lib$(lib_name).a
lib_file_dir = $(build_dir)
lib_file = $(lib_file_dir)$(lib_file_name)

header_file_name = jmaths.hpp
header_file_dir = $(build_dir)
header_file = $(header_file_dir)$(header_file_name)

debug_lib_name = dbg$(lib_name)
debug_lib_file_name = lib$(debug_lib_name).a
debug_lib_file_dir = $(build_dir)
debug_lib_file = $(debug_lib_file_dir)$(debug_lib_file_name)

debug_header_file_name = dbg$(header_file_name)
debug_header_file_dir = $(build_dir)
debug_header_file = $(debug_header_file_dir)$(debug_header_file_name)

source_dir = src/
build_dir = build/
config_dir = config/
test_dir = test/

$(shell mkdir -p "$(build_dir)")

dependencies_file_name = dependencies.hpp
dependencies_file_dir = $(config_dir)
dependencies_file = $(dependencies_file_dir)$(dependencies_file_name)

config_source_file_name = configure.cpp
config_source_file_dir = $(config_dir)
config_source_file = $(config_source_file_dir)$(config_source_file_name)

config_program_name = configure
config_program_dir = $(build_dir)
config_program = $(config_program_dir)$(config_program_name)

user_settings_file_name = user_settings.cfg
user_settings_file_dir = $(build_dir)
user_settings_file = $(user_settings_file_dir)$(user_settings_file_name)

unity_source_file_name = unity.cpp
unity_source_file_dir = $(build_dir)
unity_source_file = $(unity_source_file_dir)$(unity_source_file_name)

unity_obj_file_name = unity.o
unity_obj_file_dir = $(build_dir)
unity_obj_file = $(unity_obj_file_dir)$(unity_obj_file_name)

debug_unity_obj_file_name = debug_unity.o
debug_unity_obj_file_dir = $(build_dir)
debug_unity_obj_file = $(debug_unity_obj_file_dir)$(debug_unity_obj_file_name)

test_source_file_name = unit_test.cpp
test_source_file_dir = $(test_dir)
test_source_file = $(test_source_file_dir)$(test_source_file_name)

test_program_name = unit_test
test_program_dir = $(test_dir)
test_program = $(test_program_dir)$(test_program_name)

error_log_source_file_name = handle_error_log.cpp
error_log_source_file_dir = $(test_dir)
error_log_source_file = $(error_log_source_file_dir)$(error_log_source_file_name)

error_log_program_name = handle_error_log
error_log_program_dir = $(test_dir)
error_log_program = $(error_log_program_dir)$(error_log_program_name)

implementation_header_file_name = jmaths.hpp
implementation_header_file_dir = $(source_dir)
implementation_header_file = $(implementation_header_file_dir)$(implementation_header_file_name)

header_objs = $(implementation_header_file) $(user_settings_file) $(dependencies_file) $(source_dir)jmaths_tmpl.tpp $(config_dir)jmaths_aliases.hpp
source_files = jmaths_N.cpp jmaths_Z.cpp jmaths_Q.cpp jmaths_calc.cpp jmaths_misc.cpp jmaths_error.cpp jmaths_literals.cpp jmaths_hash.cpp jmaths_rand.cpp
source_objs = $(addprefix $(source_dir), $(source_files))

create_while_building = "$(config_program)" "$(user_settings_file)" "$(unity_source_file)" "$(lib_file)" "$(debug_lib_file)" "$(header_file)"* "$(debug_header_file)"* "$(unity_obj_file)" "$(debug_unity_obj_file)" "$(test_program)" "$(test_program).dSYM" "$(error_log_program)"

cc ?= clang++
compiler_version = -std=c++2b
compiler_warnings = -Wall -Werror -Wextra -Wpedantic -Wpessimizing-move
ifeq ($(cc), clang++)
	compiler_warnings += -Wreturn-std-move -Wdefaulted-function-deleted
endif
compiler_release = -O3 -march=native -flto -DNDEBUG
compiler_debug = -O0 -g

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

define preprocess_header
	echo "Creating $(2)..."
	echo "#pragma once" > "$(2)"
	cat "$(4)" >> "$(2)"
	echo "Preprocessing $(1) in $(5) mode..."
	$(cc) $(compiler_version) -nostdinc++ -nostdinc -DPREPROCESSING_HEADER $(3) "$(1)" -E -P >> "$(2)"
	echo "Successfully created $(2)"
endef

define precompile_header
	echo "Precompiling $(1)..."
	$(cc) $(compiler_version) -x c++-header $(2) "$(1)" -o "$(1)$(pth_extension)"
	echo "Precompilation of header succesful"
endef

export

.PHONY: all debug fresh clean build debug_build install uninstall configure unity debug_unity library debug_library header debug_header test

all:
	@$(MAKE) build

debug:
	@$(MAKE) debug_build
	
fresh:
	@$(MAKE) clean && $(MAKE) all

clean:
	@echo "Cleaning files..."
	@rm -rf $(create_while_building)
	@echo "Files cleaned successfully"
	
build:
	@echo "Building..."
	@$(MAKE) library && $(MAKE) header
	@echo "Build successful"
	@echo "Library ready for installation"

debug_build:
	@echo "Building in debug mode..."
	@$(MAKE) debug_library && $(MAKE) debug_header
	@echo "Debug build successful"

install:
	@echo "Starting installation..."
	@mkdir -p $(install_dir) && mv $(lib_file) $(header_file)* $(install_dir)
	@echo "Installation successful"
	@echo "Library installed to: $(install_dir)"
	@echo "Library ready for use"
	
uninstall:
	@echo "Uninstalling..."
	@rm -rf $(install_dir)
	@echo "Uninstalling successful"
	@echo "Library removed from system"
	
$(user_settings_file): $(config_program)
	@$(config_program)

$(config_program): $(config_source_file)
	@$(call compile_to_exec,$<,$@,$(compile_parms_release) -DSETTINGS_FILE="$(user_settings_file)","configuration")
	
$(unity_source_file): $(source_objs)
	@rm -f "$@"
	@echo "Creating $@..."
	@for source_file in $^; do echo "#include \"../$$source_file\"" >> "$@"; done
	@echo "Successfully created $@"
	
$(unity_obj_file): $(unity_source_file) $(header_objs)
	@$(call compile,$<,$@,$(compile_parms_release),"release","unity file")
	
$(debug_unity_obj_file): $(unity_source_file) $(header_objs)
	@$(call compile,$<,$@,$(compile_parms_debug),"debug","unity file")

$(lib_file): $(unity_obj_file)
	@$(call archive,$^,$@)

$(debug_lib_file): $(debug_unity_obj_file)
	@$(call archive,$^,$@)
	
$(header_file): $(header_objs)
	@$(call preprocess_header,$<,$@,-DNDEBUG,$(dependencies_file),"release")
	@$(call precompile_header,$@,$(patsubst -flto,,$(compiler_release)))

$(debug_header_file): $(header_objs)
	@$(call preprocess_header,$<,$@,,$(dependencies_file),"debug")
	@$(call precompile_header,$@,$(compiler_debug))

$(test_program): $(test_source_file) $(debug_lib_file) $(debug_header_file)
	@$(call compile_to_exec,$<,$@,$(compile_parms_debug) $(debug_lib_file),"unit test")
	
$(error_log_program): $(error_log_source_file)
	@$(call compile_to_exec,$<,$@,$(compile_parms_release),"error handling")

configure: $(config_program)

unity: $(unity_obj_file)

debug_unity: $(debug_unity_obj_file)
	
library: $(lib_file)

debug_library: $(debug_lib_file)

header: $(header_file)

debug_header: $(debug_header_file)

test: $(test_program) $(error_log_program)
