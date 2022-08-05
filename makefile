CC = gcc -Wall
LIBS = -lm -ldl
MAIN= interprete

SRCS = main.c lex.yy.c abb.c tabla_simbolos.c errores.c gramatica.tab.c estandar.c

DEPS = lex.yy.h abb.h tabla_simbolos.h errores.h gramatica.tab.h estandar.h


OBJS = $(SRCS:.c=.o)

$(MAIN): $(OBJS)
	$(CC) -o $(MAIN) $(OBJS) $(LIBS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

cleanall: clean
	rm -f $(MAIN)

clean:
	rm -f *.o *.~w