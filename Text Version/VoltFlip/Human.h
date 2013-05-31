#pragma once
#include "player.h"
class Human :
    public Player
{
public:
    Human(std::string);
    ~Human(void);
    Move get_move(GameState*);
};

