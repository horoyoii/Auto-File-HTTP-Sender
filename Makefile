CC = g++
CFLAGS = -pthread
TARGET = main_exe

$(TARGET): main_program.o AutoFileSender.o LocalMonitor.o Semaphore.o Sender.o
	$(CC) $(CFLAGS) -o $(TARGET) AutoFileSender.o main_program.o LocalMonitor.o Semaphore.o Sender.o

main_program.o: main_program.cpp
	$(CC) -c -o main_program.o main_program.cpp

AutoFileSender.o: AutoFileSender.cpp
	$(CC) $(CFLAGS) -c -o AutoFileSender.o AutoFileSender.cpp 

LocalMonitor.o: LocalMonitor.cpp
	$(CC) -c -o LocalMonitor.o LocalMonitor.cpp
    
Semaphore.o: Semaphore.cpp
	$(CC) -c -o Semaphore.o Semaphore.cpp
    
Sender.o: Sender.cpp
	$(CC) -c -o Sender.o Sender.cpp   




clean:
	rm *.o
