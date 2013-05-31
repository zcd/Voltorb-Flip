#pragma once
#include "player.h"

/*
The Human class is a Player which performs get_move by accepting
user input through command prompt.
*/

class Human :
    public Player
{
public:
    Human(std::string);
    ~Human(void);
    Move get_move(GameState*);
};

