#------------------------------------------------------------------------------
# Firmware build
#
# Selectable build options 
#------------------------------------------------------------------------------

TARGET_BOARD         ?= BOARD_PCA10000
#TARGET_BOARD         ?= BOARD_PCA10031

USE_DBGLOG           := "no"
USE_RBC_MESH_SERIAL  := "no"

#------------------------------------------------------------------------------
# Define relative paths to SDK components
#------------------------------------------------------------------------------

SDK_BASE      := ../../../../..
COMPONENTS    := $(SDK_BASE)/components
TEMPLATE_PATH := $(COMPONENTS)/toolchain/gcc

LINKER_SCRIPT := ./gcc_nrf51_s110_xxaa.ld
OUTPUT_NAME   := application

#------------------------------------------------------------------------------
# Proceed cautiously beyond this point.  Little should change.
#------------------------------------------------------------------------------

export OUTPUT_NAME
export GNU_INSTALL_ROOT

MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) ) 

ifeq ($(OS),Windows_NT)
  include $(TEMPLATE_PATH)/Makefile.windows
else
  include $(TEMPLATE_PATH)/Makefile.posix
endif

# echo suspend
ifeq ("$(VERBOSE)","1")
  NO_ECHO := 
else
  NO_ECHO := @
endif

ifeq ($(MAKECMDGOALS),debug)
  BUILD_TYPE := debug
else
  BUILD_TYPE := release
endif

# Toolchain commands
CC       := "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc"
AS       := "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as"
AR       := "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar" -r
LD       := "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld"
NM       := "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm"
OBJDUMP  := "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump"
OBJCOPY  := "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy"
SIZE     := "$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size"
MK       := mkdir
RM       := rm -rf
CP       := cp
GENDAT   := ./gen_dat
GENZIP   := zip

BUILDMETRICS  := ./buildmetrics.py

# function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

# source common to all targets

C_SOURCE_FILES += ../main.c
C_SOURCE_FILES += ../led_config.c

ifeq ($(USE_DBGLOG), "yes")
	CFLAGS += -D USE_DBGLOG=1
	C_SOURCE_FILES += ../../../common/src/uart.c
	C_SOURCE_FILES += ../../../common/src/printf.c
endif

ifeq ($(USE_RBC_MESH_SERIAL), "yes")
	CFLAGS += -D RBC_MESH_SERIAL=1

	C_SOURCE_FILES += ../../../rbc_mesh/src/serial_handler.c
	C_SOURCE_FILES += ../../../rbc_mesh/src/serial_queue.c
endif

C_SOURCE_FILES += ../../../rbc_mesh/src/radio_control.c
C_SOURCE_FILES += ../../../rbc_mesh/src/rbc_mesh.c
C_SOURCE_FILES += ../../../rbc_mesh/src/timer_control.c
C_SOURCE_FILES += ../../../rbc_mesh/src/timeslot_handler.c
C_SOURCE_FILES += ../../../rbc_mesh/src/trickle.c
C_SOURCE_FILES += ../../../rbc_mesh/src/mesh_srv.c
C_SOURCE_FILES += ../../../rbc_mesh/src/transport_control.c
C_SOURCE_FILES += ../../../rbc_mesh/src/mesh_aci.c

C_SOURCE_FILES += $(COMPONENTS)/ble/common/ble_advdata.c
C_SOURCE_FILES += $(COMPONENTS)/toolchain/system_nrf51.c

C_SOURCE_FILES += $(COMPONENTS)/drivers_nrf/spi_slave/spi_slave.c

# assembly files common to all targets
ASM_SOURCE_FILES  += $(COMPONENTS)/toolchain/gcc/gcc_startup_nrf51.s

# includes common to all targets

INC_PATHS += -I../include
INC_PATHS += -I../../../rbc_mesh
INC_PATHS += -I../../../rbc_mesh/include
INC_PATHS += -I../../../SDK/bsp
INC_PATHS += -I../../../common/include

INC_PATHS += -I$(COMPONENTS)/softdevice/s110/headers
INC_PATHS += -I$(COMPONENTS)/toolchain/gcc
INC_PATHS += -I$(COMPONENTS)/libraries/util
INC_PATHS += -I$(COMPONENTS)/ble/common
INC_PATHS += -I$(COMPONENTS)/drivers_nrf/hal
INC_PATHS += -I$(COMPONENTS)/drivers_nrf/spi_slave

INC_PATHS += -I$(COMPONENTS)/toolchain/gcc
INC_PATHS += -I$(COMPONENTS)/toolchain
INC_PATHS += -I$(COMPONENTS)/device
INC_PATHS += -I$(COMPONENTS)/softdevice/s110/headers
INC_PATHS += -I$(COMPONENTS)/drivers_nrf/hal
INC_PATHS += -I$(COMPONENTS)/drivers_nrf/spi_slave

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

ifeq ($(BUILD_TYPE),debug)
  DEBUG_FLAGS += -D DEBUG -g -O0
else
  DEBUG_FLAGS += -D NDEBUG -O3
endif

# flags common to all targets
#CFLAGS += -save-temps
CFLAGS += $(DEBUG_FLAGS)
CFLAGS += -D NRF51
CFLAGS += -D BLE_STACK_SUPPORT_REQD
CFLAGS += -D S110
CFLAGS += -D SOFTDEVICE_PRESENT
CFLAGS += -D $(TARGET_BOARD)
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall -Werror
CFLAGS += -Wa,-adhln
CFLAGS += -mfloat-abi=soft
CFLAGS += -ffunction-sections 
CFLAGS += -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin

LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_NAME).map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
LDFLAGS += $(DEBUG_FLAGS)
LDFLAGS += -Wl,--gc-sections
LDFLAGS += --specs=nano.specs -lc -lnosys

# Assembler flags
ASMFLAGS += $(DEBUG_FLAGS)
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -D NRF51
ASMFLAGS += -D BLE_STACK_SUPPORT_REQD
ASMFLAGS += -D S110
ASMFLAGS += -D SOFTDEVICE_PRESENT
ASMFLAGS += -D $(TARGET_BOARD)

C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )

TOOLCHAIN_BASE = $(basename $(notdir $(GNU_INSTALL_ROOT)))

TIMESTAMP := $(shell date +'%s')

vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

all: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_NAME).elf
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

	@echo "*****************************************************"
	@echo "build project: $(OUTPUT_NAME)"
	@echo "build type:    $(BUILD_TYPE)"
	@echo "build with:    $(TOOLCHAIN_BASE)"
	@echo "build target:  $(TARGET_BOARD)"
	@echo "build options  --"
	@echo "               USE_DBGLOG          $(USE_DBGLOG)"
	@echo "               USE_RBC_MESH_SERIAL $(USE_RBC_MESH_SERIAL)"
	@echo "build products --"
	@echo "               $(OUTPUT_NAME).elf"
	@echo "               $(OUTPUT_NAME).hex"
	@echo "*****************************************************"

debug : all

release : all

# Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) \
	-c $< -o $@ > $(OUTPUT_BINARY_DIRECTORY)/$*.lst

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<

# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_NAME).elf
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf

# Create binary .bin file from the .elf file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf
	@echo Preparing: $(OUTPUT_NAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).bin

# Create binary .hex file from the .elf file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf
	@echo Preparing: $(OUTPUT_NAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_NAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).bin

# Create binary .hex file from the .elf file
genhex: 
	@echo Preparing: $(OUTPUT_NAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).hex

echosize:
	-@echo ""
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_NAME).elf
	-@echo ""

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o

