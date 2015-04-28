CIBLE = main
SRCS =  Main.cpp Camera.cpp Mesh.cpp GLProgram.cpp render.cpp Matrices.cpp
OPENGL_PATH = /usr/lib/nvidia-331/ # change this for your own environment
LIBS = -L$(OPENGL_PATH) -lglut -lGLU -lGL -lGLEW -lm -pthread
#INCLUDE = ~/Documents/stage de recherche Telecom/TP_Shaders/glm

CC = g+
CPP = g++

FLAGS = -Wall -O2 -g #-L/usr/lib/nvidia-331/lib 

CFLAGS = $(FLAGS)
CXXFLAGS = $(FLAGS)

OBJS = $(SRCS:.cpp=.o)   

$(CIBLE): $(OBJS)
	g++ $(LDFLAGS) -o $(CIBLE) $(OBJS) $(LIBS) # -I$(INCLUDE)
clean:
	rm -f  *~  $(CIBLE) $(OBJS)

Matrices.o: Matrices.cpp Matrices.h Vectors.h
Camera.o: Camera.cpp Camera.h Vec3.h
Mesh.o: Mesh.cpp Mesh.h Vec3.h
GLProgram.o: GLProgram.cpp GLProgram.h
render.o: render.cpp render.h GLProgram.h Matrices.h
Main.o: Main.cpp Vec3.h Camera.h Mesh.h GLProgram.h render.h



