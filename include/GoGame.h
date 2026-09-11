#ifndef _GoGame
#define _GoGame
#include <SFML/Graphics.hpp>
#include <vector>
#include <ostream>
using std::vector;

struct BoardPos
{
    int x;
    int y;
    char state;

    BoardPos(int _y, int _x, char s)
    {
        x = _x;
        y = _y;
        state = s;
    }
    bool operator==(const BoardPos &pos)
    {
        return (pos.x == x && pos.y == y);
    }
};

class GoGame
{

    BoardPos *getRelativePos(BoardPos pos, int xTransform, int yTransform);
    bool rec_hasLiberties(vector<BoardPos> &searched, BoardPos *stone);
    bool _blackTurn;
    bool _potentialKo;
    sf::Vector2i _lastTurnCapture;

public:
    sf::Vector2i lastMove;
    int whiteCaptures;
    int blackCaptures;
    int moveCount;
    GoGame(int w, int h);
    bool placeStone(sf::Vector2i);

    vector<vector<BoardPos>> board;
    vector<sf::Vector2i> getEmptySpaces();
    int height;
    int width;
};

#endif
