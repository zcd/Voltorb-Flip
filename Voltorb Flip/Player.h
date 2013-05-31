#include <string>
#include "Move.h"
#include "GameState.h"

#pragma once

/* 
The Player base class holds information about the current player.
This class holds the point count across a single session of Voltorb Flip
and the player's current level.

Each Player must implement a get_move() method which determines the next
move to take given the current state of the game.
*/

class Player
{
protected:
    std::string name;
    int score;
    int level;

public:
    Player(void);
    ~Player(void);

    int get_score(void) const;
    int get_level(void) const;
    std::string get_name(void) const;
    int add_score(int);
    void level_up(void);
    void set_level(int);
    virtual Move get_move(GameState*)=0;
};

