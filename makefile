# Define cross-compiler
CC      := arm-none-eabi-gcc
CXX     := arm-none-eabi-g++

# Define the source and object file directories
CORE_DIR := Core
STARTUP_DIR := Startup
OBJ_DIR := Debug
LINKER := stm32_ls.ld
INC_DIR := Include
TESTS_DIR := Tests
UTILS_DIR := Utils
DRIVERS_DIR := Drivers

OPT_DBG_FLAGS := -g3 -O0
C_STDR := -std=gnu11
CXX_STDR := -std=gnu++20

PROJECT_NAME := $(shell basename "$(shell pwd)")
LINKER_PATH := $(shell pwd)/${STARTUP_DIR}/${LINKER}
TARGET := ${OBJ_DIR}/${PROJECT_NAME}.elf

# Define include directories
INC_DIRS := $(shell find ${CORE_DIR}/${INC_DIR} -type d) $(shell find ${CORE_DIR}/${UTILS_DIR}/* -type d) $(shell find ${CORE_DIR}/${DRIVERS_DIR}/* -type d)
INC_FLAGS := $(addprefix -I, $(INC_DIRS))

EXCEPTIONS_FLAG := -fexceptions
NANO_SPECS := --specs=nano.specs
RTTI := -fno-rtti

# Define the flags
CFLAGS  := -mcpu=cortex-m4 $(C_STDR) -c ${OPT_DBG_FLAGS} ${NANO_SPECS} -ffunction-sections -fdata-sections ${EXCEPTIONS_FLAG} -Wall -fstack-usage -MMD -MP  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb $(INC_FLAGS)
CXXFLAGS:= -mcpu=cortex-m4 $(CXX_STDR) -c ${OPT_DBG_FLAGS} ${NANO_SPECS} -ffunction-sections -fdata-sections ${EXCEPTIONS_FLAG} -Wall -fstack-usage -MMD -MP  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb $(INC_FLAGS) $(RTTI) -fno-use-cxa-atexit

# Automatically gather all source files and determine the object and dependency file paths
CPP_SOURCES_CORE := $(shell find $(CORE_DIR) -name '*.cpp' -not -path "$(TESTS_DIR)/*")
C_SOURCES_CORE   := $(shell find $(CORE_DIR) -name '*.c' -not -path "$(TESTS_DIR)/*")
ASM_SOURCES_STARTUP := $(shell find $(STARTUP_DIR) -name '*.s')
CPP_OBJECTS := $(CPP_SOURCES_CORE:$(CORE_DIR)/%.cpp=$(OBJ_DIR)/%.o)
C_OBJECTS   := $(C_SOURCES_CORE:$(CORE_DIR)/%.c=$(OBJ_DIR)/%.o)
ASM_OBJECTS := $(ASM_SOURCES_STARTUP:%.s=$(OBJ_DIR)/%.o)
DEPS        := $(CPP_OBJECTS:.o=.d) $(C_OBJECTS:.o=.d) $(ASM_OBJECTS:.o=.d)
MACROS := COMPILE

all: clean build

build: $(TARGET)

flash: all
	openocd -f interface/stlink.cfg -f board/st_nucleo_f4.cfg -c "program ./${OBJ_DIR}/${PROJECT_NAME}.elf verify reset exit"

$(TARGET): $(CPP_OBJECTS) $(C_OBJECTS) $(ASM_OBJECTS) 
	$(CXX) -T $(LINKER_PATH) $^ -o $@ -Wl,-Map=$(TARGET:.elf=.map),--cref -D$(MACROS) -mcpu=cortex-m4 ${NANO_SPECS} -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group -Wl,--print-memory-usage
	@echo 'Finished building target: $@'
	@echo ' '

# Rule to make the object files
$(OBJ_DIR)/%.o: $(CORE_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $< $(CXXFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@"

$(OBJ_DIR)/%.o: $(CORE_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $< $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@"

$(OBJ_DIR)/${STARTUP_DIR}/%.o: $(STARTUP_DIR)/%.s
	@mkdir -p $(@D)
	$(CC) -c $< -o $@

# Include all .d files
-include $(DEPS)

.PHONY: clean

clean:
	rm -rf $(OBJ_DIR)
