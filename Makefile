CC = g++
CPPFLAGS     = -g -fsanitize=address -Wall -Werror
# CPPFLAGS     = -g -Wall -Werror
LDFLAGS      =
LIBS         = -lm

DESTDIR = ./
SERVER_TARGET = server
SERVER_OBJS = server.o msg_lib.o table.o

TEST_TARGET = test
TEST_OBJS = test_hashtable.o msg_lib.o table.o

CLIENT_TARGET = client
CLIENT_OBJS = client.o msg_lib.o

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))


$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CPPFLAGS) $^ -o $@.out

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(CPPFLAGS) $^ -o $@.out

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CPPFLAGS) $^ -o $@.out

all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Generic rule for creating .o files
%.o: %.cpp
	${CC} $(CPPFLAGS) -c $<

clean:
	-rm -f $(OBJECTS)
	-rm -f $(SERVER_TARGET) $(CLIENT_TARGET)
	-rm -f *.out
	-rm -f *.o