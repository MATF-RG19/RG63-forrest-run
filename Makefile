PROGRAM = forest
CC      = gcc
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
LDLIBS   = -lGL -lGLU -lglut -lm
endif

ifeq ($(UNAME), Darwin)
LDLIBS   = -framework GLUT -framework OpenGL
endif

$(PROGRAM): main.o
	$(CC) -o $(PROGRAM) main.o $(LDLIBS)