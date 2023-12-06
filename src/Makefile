CC = g++
CFLAGS = -O2
TARGET = ChessAI

all: $(TARGET)

$(TARGET): main.cpp Board.cpp Move.cpp MoveList.cpp
	$(CC) $(CFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)