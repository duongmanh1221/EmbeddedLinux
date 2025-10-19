CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lrt

# Tên các file thực thi
TARGETS = dispatcher worker

all: $(TARGETS)

writer: dispatcher.c
	$(CC) $(CFLAGS) dispatcher.c -o dispatcher $(LDFLAGS)

reader: worker.c
	$(CC) $(CFLAGS) worker.c -o worker $(LDFLAGS)

clean:
	rm -f $(TARGETS)
