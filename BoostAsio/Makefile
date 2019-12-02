CC = g++
CFLAGS = -lboost_system -pthread
TARGET = client

$(TARGET): client.o
	$(CC) -o $(TARGET) client.o base64.o $(CFLAGS)

client.o: client.cpp
	$(CC) $(CFLAGS) -c -o client.o client.cpp

base64.o: base64.cpp
	$(cc) $(CFLAGS) -c -o base64.o base64.cpp
clean:
	rm *.o
