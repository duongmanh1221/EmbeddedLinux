# ==== Directories ====
CUR_DIR := .
APP_DIR := $(CUR_DIR)/Application
LIB_DIR := $(CUR_DIR)/library/source
INC_DIR := $(CUR_DIR)/library/header
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin

# ==== Compiler settings ====
CC := gcc
CFLAGS := -Wall -fPIC -I$(INC_DIR)

# ==== Libraries ====
STATIC_LIB := $(LIB_DIR)/libstrutils.a
SHARED_LIB := $(LIB_DIR)/libstrutils.so

# ==== Object files ====
OBJ := $(OBJ_DIR)/main.o $(OBJ_DIR)/strutils.o

# ==== Default target ====
all: $(BIN_DIR)/main

# ==== Link final binary ====
$(BIN_DIR)/main: $(OBJ)
ifeq ($(wildcard $(STATIC_LIB)),)
	@echo "Linking with shared library"
	$(CC) $^ -L$(LIB_DIR) -lstrutils -Wl,-rpath=$(LIB_DIR) -o $@
else
	@echo "Linking with static library"
	$(CC) $^ $(STATIC_LIB) -o $@
endif

# ==== Build static library ====
static: $(OBJ_DIR)/strutils.o
	ar rcs $(STATIC_LIB) $<

# ==== Build shared library ====
shared: $(OBJ_DIR)/strutils.o
	$(CC) -shared -o $(SHARED_LIB) $<

# ==== Compile object files ====
$(OBJ_DIR)/main.o: $(APP_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/strutils.o: $(LIB_DIR)/strutils.c
	$(CC) $(CFLAGS) -c $< -o $@

# ==== Clean ====
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/main $(STATIC_LIB) $(SHARED_LIB)
