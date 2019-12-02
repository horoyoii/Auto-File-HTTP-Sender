CC = g++
CFLAGS = -lboost_system -pthread
TARGET = main_exe

$(TARGET): main_program.o AutoFileSender.o LocalMonitor.o Semaphore.o Sender.o HttpClient.o Mutex.o base64.o
	$(CC) -o $(TARGET) AutoFileSender.o main_program.o LocalMonitor.o Semaphore.o Sender.o HttpClient.o base64.o Mutex.o $(CFLAGS)

main_program.o: main_program.cpp
	$(CC) -c -o main_program.o main_program.cpp $(CLFAGS)

AutoFileSender.o: AutoFileSender.cpp
	$(CC)  -c -o AutoFileSender.o AutoFileSender.cpp $(CFLAGS)

LocalMonitor.o: LocalMonitor.cpp
	$(CC) -c -o LocalMonitor.o LocalMonitor.cpp
    
Semaphore.o: Semaphore.cpp
	$(CC) -c -o Semaphore.o Semaphore.cpp
    
Sender.o: Sender.cpp
	$(CC) -c -o Sender.o Sender.cpp $(CFLAGS) 

HttpClient.o: HttpClient.cpp
	$(CC) $(CFLAGS) -c -o HttpClient.o HttpClient.cpp

Mutex.o: Mutex.cpp
	$(CC) $(CFLAGS) -c -o Mutex.o Mutex.cpp

base64.o: base64.cpp
	$(CC) $(CFLAGS) -c -o base64.o base64.cpp

clean:
	rm *.o
