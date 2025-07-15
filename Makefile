CC = g++
CPPFLAGS     = -g
LDFLAGS      =
LIBS         = -lm

DESTDIR = ./
SERVER_TARGET = server
SERVER_SOURCES = server.cpp msg_lib.cpp
SERVER_OUTPUT = server.out

CLIENT_TARGET = client
CLIENT_SOURCES = client.cpp msg_lib.cpp
CLIENT_OUTPUT = client.out

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))


$(SERVER_TARGET): $(SERVER_SOURCES)
	$(CC) $(CPPFLAGS) $(SERVER_SOURCES) -o $(SERVER_OUTPUT)

$(CLIENT_TARGET): $(CLIENT_SOURCES)
	$(CC) $(CPPFLAGS) $(CLIENT_SOURCES) -o $(CLIENT_OUTPUT)

all: $(SERVER_TARGET) $(CLIENT_TARGET)

clean:
	-rm -f $(OBJECTS)
	-rm -f $(SERVER_TARGET) $(CLIENT_TARGET)