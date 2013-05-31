#include <list>
#include "Move.h"
#pragma once

const int One = 1;
const int Two = 2;
const int Three = 3;
const int Voltorb = -1;
const int Null = 0;

const int height = 5;
const int width = 5;
const int candidates[7][4] = {{24, 27, 36},         // 1
                              {54, 64, 81},         // 2
                              {108, 128, 144, 162}, // 3
                              {288, 324},           // 4
                              {432, 486, 576},      // 5
                              {729, 972},           // 6
                              {1458}};              // 7

class GameState
{
private:
    int level;
    int points;
    int total_points;
    int cards_flipped;

    struct board_cell
    {
        int points;
        int marker;
        bool flipped;
    };

    struct info
    {
        int points_sum;
        int voltorbs_sum;
    };

    std::list<int> multipliers;
    board_cell board[height][width];
    info col[width];
    info row[height];
    void setup_multipliers(int);

public:
    GameState(int level=1);
    ~GameState(void);
    void disp_board(void);
    void debug_state(void);
    void setup_board(void);
    bool do_move(Move m);
    bool level_done(void);
    int get_points(void) const;
    int get_level(void) const;
    int get_cards_flipped(void) const;
};