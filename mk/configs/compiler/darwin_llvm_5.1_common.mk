include $(BUILD_ROOT)/mk/configs/compiler/include_common.mk
include $(BUILD_ROOT)/mk/configs/compiler/defines_common.mk
include $(BUILD_ROOT)/mk/configs/compiler/darwin_common.mk
include $(BUILD_ROOT)/mk/configs/compiler/llvm5.1-common.mk
include $(BUILD_ROOT)/mk/configs/compiler/ut_flags.mk

CC :=  /usr/bin/clang
CXX := /usr/bin/clang++
GCOV := gcov

BUILD_32BIT := -m32

CC_MUNCH := $(BUILD_ROOT)/mk/bin/empty.sh

LINK_LIB := $(CXX)
LINK_LIB_FLAGS := -r -nostdlib #$(BUILD_32BIT)
POST_LINK_LIB := $(BUILD_ROOT)/mk/bin/empty.sh

LINK_BIN := $(CXX)
LINK_BIN_FLAGS :=  $(LIBS) #$(BUILD_32BIT)

POST_LINK_BIN := @echo

FILE_SIZE := $(LS) $(LS_SIZE)
LOAD_SIZE := $(SIZE)

LINK_LIBS := -ldl -lpthread -lm -lc++

OPT_SPEED := -Os
DEBUG := -g3

DARWIN_LLVM_CFLAGS := $(LINUX_FLAGS_COMMON) \
					$(COMMON_DEFINES) \
					$(LLVM_CFLAGS_COMMON) \
					#$(BUILD_32BIT) # Quantum framework won't build 32-bit

DARWIN_LLVM_CXXFLAGS :=	$(LINUX_FLAGS_COMMON) \
						$(COMMON_DEFINES) \
						$(LLVM_CXXFLAGS_COMMON) \
						#$(BUILD_32BIT)

COVERAGE := -fprofile-arcs -ftest-coverage

DARWIN_LLVM_INCLUDES := $(LINUX_INCLUDES_COMMON) $(COMMON_INCLUDES)
