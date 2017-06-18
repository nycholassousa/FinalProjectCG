CC 		 = g++
INCLUDES = includes/
FILES 	 = $(INCLUDES)main.cpp $(INCLUDES)game.cpp $(INCLUDES)gamedrawer.cpp $(INCLUDES)imageloader.cpp $(INCLUDES)md2model.cpp $(INCLUDES)text3d.cpp $(INCLUDES)vec3f.cpp
LINKERS  = -lglut -lGLU -lGL -I. -std=c++11 -o
PROGRAM  = cgprog

all:
	$(CC) $(FILES) $(LINKERS) $(PROGRAM)
	./$(PROGRAM)

clean:
	rm $(PROGRAM)