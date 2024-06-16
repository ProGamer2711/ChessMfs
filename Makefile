ifeq ($(OS),Windows_NT)
	EXECUTABLE = .exe
	CLEAN = del
else
	EXECUTABLE = .out
	CLEAN = rm
endif

all: main$(EXECUTABLE)

main$(EXECUTABLE): main.c Bot.o Chess.o Menu.o Printing.o Replay.o Vector.o Replay.h Chess.h Constants.h Menu.h
	gcc main.c -o main$(EXECUTABLE) Bot.o Chess.o Menu.o Printing.o Replay.o Vector.o

Bot.o: Bot.c Bot.h Chess.h Constants.h Vector.h
	gcc Bot.c -c -o Bot.o

Chess.o: Chess.c Chess.h Bot.h Constants.h Menu.h Printing.h Replay.h Vector.h
	gcc Chess.c -c -o Chess.o

Menu.o: Menu.c Menu.h Chess.h Constants.h Printing.h Replay.h Vector.h
	gcc Menu.c -c -o Menu.o

Printing.o: Printing.c Printing.h Chess.h Vector.h
	gcc Printing.c -c -o Printing.o

Replay.o: Replay.c Replay.h Chess.h Vector.h Constants.h Menu.h
	gcc Replay.c -c -o Replay.o

Vector.o: Vector.c Vector.h
	gcc Vector.c -c -o Vector.o

clean:
	$(CLEAN) *.exe *.out *.o