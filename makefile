CC=g++
CFLAGS=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal\
       -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs \
       -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions\
	   -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security\
	   -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor\
	   -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing\
	   -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla\
	   -D_DEBUG -mavx2 -mfma -mavx -msse4.2


SRC_DIR = ./source/
OBJ_DIR = ./object/

SRC = $(wildcard $(SRC_DIR)*.cpp)
OBJ = $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SRC))

all: 

intrinsic:
	$(CC) $(CFLAGS) $(OPT_LEVEL) -c ./source/intrinsic_realization.cpp -o ./object/intrinsic_realization.o
	$(CC) $(CFLAGS) $(OPT_LEVEL) -c ./source/drawing_lib.cpp -o ./object/drawing_lib.o
	$(CC) ./object/drawing_lib.o ./object/intrinsic_realization.o -o mandelbrot_intrinsic -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

vector:
	$(CC) $(CFLAGS) $(OPT_LEVEL) -c ./source/vector_realization.cpp -o ./object/vector_realization.o
	$(CC) $(CFLAGS) $(OPT_LEVEL) -c ./source/drawing_lib.cpp -o ./object/drawing_lib.o
	$(CC) ./object/drawing_lib.o ./object/vector_realization.o -o mandelbrot_vector -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

simple:
	$(CC) $(CFLAGS) $(OPT_LEVEL) -c ./source/simple_realization.cpp -o ./object/simple_realization.o
	$(CC) $(CFLAGS) $(OPT_LEVEL) -c ./source/drawing_lib.cpp -o ./object/drawing_lib.o
	$(CC) ./object/drawing_lib.o ./object/simple_realization.o -o mandelbrot_simple -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm $(OBJ)