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

all: link

link: $(OBJ)
	@$(CC) $(OBJ) -o mandelbrot_set -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp
	@$(CC) $(CFLAGS) $(OPT_LEVEL) -c $< -o $@

clean:
	@rm $(OBJ)