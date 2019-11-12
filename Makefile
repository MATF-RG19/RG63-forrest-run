PROGRAM = forest
CC      = gcc
LDLIBS  = -framework GLUT -framework OpenGL

$(PROGRAM): main.o
	$(CC) -o $(PROGRAM) main.o $(LDLIBS)