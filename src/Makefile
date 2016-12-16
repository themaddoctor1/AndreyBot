CC = gcc

CFLAGS = -c -Wall -Werror --pedantic -I../include/ -lm -g

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: $(OBJS)
	ar -rc libmylist.a $(OBJS)
	ranlib libmylist.a
	mv libmylist.a ../lib

clean:
	rm -f $(OBJS)

fclean:
	rm -f $(OBJS) ../lib/libmylist.a

re:
	make fclean all
