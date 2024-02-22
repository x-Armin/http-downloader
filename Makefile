CC = g++
CFLAGS = -std=c++11 -Wall -I$(INCLUDE_PATH) -g
LDFLAGS = -L$(LIB_PATH) -lcurl -lpthread

OBJ_DIR = obj
BIN_DIR = bin

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

TARGET = $(BIN_DIR)/multi_threaded_downloader

INCLUDE_PATH = /usr/include/curl include
LIB_PATH = /usr/lib64

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
