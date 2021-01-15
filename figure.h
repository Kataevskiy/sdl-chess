#ifndef FIGURE_H
#define FIGURE_H

#include <SDL.h>

enum class TYPE
{
    KING,
    QUEEN,
    BISHOP,
    ROOK,
    KNIGHT,
    PAWN,
    NONE
};

enum class COLOUR
{
    WHITE,
    BLACK,
    NONE
};

class figure
{
private:
    TYPE type;
    COLOUR colour;
    int x, y;
    SDL_Texture *texture;

public:
    figure();

    figure(TYPE type, COLOUR colour, int x, int y);

    figure &operator=(figure &&another);

    ~figure();

    bool checkMove(int x, int y);

    bool move(int x, int y);

    COLOUR getColour();

    TYPE getType();

    void render();
};

#endif