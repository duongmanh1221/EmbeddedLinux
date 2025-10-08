CC = gcc
CFLAGS = -Wall -Wextra -O2
OBJ = main.o conect.o server.o utils.o
DEPS = conect.h server.h utils.h my_config.h

TARGET = main

all: $(TARGET)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) -lpthread

clean:
	rm -f $(OBJ) $(TARGET)
