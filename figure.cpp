#include "figure.h"
#include "SDL_integration.h"

extern SDL_Renderer *mainRenderer;
extern figure chessBoard[8][8];

SDL_Texture *setTexture(TYPE type, COLOUR colour)
{
    if (type == TYPE::NONE || colour == COLOUR::NONE)
        return NULL;

    std::string path(SDL_GetBasePath());
    path += "assets\\chess_";

    switch (type)
    {
    case TYPE::KING:
        path += "king_";
        break;
    case TYPE::QUEEN:
        path += "queen_";
        break;
    case TYPE::BISHOP:
        path += "bishop_";
        break;
    case TYPE::ROOK:
        path += "rook_";
        break;
    case TYPE::KNIGHT:
        path += "knight_";
        break;
    case TYPE::PAWN:
        path += "pawn_";
        break;
    default:
        path += "pawn_";
        break;
    }
    switch (colour)
    {
    case COLOUR::WHITE:
        path += "white.png";
        break;
    case COLOUR::BLACK:
        path += "black.png";
        break;
    default:
        path += "white";
        break;
    }

    return loadTexture(path);
}

figure::figure()
{
    x = 1;
    y = 1;
    type = TYPE::NONE;
    colour = COLOUR::NONE;
    texture = NULL;
}

figure::figure(TYPE type, COLOUR colour, int x, int y)
{
    this->type = type;
    this->colour = colour;
    this->x = x;
    this->y = y;
    this->texture = setTexture(type, colour);
}

figure &figure::operator=(figure &&another)
{
    this->x = another.x;
    this->y = another.y;
    this->colour = another.colour;
    this->type = another.type;
    this->texture = another.texture;
    another.texture = NULL;
    return *this;
}

figure::~figure()
{
    if (texture != NULL)
        SDL_DestroyTexture(texture);
    texture = NULL;
}

void figure::render()
{
    if (texture != NULL)
    {
        SDL_Rect rect{.x = x * 80, .y = y * 80, .w = 80, .h = 80};
        SDL_RenderCopy(mainRenderer, texture, NULL, &rect);
    }
}

COLOUR figure::getColour()
{
    return this->colour;
}

TYPE figure::getType()
{
    return this->type;
}

bool checkMoveKing(int startX, int startY, int endX, int endY)
{
    int deltaX = endX - startX;
    int deltaY = endY - startY;
    if (endX > -1 && endX < 8 && endY > -1 && endY < 8)
        if (abs(deltaX) < 2 && abs(deltaY) < 2)
            if (chessBoard[endY][endX].getColour() != chessBoard[startY][startX].getColour())
                return true;

    return false;
}

bool checkMoveQueen(int startX, int startY, int endX, int endY)
{
    int deltaX = endX - startX;
    int deltaY = endY - startY;
    if (endX > -1 && endX < 8 && endY > -1 && endY < 8)
        if (abs(deltaX) == abs(deltaY) || (deltaX == 0) != (deltaY == 0))
        {
            int stepX, stepY;
            if (deltaX == 0)
                stepX = 0;
            else
                stepX = deltaX / abs(deltaX);
            if (deltaY == 0)
                stepY = 0;
            else
                stepY = deltaY / abs(deltaY);

            int positionX = startX + stepX;
            int positionY = startY + stepY;
            while (positionX != endX || positionY != endY)
            {
                if (chessBoard[positionY][positionX].getColour() != COLOUR::NONE)
                    return false;
                positionX += stepX;
                positionY += stepY;
            }
            if (chessBoard[endY][endX].getColour() != chessBoard[startY][startX].getColour())
                return true;
        }

    return false;
}

bool checkMoveRook(int startX, int startY, int endX, int endY)
{
    int deltaX = endX - startX;
    int deltaY = endY - startY;
    if (endX > -1 && endX < 8 && endY > -1 && endY < 8)
        if ((deltaX == 0) != (deltaY == 0))
        {
            int stepX, stepY;
            if (deltaX == 0)
                stepX = 0;
            else
                stepX = deltaX / abs(deltaX);
            if (deltaY == 0)
                stepY = 0;
            else
                stepY = deltaY / abs(deltaY);

            int positionX = startX + stepX;
            int positionY = startY + stepY;
            while (positionX != endX || positionY != endY)
            {
                if (chessBoard[positionY][positionX].getColour() != COLOUR::NONE)
                    return false;
                positionX += stepX;
                positionY += stepY;
            }
            if (chessBoard[endY][endX].getColour() != chessBoard[startY][startX].getColour())
                return true;
        }

    return false;
}

