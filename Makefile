NAME = wm-class-tool

PREFIX = /usr

CC = gcc
LIBS = -lX11
OBJ = $(NAME).o

%.o: %.c
	$(CC) -c -o $@ $<

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

install: $(NAME)
	cp $< $(DESTDIR)$(PREFIX)/bin/$(NAME)

clean:
	rm -f $(NAME)
	rm -f *.o
