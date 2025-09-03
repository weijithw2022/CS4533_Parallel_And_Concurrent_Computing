CC = clang
CFLAGS = -Wall -Wextra -pthread
HEADERS = definitions.h

# Source files shared by both
COMMON_SRCS = linked_list.c utils.c
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

# Executables
SERIAL = serial_linked_list
PARALLEL = parallel_single_linked_list

# Default: build both
all: $(SERIAL) $(PARALLEL)

# Serial executable
$(SERIAL): serial_linked_list.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Parallel executable
$(PARALLEL): parallel_single_linked_list.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Generic build rule for .o files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(SERIAL) $(PARALLEL) *.o