bool checkMoveBishop(int startX, int startY, int endX, int endY)
{
    int deltaX = endX - startX;
    int deltaY = endY - startY;
    if (endX > -1 && endX < 8 && endY > -1 && endY < 8)
        if (abs(deltaX) == abs(deltaY))
        {
            int stepX, stepY;
            if (deltaX == 0)
                stepX = 0;
            else
                stepX = deltaX / abs(deltaX);
            if (deltaY == 0)
                stepY = 0;
            else
                stepY = deltaY / abs(deltaY);

            int positionX = startX + stepX;
            int positionY = startY + stepY;
            while (positionX != endX || positionY != endY)
            {
                if (chessBoard[positionY][positionX].getColour() != COLOUR::NONE)
                    return false;
                positionX += stepX;
                positionY += stepY;
            }
            if (chessBoard[endY][endX].getColour() != chessBoard[startY][startX].getColour())
                return true;
        }

    return false;
}

bool checkMoveKnight(int startX, int startY, int endX, int endY)
{
    int deltaX = endX - startX;
    int deltaY = endY - startY;
    if (endX > -1 && endX < 8 && endY > -1 && endY < 8)
        if ((abs(deltaX) == 1 && abs(deltaY) == 2) || (abs(deltaX) == 2 && abs(deltaY) == 1))
            if (chessBoard[endY][endX].getColour() != chessBoard[startY][startX].getColour())
                return true;

    return false;
}

bool checkMovePawn(int startX, int startY, int endX, int endY)
{
    int deltaX = endX - startX;
    int deltaY = endY - startY;
    if (endX > -1 && endX < 8 && endY > -1 && endY < 8)
    {
        if (deltaX == 0 && abs(deltaY) == 1)
        {
            if (chessBoard[startY][startX].getColour() == COLOUR::WHITE && deltaY == -1)
            {
                if (chessBoard[endY][endX].getColour() == COLOUR::NONE)
                    return true;
            }
            else if (chessBoard[startY][startX].getColour() == COLOUR::BLACK && deltaY == 1)
                if (chessBoard[endY][endX].getColour() == COLOUR::NONE)
                    return true;
        }
        else if (deltaX == 0 && abs(deltaY) == 2)
        {
            if (startY == 1 && chessBoard[startY][startX].getColour() == COLOUR::BLACK)
            {
                if (chessBoard[2][startX].getColour() == COLOUR::NONE && chessBoard[3][startX].getColour() == COLOUR::NONE)
                    return true;
            }
            else if (startY == 6 && chessBoard[startY][startX].getColour() == COLOUR::WHITE)
            {
                if (chessBoard[5][startX].getColour() == COLOUR::NONE && chessBoard[4][startX].getColour() == COLOUR::NONE)
                    return true;
            }
        }
        else if (abs(deltaX) == 1 && abs(deltaY) == 1)
        {
            if (chessBoard[startY][startX].getColour() == COLOUR::WHITE && deltaY == -1)
            {
                if (chessBoard[endY][endX].getColour() == COLOUR::BLACK)
                    return true;
            }
            else if (chessBoard[startY][startX].getColour() == COLOUR::BLACK && deltaY == 1)
            {
                if (chessBoard[endY][endX].getColour() == COLOUR::WHITE)
                    return true;
            }
        }
    }

    return false;
}

bool figure::checkMove(int posX, int posY)
{
    switch (this->type)
    {
    case TYPE::KING:
        return checkMoveKing(x, y, posX, posY);
        break;

    case TYPE::QUEEN:
        return checkMoveQueen(x, y, posX, posY);
        break;

    case TYPE::ROOK:
        return checkMoveRook(x, y, posX, posY);
        break;

    case TYPE::BISHOP:
        return checkMoveBishop(x, y, posX, posY);
        break;

    case TYPE::KNIGHT:
        return checkMoveKnight(x, y, posX, posY);
        break;

    case TYPE::PAWN:
        return checkMovePawn(x, y, posX, posY);
        break;

    default:
        return false;
        break;
    }
}

bool figure::move(int x, int y)
{
    if (checkMove(x, y))
    {
        int tempX = this->x;
        int tempY = this->y;
        chessBoard[y][x] = figure(this->type, this->colour, x, y);
        chessBoard[tempY][tempX] = figure();
        return true;
    }
    else
        return false;
}