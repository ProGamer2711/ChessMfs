ifeq ($(OS),Windows_NT)
	EXECUTABLE = .exe
	CLEAN = del
else
	EXECUTABLE = .out
	CLEAN = rm
endif

all: main$(EXECUTABLE)

main$(EXECUTABLE): main.c Bot.o Chess.o Constants.h Menu.o Printing.o Replay.o Vector.o
	gcc main.c Bot.o Chess.o Menu.o Printing.o Replay.o Vector.o -o main$(EXECUTABLE)

Bot.o: Bot.c Bot.h
	gcc -c Bot.c -o Bot.o

Chess.o: Chess.c Chess.h
	gcc -c Chess.c -o Chess.o

Menu.o: Menu.c Menu.h
	gcc -c Menu.c -o Menu.o

Printing.o: Printing.c Printing.h
	gcc -c Printing.c -o Printing.o

Replay.o: Replay.c Replay.h
	gcc -c Replay.c -o Replay.o

Vector.o: Vector.c Vector.h
	gcc -c Vector.c -o Vector.o

clean:
	$(CLEAN) *.exe *.out *.o