
CC = gcc

CFLAGS = -Wall -Werror --pedantic -I../mylibs/include -Iinclude -lm -g

SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:.c=.o)

EXEC=chainfactory.out

all: $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) $(CFLAGS)

clean:
	rm -f $(OBJS)

fclean:
	rm -f $(OBJS) $(EXEC)

re:
	make fclean all
