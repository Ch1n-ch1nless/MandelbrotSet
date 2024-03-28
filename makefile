CC=g++
CFLAGS=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal\
       -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs \
       -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions\
	   -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security\
	   -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor\
	   -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing\
	   -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla\
	   -D_DEBUG -DSHOW_TIME -mavx2 -mfma -mavx -msse4.2

SRC_DIR = ./source/
OBJ_DIR = ./object/

SRC = $(wildcard $(SRC_DIR)*.cpp)
OBJ = $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SRC))

all: with_simd_O3

with_simd_O3:
	$(CC) $(CFLAGS) -O3 -c ./source/simd_realization.cpp -o ./object/simd_realization.o
	$(CC) $(CFLAGS) -O3 -c ./source/drawing_lib.cpp -o ./object/drawing_lib.o
	$(CC) ./object/drawing_lib.o ./object/simd_realization.o -o mandelbrot_with_simd_O3 -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

with_simd_O0:
	$(CC) $(CFLAGS) -O0 -c ./source/simd_realization.cpp -o ./object/simd_realization.o
	$(CC) $(CFLAGS) -O0 -c ./source/drawing_lib.cpp -o ./object/drawing_lib.o
	$(CC) ./object/drawing_lib.o ./object/simd_realization.o -o mandelbrot_with_simd_O0 -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

with_fake_simd_O3:
	$(CC) $(CFLAGS) -O3 -c ./source/8_point_realization.cpp -o ./object/8_point_realization.o
	$(CC) $(CFLAGS) -O3 -c ./source/drawing_lib.cpp -o ./object/drawing_lib.o
	$(CC) ./object/drawing_lib.o ./object/8_point_realization.o -o mandelbrot_with_fake_simd_O3 -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

with_fake_simd_O0:
	$(CC) $(CFLAGS) -O0 -c ./source/8_point_realization.cpp -o ./object/8_point_realization.o
	$(CC) $(CFLAGS) -O0 -c ./source/drawing_lib.cpp -o ./object/drawing_lib.o
	$(CC) ./object/drawing_lib.o ./object/8_point_realization.o -o mandelbrot_with_fake_simd_O0 -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

without_simd_O3:
	$(CC) $(CFLAGS) -O3 -c ./source/simple_realization.cpp -o ./object/simple_realization.o
	$(CC) $(CFLAGS) -O3 -c ./source/drawing_lib.cpp -o ./object/drawing_lib.o
	$(CC) ./object/drawing_lib.o ./object/simple_realization.o -o mandelbrot_without_simd_O3 -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

without_simd_O0:
	$(CC) $(CFLAGS) -O0 -c ./source/simple_realization.cpp -o ./object/simple_realization.o
	$(CC) $(CFLAGS) -O0 -c ./source/drawing_lib.cpp -o ./object/drawing_lib.o
	$(CC) ./object/drawing_lib.o ./object/simple_realization.o -o mandelbrot_without_simd_O0 -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm $(OBJ)