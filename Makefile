CC = clang
OBJ = main.o render.o glad.o
CFLAGS = -gdwarf-4 $(shell pkg-config --cflags glfw3) -Wno-deprecated
LDFLAGS = $(shell pkg-config --libs glfw3) -framework OpenGL

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: main
	./main

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) main