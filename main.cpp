#include "SDL_integration.h"
#include "figure.h"
#include <iostream>

using namespace std;

SDL_Window *mainWindow;
SDL_Renderer *mainRenderer;

figure chessBoard[8][8];
bool possibleMoves[8][8];

COLOUR oppositeColour(COLOUR colour)
{
    if (colour == COLOUR::WHITE)
        return COLOUR::BLACK;
    else if (colour == COLOUR::BLACK)
        return COLOUR::WHITE;
    else
        return COLOUR::NONE;
}

void createTable()
{
    for (int x = 0; x < 8; x++)
    {
        chessBoard[1][x] = figure(TYPE::PAWN, COLOUR::BLACK, x, 1);
        chessBoard[6][x] = figure(TYPE::PAWN, COLOUR::WHITE, x, 6);
    }

    chessBoard[0][0] = figure(TYPE::ROOK, COLOUR::BLACK, 0, 0);
    chessBoard[0][7] = figure(TYPE::ROOK, COLOUR::BLACK, 7, 0);
    chessBoard[7][0] = figure(TYPE::ROOK, COLOUR::WHITE, 0, 7);
    chessBoard[7][7] = figure(TYPE::ROOK, COLOUR::WHITE, 7, 7);

    chessBoard[0][1] = figure(TYPE::KNIGHT, COLOUR::BLACK, 1, 0);
    chessBoard[0][6] = figure(TYPE::KNIGHT, COLOUR::BLACK, 6, 0);
    chessBoard[7][1] = figure(TYPE::KNIGHT, COLOUR::WHITE, 1, 7);
    chessBoard[7][6] = figure(TYPE::KNIGHT, COLOUR::WHITE, 6, 7);

    chessBoard[0][2] = figure(TYPE::BISHOP, COLOUR::BLACK, 2, 0);
    chessBoard[0][5] = figure(TYPE::BISHOP, COLOUR::BLACK, 5, 0);
    chessBoard[7][2] = figure(TYPE::BISHOP, COLOUR::WHITE, 2, 7);
    chessBoard[7][5] = figure(TYPE::BISHOP, COLOUR::WHITE, 5, 7);

    chessBoard[0][3] = figure(TYPE::QUEEN, COLOUR::BLACK, 3, 0);
    chessBoard[7][3] = figure(TYPE::QUEEN, COLOUR::WHITE, 3, 7);

    chessBoard[0][4] = figure(TYPE::KING, COLOUR::BLACK, 4, 0);
    chessBoard[7][4] = figure(TYPE::KING, COLOUR::WHITE, 4, 7);
}

int memoryStartX, memoryStartY, memoryEndX, memoryEndY;
TYPE memoryStartType, memoryEndType;
COLOUR memoryStartColour, memoryEndColour;
void rememberMove(int startX, int startY, int endX, int endY)
{
    memoryStartX = startX;
    memoryStartY = startY;
    memoryEndX = endX;
    memoryEndY = endY;
    memoryStartColour = chessBoard[startY][startX].getColour();
    memoryEndColour = chessBoard[endY][endX].getColour();
    memoryStartType = chessBoard[startY][startX].getType();
    memoryEndType = chessBoard[endY][endX].getType();
}

void revertLastMove()
{
    chessBoard[memoryStartY][memoryStartX] = figure(memoryStartType, memoryStartColour, memoryStartX, memoryStartY);
    chessBoard[memoryEndY][memoryEndX] = figure(memoryEndType, memoryEndColour, memoryEndX, memoryEndY);
}

bool underAttack(COLOUR colour, int x, int y)
{
    for (int line = 0; line < 8; line++)
        for (int column = 0; column < 8; column++)
        {
            if (chessBoard[line][column].getColour() == colour && chessBoard[line][column].checkMove(x, y))
                return true;
        }

    return false;
}

bool underAttackNoKing(COLOUR colour, int x, int y)
{
    for (int line = 0; line < 8; line++)
        for (int column = 0; column < 8; column++)
        {
            if (chessBoard[line][column].getColour() == colour && chessBoard[line][column].getType() != TYPE::KING && chessBoard[line][column].checkMove(x, y))
                return true;
        }

    return false;
}

bool simpleCheck(COLOUR defenderColour)
{
    COLOUR attackerColour;
    int kingX, kingY;
    for (int line = 0; line < 8; line++)
        for (int column = 0; column < 8; column++)
        {
            if (chessBoard[line][column].getType() == TYPE::KING)
            {
                if (chessBoard[line][column].getColour() == defenderColour)
                {
                    kingX = column;
                    kingY = line;
                }
            }
        }
    if (defenderColour == COLOUR::WHITE)
        attackerColour = COLOUR::BLACK;
    else
        attackerColour = COLOUR::WHITE;
    if (underAttack(attackerColour, kingX, kingY))
        return true;

    return false;
}

