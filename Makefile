OBJS = main.cpp SDL_integration.cpp figure.cpp

COMPILER = x86_64-w64-mingw32-g++

FLAGS = -O3 -Wall #-Wl,-subsystem,windows

#begin with I
INCLUDES =  -IC:/SDL_MinGW/SDL2-2.0.12/x86_64-w64-mingw32/include/SDL2 \
			-IC:/SDL_MinGW/SDL2_image-2.0.5/x86_64-w64-mingw32/include/SDL2

#begin with L
LIBRARIES = -LC:/SDL_MinGW/SDL2-2.0.12/x86_64-w64-mingw32/lib \
			-LC:/SDL_MinGW/SDL2_image-2.0.5/x86_64-w64-mingw32/lib

#begin with l
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image 

all :
	$(COMPILER) $(OBJS) $(FLAGS) $(INCLUDES) $(LIBRARIES) $(LINKER_FLAGS) -o release/SDL_chess
