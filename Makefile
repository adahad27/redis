CC = g++
CPPFLAGS     = -g
LDFLAGS      =
LIBS         = -lm

DESTDIR = ./
SERVER_TARGET = server
SERVER_OBJS = server.o msg_lib.o

CLIENT_TARGET = client
CLIENT_OBJS = client.o msg_lib.o

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))


$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CPPFLAGS) $^ -o $@.out

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(CPPFLAGS) $^ -o $@.out

all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Generic rule for creating .o files
%.o: %.cpp
	${CC} -c $<

clean:
	-rm -f $(OBJECTS)
	-rm -f $(SERVER_TARGET) $(CLIENT_TARGET)
	-rm -f *.out