bool checkMate(COLOUR defenderColour)
{
    int kingX, kingY;
    COLOUR attackerColour;
    for (int line = 0; line < 8; line++)
        for (int column = 0; column < 8; column++)
        {
            if (chessBoard[line][column].getType() == TYPE::KING && chessBoard[line][column].getColour() == defenderColour)
            {
                kingX = column;
                kingY = line;
                if (defenderColour == COLOUR::WHITE)
                    attackerColour = COLOUR::BLACK;
                else
                    attackerColour = COLOUR::WHITE;
            }
        }

    if (underAttack(attackerColour, kingX, kingY))
    {
        for (int moveY = -1; moveY < 2; moveY++)
            for (int moveX = -1; moveX < 2; moveX++)
            {
                rememberMove(kingX, kingY, kingX + moveX, kingY + moveY);
                if (chessBoard[kingY][kingX].move(kingX + moveX, kingY + moveY))
                {
                    if (!underAttack(attackerColour, kingX + moveX, kingY + moveY))
                    {
                        revertLastMove();
                        return false;
                    }
                    revertLastMove();
                }
            }
        int attackerX = -1, attackerY = -1;
        for (int line = 0; line < 8; line++)
            for (int column = 0; column < 8; column++)
            {
                if (chessBoard[line][column].checkMove(kingX, kingY))
                {
                    if (attackerX != -1)
                        return true;
                    else
                    {
                        attackerX = column;
                        attackerY = line;
                    }
                }
            }
        if (chessBoard[attackerY][attackerX].getType() == TYPE::QUEEN ||
            chessBoard[attackerY][attackerX].getType() == TYPE::ROOK ||
            chessBoard[attackerY][attackerX].getType() == TYPE::BISHOP)
        {
            int deltaX = kingX - attackerX;
            int deltaY = kingY - attackerY;
            int stepX, stepY;
            if (deltaX == 0)
                stepX = 0;
            else
                stepX = deltaX / abs(deltaX);
            if (deltaY == 0)
                stepY = 0;
            else
                stepY = deltaY / abs(deltaY);
            while (attackerX != kingX || attackerY != kingY)
            {
                if (underAttackNoKing(defenderColour, attackerX, attackerY))
                    return false;

                attackerY += stepY;
                attackerX += stepX;
            }
            return true;
        }
        else if (underAttack(defenderColour, attackerX, attackerY))
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    return false;
}

void markPossibilities(int x, int y)
{
    for (int line = 0; line < 8; line++)
        for (int column = 0; column < 8; column++)
            if (chessBoard[y][x].checkMove(column, line))
                possibleMoves[line][column] = true;
            else
                possibleMoves[line][column] = false;
}

void clearMarks()
{
    for (int line = 0; line < 8; line++)
        for (int column = 0; column < 8; column++)
            possibleMoves[line][column] = false;
}

int main(int argc, char *argv[])
{
    init();
    createWindowAndRenderer();
    SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);
    SDL_Event e;
    SDL_Texture *table = loadTexture((string)(SDL_GetBasePath()) + "assets\\chess_board1.png");
    SDL_Texture *mark = loadTexture((string)(SDL_GetBasePath()) + "assets\\chess_mark.png");
    SDL_Texture *blackWin = loadTexture((string)(SDL_GetBasePath()) + "assets\\chess_black_win.png");
    SDL_Texture *whiteWin = loadTexture((string)(SDL_GetBasePath()) + "assets\\chess_white_win.png");
    SDL_Rect tableRect;
    tableRect.x = 0;
    tableRect.y = 0;
    tableRect.w = 640;
    tableRect.h = 640;
    bool quit = false;

    createTable();
    bool firstPositionSelected = false;
    int lastX, lastY;
    COLOUR currentTurn = COLOUR::WHITE;
    COLOUR kingAttacked = COLOUR::NONE;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
                quit = true;
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                x /= 80;
                y /= 80;

                if (firstPositionSelected)
                {
                    if (x != lastX || y != lastY)
                    {
                        if (chessBoard[lastY][lastX].checkMove(x, y))
                        {
                            if (kingAttacked == currentTurn)
                            {
                                rememberMove(lastX, lastY, x, y);
                                chessBoard[lastY][lastX].move(x, y);
                                if (simpleCheck(kingAttacked))
                                    revertLastMove();
                                else
                                    kingAttacked = COLOUR::NONE;
                                clearMarks();
                                firstPositionSelected = false;
                            }
                            if (kingAttacked == COLOUR::NONE)
                            {
                                rememberMove(lastX, lastY, x, y);
                                chessBoard[lastY][lastX].move(x, y);
                                currentTurn = oppositeColour(currentTurn);
                                clearMarks();
                                firstPositionSelected = false;
                                if (simpleCheck(oppositeColour(currentTurn)))
                                {
                                    revertLastMove();
                                    currentTurn = oppositeColour(currentTurn);
                                }
                                else if (checkMate(currentTurn))
                                {
                                    quit = true;
                                    kingAttacked = currentTurn;
                                }
                                else if (simpleCheck(currentTurn))
                                {
                                    kingAttacked = currentTurn;
                                }
                            }
                        }
                        else
                        {
                            clearMarks();
                            firstPositionSelected = false;
                        }
                    }
                }
                if (chessBoard[y][x].getColour() == currentTurn && !firstPositionSelected)
                {
                    lastX = x;
                    lastY = y;
                    markPossibilities(x, y);
                    firstPositionSelected = true;
                }
            }
        }

        SDL_RenderClear(mainRenderer);
        SDL_RenderCopy(mainRenderer, table, NULL, &tableRect);
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++)
            {
                chessBoard[y][x].render();
                if (possibleMoves[y][x])
                {
                    SDL_Rect temp{.x = x * 80, .y = y * 80, .w = 80, .h = 80};
                    SDL_RenderCopy(mainRenderer, mark, NULL, &temp);
                }
            }

        if (quit)
        {
            if (kingAttacked == COLOUR::BLACK)
                SDL_RenderCopy(mainRenderer, whiteWin, NULL, NULL);
            else
                SDL_RenderCopy(mainRenderer, blackWin, NULL, NULL);
            SDL_RenderPresent(mainRenderer);
            SDL_Delay(5000);
        }
        SDL_RenderPresent(mainRenderer);
    }

    SDL_Quit();
    return 0;
}