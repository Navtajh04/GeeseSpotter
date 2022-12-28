#include "geesespotter_lib.h"
#include <iostream>

int main();
char *createBoard(std::size_t xdim, std::size_t ydim);
void cleanBoard(char *board);
void printBoard(char *board, std::size_t xdim, std::size_t ydim);
void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim);
void hideBoard(char *board, std::size_t xdim, std::size_t ydim);
int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
           std::size_t yloc);
int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
         std::size_t yloc);
bool isGameWon(char *board, std::size_t xdim, std::size_t ydim);

char *createBoard(std::size_t xdim, std::size_t ydim)
{
    char *board{new char[xdim * ydim]{0}};
    return board;
}

void cleanBoard(char *board)
{
    delete[] board;
    board = nullptr;
}

void printBoard(char *board, std::size_t xdim, std::size_t ydim)
{
    for (std::size_t j{0}; j < (xdim * ydim); ++j)
    {
        if (((j % xdim) == 0) && (j > 0))
        {
            std::cout << std::endl;
        }
        if ((markedBit() & board[j]) == markedBit())
        {
            std::cout << "M";
        }
        else if ((hiddenBit() & board[j]) == hiddenBit())
        {
            std::cout << "*";
        }
        else
        {
            std::cout << std::to_string(board[j]);
        }
    }
}

void hideBoard(char *board, std::size_t xdim, std::size_t ydim)
{
    int mask{32};
    for (std::size_t k{0}; k < (xdim * ydim); ++k)
    {
        board[k] |= mask;
    }
}

int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
         std::size_t yloc)
{
    std::size_t index{xloc + (xdim * yloc)};
    int mask{1};
    if (board[index] & (mask << 5))
    {
        board[index] ^= (mask << 4);
        return 0;
    }
    else
    {
        return 2;
    }
}

void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim)
{
    for (std::size_t i{0}; i < (xdim * ydim); ++i)
    {
        if (board[i] >= 9)
        {
            if ((i % xdim) == 0)
            {
                if (i < xdim)
                {
                    ++board[i + 1];
                    ++board[i + xdim];
                    ++board[i + xdim + 1];
                }
                else if (i >= (xdim * (ydim - 1)))
                {
                    ++board[i + 1];
                    ++board[i - xdim];
                    ++board[i - xdim + 1];
                }
                else
                {
                    ++board[i - xdim];
                    ++board[i - xdim + 1];
                    ++board[i + 1];
                    ++board[i + xdim + 1];
                    ++board[i + xdim];
                }
            }
            else if (((i + 1) % xdim) == 0)
            {
                if (i < xdim)
                {
                    ++board[i - 1];
                    ++board[i + xdim - 1];
                    ++board[i + xdim];
                }
                else if (i >= (xdim * (ydim - 1)))
                {
                    ++board[i - 1];
                    ++board[i - xdim];
                    ++board[i - xdim - 1];
                }
                else
                {
                    ++board[i - xdim];
                    ++board[i - xdim - 1];
                    ++board[i - 1];
                    ++board[i + xdim - 1];
                    ++board[i + xdim];
                }
            }
            else if (i < xdim)
            {
                ++board[i - 1];
                ++board[i + xdim - 1];
                ++board[i + xdim];
                ++board[i + xdim + 1];
                ++board[i + 1];
            }
            else if (i >= (xdim * (ydim - 1)))
            {
                ++board[i - 1];
                ++board[i - xdim - 1];
                ++board[i - xdim];
                ++board[i - xdim + 1];
                ++board[i + 1];
            }
            else
            {
                ++board[i - 1];
                ++board[i - xdim - 1];
                ++board[i - xdim];
                ++board[i - xdim + 1];
                ++board[i + 1];
                ++board[i + xdim + 1];
                ++board[i + xdim];
                ++board[i + xdim - 1];
            }
        }
    }
    for (std::size_t k{0}; k < (xdim * ydim); ++k)
    {
        if (board[k] > 9)
        {
            board[k] = 9;
        }
    }
}

int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
           std::size_t yloc)
{
    unsigned int mask{1};
    std::size_t index{xloc + (xdim * yloc)};
    if (board[index] & (mask << 4))
    {
        return 1;
    }
    else if ((board[index] & (mask << 5)) == 0)
    {
        return 2;
    }
    else if (board[index] == 41)
    {
        board[index] ^= (mask << 5);
        return 9;
    }
    else
    {
        board[index] ^= (mask << 5);
        if (board[index] == 0)
        {
            if ((index % xdim) == 0)
            {
                if (index < xdim)
                {
                    mask = 32;
                    board[index + 1] ^= mask;
                    board[index + xdim] ^= mask;
                    board[index + xdim + 1] ^= mask;
                }
                else if (index >= (xdim * (ydim - 1)))
                {
                    board[index + 1] ^= mask;
                    board[index - xdim] ^= mask;
                    board[index - xdim + 1] ^= mask;
                }
            }
            else if (((index + 1) % xdim == 0))
            {
                if (index < xdim)
                {
                    board[index - 1] ^= mask;
                    board[index + xdim - 1] ^= mask;
                    board[index + xdim] ^= mask;
                }
                else if (index >= (xdim * (ydim - 1)))
                {
                    board[index - 1] ^= mask;
                    board[index - xdim] ^= mask;
                    board[index - xdim - 1] ^= mask;
                }
            }
            else if (index < xdim)
            {
                board[index - 1] ^= mask;
                board[index + xdim - 1] ^= mask;
                board[index + xdim] ^= mask;
                board[index + xdim + 1] ^= mask;
                board[index + 1] ^= mask;
            }
            else if (index >= (xdim * (ydim - 1)))
            {
                board[index - 1] ^= mask;
                board[index - xdim - 1] ^= mask;
                board[index - xdim] ^= mask;
                board[index - xdim + 1] ^= mask;
                board[index + 1] ^= mask;
            }
            else
            {
                board[index - 1] ^= mask;
                board[index - xdim - 1] ^= mask;
                board[index - xdim] ^= mask;
                board[index - xdim + 1] ^= mask;
                board[index + 1] ^= mask;
                board[index + xdim + 1] ^= mask;
                board[index + xdim] ^= mask;
                board[index + xdim - 1] ^= mask;
            }
        }
    }
    return 0;
}

bool isGameWon(char *board, std::size_t xdim, std::size_t ydim)
{
    unsigned int mask{15};
    for (std::size_t k{0}; k < (xdim * ydim); ++k)
    {
        if (((board[k] & mask) < 9) && ((board[k] & hiddenBit()) == hiddenBit()))
        {
            return false;
        }
    }
    return true;
}