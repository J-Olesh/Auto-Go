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
    lastMove.x = NULL;
    lastMove.y = NULL;
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
    moveCount = 0;
    white_captures = 0;
    black_captures = 0;
}

bool GoGame::placeStone(sf::Vector2i xy)
{
    /*
    capture logic: when a stone is played:
    for each adjacent enemy stone:
    if the stone check status with rec_hasLiberties(stone):

    */

    char colour = 'B';
    if (!blackTurn)
    {
        colour = 'W';
    }

    BoardPos move(xy.y, xy.x, colour);
    if (board[move.x][move.y].state != ' ')
    {
        // std::cout << "invalid move: space occupied" << std::endl;
        return false;
    }

    board[move.x][move.y] = move;
    // std::cout << "place stone: " << move << std::endl;
    if (move.x == 2 && move.y == 3)
    {
        int f;
    }
    int transforms[4][2] = {{0, 1},
                            {1, 0},
                            {0, -1},
                            {-1, 0}};
    bool capturedSomething = false;

    vector<BoardPos> alreadyChecked = vector<BoardPos>();

    // for each adj space (up, right, down, left)
    for (auto transform : transforms)
    {
        BoardPos *adj = getRelativePos(move, transform[0], transform[1]);

        // if adj is not OOB
        if (adj != nullptr)
        {

            if (alreadyChecked.size() != 0)
            {
                if (std::find(alreadyChecked.begin(), alreadyChecked.end(), *adj) != alreadyChecked.end())
                {
                    continue;
                }
            }
            // std::cout << "checking adj: " << *adj << std::endl;
            //  if adj is enemy
            if ((adj->state != move.state) && (adj->state != ' '))
            {
                // a list of all the enemy stones we checked for liberties
                vector<BoardPos> checkedForLiberties = vector<BoardPos>();

                if (!rec_hasLiberties(checkedForLiberties, adj))
                {
                    capturedSomething = true;
                    if (adj->state == 'B')
                    {
                        white_captures += checkedForLiberties.size();
                        // std::cout << "white captures + " << searched.size();
                    }
                    else
                    {
                        black_captures += checkedForLiberties.size();
                        // std::cout << "black_captures + " << searched.size();
                    }
                    for (auto pos : checkedForLiberties)
                    {
                        board[pos.x][pos.y].state = ' ';
                    }
                }
                else
                {

                    for (auto pos : checkedForLiberties)
                    {
                        if (pos.x == move.x + 1 || pos.x == move.x - 1 || pos.y == move.y + 1 || pos.y == move.y - 1)
                        {
                            alreadyChecked.push_back(pos);
                        }
                    }
                }
            }
        }
    }
    if (!capturedSomething)
    {
        vector<BoardPos> searched = vector<BoardPos>();
        if (!rec_hasLiberties(searched, &move))
        {
            // std::cout << "invalid move: suicide rule" << std::endl;
            board[move.x][move.y].state = ' ';
            return false;
        }
    }
    moveCount++;
    lastMove.x = move.x;
    lastMove.y = move.y;
    blackTurn = !blackTurn;
    // std::cout << "valid move, returning" << std::endl;
    return true;
}

bool GoGame::rec_hasLiberties(
    vector<BoardPos> &searched,
    BoardPos *stone)
{
    // Already visited this stone
    if (std::find(searched.begin(), searched.end(), *stone) != searched.end())
        return false;

    searched.push_back(*stone);

    int transforms[4][2] = {
        {0, 1},
        {1, 0},
        {0, -1},
        {-1, 0}};

    for (auto transform : transforms)
    {
        BoardPos *adjacent =
            getRelativePos(*stone, transform[0], transform[1]);

        if (adjacent == nullptr)
            continue;

        if (adjacent->state == ' ')
        {
            return true;
        }

        if (adjacent->state == stone->state)
        {
            if (rec_hasLiberties(searched, adjacent))
                return true;
        }
    }

    return false;
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
