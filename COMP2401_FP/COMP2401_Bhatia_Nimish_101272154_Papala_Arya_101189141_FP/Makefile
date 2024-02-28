CC = gcc
OPT = -Wall
CFLAGS = -pthread
DEPS = defs.h
OBJ = main.o ghost.o hunter.o room.o house.o evidence.o logger.o utils.o

ghostHunter: $(OBJ)
	$(CC) $(OPT) -o ghostHunter ${OBJ} $(CFLAGS)

main.o: main.c defs.h
	$(CC) $(OPT) -c main.c

ghost.o: ghost.c defs.h
	$(CC) $(OPT) -c ghost.c defs.h

hunter.o: hunter.c defs.h
	$(CC) $(OPT) -c hunter.c defs.h

room.o: room.c defs.h
	$(CC) $(OPT) -c room.c defs.h

house.o: house.c defs.h
	$(CC) $(OPT) -c house.c defs.h

evidece.o: evidence.c defs.h
	$(CC) $(OPT) -c evidence.c defs.h

logger.o: logger.c defs.h
	$(CC) $(OPT) -c logger.c defs.h

utils.o: utils.c defs.h
	$(CC) $(OPT) -c utils.c defs.h	
clean:
	rm -f ${OBJ} ghostHunter *.gch