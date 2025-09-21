# ==== Directory Settings ====
CUR_DIR := .
BUILD_DIR := $(CUR_DIR)/Build

# ==== Compiler Settings ====
CC := gcc
CFLAGS := -Wall -Wextra -O2 -pthread
TARGET := main

# ==== Source and Object Files ====
SRC := main.c
OBJ := $(BUILD_DIR)/main.o

# ==== Default target ====
all: $(TARGET)

# ==== Link object file to executable ====
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

# ==== Compile .c to .o ====
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ==== Clean build ====
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
