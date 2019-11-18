CC = g++
CFLAGS = -lboost_system -pthread
TARGET = client

$(TARGET): client.o
	$(CC) -o $(TARGET) client.o $(CFLAGS)

client.o: client.cpp
	$(CC) $(CFLAGS) -c -o client.o client.cpp

clean:
	rm *.o
