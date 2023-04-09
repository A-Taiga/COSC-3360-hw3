CC = /opt/homebrew/bin/g++
CFLAGS = -std=c++17
TARGET = main
APP = main

main: $(TARGET) huffmanTree.cpp
	$(CC) $(CFLAGS) $(TARGET).cpp huffmanTree.cpp -o $(APP)

main.o: main.cpp huffmanTree.cpp
	$(CC) $(CFLAGS) -c $(TARGET).cpp

clean:
	$(RM) $(TARGET)