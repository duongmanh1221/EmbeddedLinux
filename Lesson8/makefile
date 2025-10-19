CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lrt

# Tên các file thực thi
TARGETS = posix_shm_consumer posix_shm_producer

all: $(TARGETS)

writer: posix_shm_consumer.c
	$(CC) $(CFLAGS) posix_shm_consumer.c -o posix_shm_consumer $(LDFLAGS)

reader: posix_shm_producer.c
	$(CC) $(CFLAGS) posix_shm_producer.c -o posix_shm_producer $(LDFLAGS)

clean:
	rm -f $(TARGETS)
