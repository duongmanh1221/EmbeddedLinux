# ==== Directory Settings ====
CUR_DIR := .
BUILD_DIR := $(CUR_DIR)/Build

# ==== Compiler Settings ====
CC := gcc
CFLAGS := -Wall -Wextra -O2
TARGET := filestat

# ==== Source and Object Files ====
SRC := filestat.c
OBJ := $(BUILD_DIR)/filestat.o

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
