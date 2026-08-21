#include "GoGame.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
std::ostream &operator<<(std::ostream &stream, const BoardPos &pos)
{
    stream << "x: " << pos.x << ", y: " << pos.y << ", state: " << pos.state;
    return stream;
}
BoardPos *GoGame::getRelativePos(BoardPos pos, int xTransform, int yTransform)
{
    int newX = pos.x + xTransform;
    int newY = pos.y + yTransform;
    if (newX < 0 || newY < 0)
    {
        return nullptr;
    }
    if (newX > width - 1 || newY > height - 1)
    {

        return nullptr;
    }
    return &board[newX][newY];
}

GoGame::GoGame(int w, int h)
{
    width = w;
    height = h;
    blackTurn = true;
    // init board
    for (int x = 0; x < width; x++)
    {
        vector<BoardPos> b;
        board.push_back(b);
        for (int y = 0; y < height; y++)
        {
            BoardPos pos(y, x, ' ');
            board[x].push_back(pos);
        }
    }

    white_captures = 0;
    black_captures = 0;
}

bool GoGame::placeStone(sf::Vector2i xy)
{

    char colour = 'B';
    if (!blackTurn)
    {
        colour = 'W';
    }

    BoardPos move(xy.y, xy.x, colour);
    if (board[move.x][move.y].state != ' ')
    {
        std::cout << "invalid move: space occupied" << std::endl;
        return -1;
    }

    board[move.x][move.y] = move;
    std::cout << "place stone: " << move << std::endl;
    int transforms[4][2] = {{0, 1},
                            {1, 0},
                            {0, -1},
                            {-1, 0}};
    bool capturedSomething = false;
    // for each adj space (up, left, down, right)
    for (auto transform : transforms)
    {
        BoardPos *p = getRelativePos(move, transform[0], transform[1]);
        // if adj is not OOB
        if (p != nullptr)
        {
            std::cout << "checking adj: " << *p << std::endl;
            // if connected to a friend
            if ((p->state != move.state) && (p->state != ' '))
            {
                std::cout << "found adj from: " << *p << std::endl;
                // inefficient: potentially checks same group 4 times.
                vector<BoardPos> searched = vector<BoardPos>();

                if (!rec_hasLiberties(searched, p))
                {
                    capturedSomething = true;
                    if (p->state == 'B')
                    {
                        white_captures += searched.size();
                    }
                    else
                    {
                        black_captures += searched.size();
                    }
                    for (auto pos : searched)
                    {
                        board[pos.x][pos.y].state = ' ';
                    }
                }
            }
        }
        else
        {
            std::cout << "skipped adj check: OOB" << std::endl;
        }
    }
    if (!capturedSomething)
    {
        vector<BoardPos> searched = vector<BoardPos>();
        if (!rec_hasLiberties(searched, &move))
        {
            std::cout << "invalid move: suicide rule" << std::endl;
            board[move.x][move.y].state = ' ';
            return -1;
        }
    }
    blackTurn = !blackTurn;
    std::cout << "valid move, returning" << std::endl;
    return 0;
}

bool GoGame::rec_hasLiberties(vector<BoardPos> &searched, BoardPos *stone)
{
    std::cout << *stone << std::endl;
    bool res = false;

    int transforms[4][2] = {{0, 1},
                            {1, 0},
                            {0, -1},
                            {-1, 0}};

    for (auto transform : transforms)
    {
        BoardPos *adjacent = getRelativePos(*stone, transform[0], transform[1]);
        // std::cout << "checking: " << *adjacent << std::endl;
        if (adjacent != nullptr)
        {
            if (std::find(searched.begin(), searched.end(), *adjacent) == searched.end())
            {
                searched.push_back(*stone);

                if (adjacent->state == ' ')
                {
                    res = true;
                    std::cout << "space empty" << std::endl;
                }
                else if (adjacent->state == stone->state)
                {
                    res = rec_hasLiberties(searched, adjacent);
                }
            }
        }
        else
        {
            std::cout << "[rec] skipped adj check: OOB" << std::endl;
        }
    }
    return res;
}

vector<sf::Vector2i> GoGame::getEmptySpaces()
{
    vector<sf::Vector2i> res;
    for (auto row : board)
    {
        for (auto space : row)
        {
            if (space.state == ' ')
            {
                res.push_back({space.x, space.y});
            }
        }
    }
    return res;
}