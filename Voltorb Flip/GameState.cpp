#include <iostream>
#include <iomanip>
#include <time.h>
#include "GameState.h"

/*
The GameState class constructor creates a GameState object of a given level.
This constructor implicitly expects the level to be in the range 0 < level <= 7.
*/
GameState::GameState(int level)
{
    this->level = level;
    this->points = 0;
    this->cards_flipped = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            board[i][j].points = One; //(i*j)%5 -1;
            board[i][j].flipped = false;
            board[i][j].marker = Null;
        }
    }

    setup_board();

    for (int i = 0; i < height; i++) {
        row[i].voltorbs_sum = 0;
        row[i].points_sum = 0;
    }
    for (int j = 0; j < width; j++) {
        col[j].voltorbs_sum = 0;
        col[j].points_sum = 0;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (board[i][j].points == Voltorb) {
                row[j].voltorbs_sum++;
                col[i].voltorbs_sum++;
            } else {
                row[j].points_sum += board[i][j].points;
                col[i].points_sum += board[i][j].points;
            }
        }
    }
}

GameState::~GameState(void)
{
}

/*
Prints the board with column and row counts.  Used for text version only.
*/
void GameState::disp_board(void)
{
    using namespace std;
    const string lower_border = "|----|----|----|----|----|----\n";
    cout << setfill(' ');

    for (int i = 0; i < height; i++) {
        cout << '|';
        for (int j = 0; j < width; j++) {
            cout << "  ";
            if (board[i][j].flipped) {
                switch (board[i][j].points) {
                case Voltorb:
                    cout << "X "; break;
                case One: 
                case Two:
                case Three:
                    cout << board[i][j].points << " "; break;
                }
            } else {
                cout << "? ";
            }
            cout << '|';
        }
        cout << setw(2) << col[i].points_sum << '/' << col[i].voltorbs_sum << endl;
        cout << lower_border;
    }
    cout << '|';
    for (int j = 0; j < width; j++) {
        cout << setw(2) << row[j].points_sum << '/' << row[j].voltorbs_sum << '|';
    }
    cout << endl;
}

/*
Prints the board without column and row information.
*/
void GameState::debug_state(void)
{
    using namespace std;

    for (int i = 0; i < height; i++) {
        cout << "[ ";
        for (int j = 0; j < width; j++) {
            if (board[i][j].flipped) {
                switch (board[i][j].points) {
                case Voltorb:
                    cout << "X "; break;
                case One: 
                case Two:
                case Three:
                    cout << board[i][j].points << " "; break;
                }
            } else {
                cout << "? ";
            }
        }
        cout << "]" << endl;
    }
}

/*
Called in the constructor to assign values to board cells.  Assumes
0 < this->level <= 7.
*/
void GameState::setup_board(void) 
{
    int indices, voltorbs;
    srand ((unsigned int) time(NULL));

    switch(level) {
    case 1:
    case 2:
    case 5:
        indices = 3; break;
    case 3:
        indices = 4; break;
    case 4:
    case 6:
        indices = 2; break;
    case 7:
        indices = 1; break;
    }
    
    total_points = candidates[level-1][rand() % indices];
    //std::cout << "Allocating " << total_points << " points for level " << level << std::endl;

    int i, j, multiplier, p = total_points;
    
    /* set up multiplier cards */
    while (p % 3 == 0 || p % 2 == 0) { 
        if (p % 3 == 0) {
            multiplier = 3;
        } else {
            multiplier = 2;
        }
        p /= multiplier;
        do {
            i = rand() % height;
            j = rand() % width;
        } while (board[i][j].points > 1);
        board[i][j].points = multiplier;
    }

    /* set up voltorbs */
    if (level >= 5) 
        voltorbs = 10;
    else
        voltorbs = level + 4 + rand()%2;
    for (int k = 0; k <= voltorbs; k++) { 
        do {
            i = rand() % height;
            j = rand() % width;
        } while (board[i][j].points > 1);
        board[i][j].points = -1;
    }
}

/*
Attempts to perform move m.  If the move falls out of board
dimensions or has already been done, throws a VFException.  Otherwise
modifies board contents and recalcuates score for the current round.
*/
bool GameState::do_move(Move m)
{
    if (m.x < 0 || m.y < 0 || m.x > height || m.y > width)
        throw VFException("Invalid card location", m);

    if (board[m.x][m.y].flipped)
        throw VFException("Card at location already flipped", m);

    board[m.x][m.y].flipped = true;
    if (board[m.x][m.y].points < 0) {
        points = 0;
        return true;
    } else if (points == 0) {
        points = board[m.x][m.y].points;
    } else {
        points *= board[m.x][m.y].points;
    }
    cards_flipped++;
    return false;
}

/*
Determines of current round is complete.
*/
bool GameState::level_done(void)
{
    return points == total_points;
}

int GameState::get_points(void) const
{
    return points;
}

int GameState::get_level(void) const
{
    return level;
}

int GameState::get_cards_flipped(void) const
{
    return cards_flipped;
}

int main0() {
    GameState *s;
    for (int i = 0; i < 10; i++) {
        std::cout << "Level " << (1+ i % 7) << std::endl;

        s = new GameState(1+ i % 7);
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                s->do_move(Move(i, j));
        s->debug_state();
        delete s;
        std::cout << "\n";
    }
    std::cin.get();
    return 0;
}