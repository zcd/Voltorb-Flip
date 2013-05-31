#include <string>
#include "Move.h"
#include "GameState.h"

#pragma once
class Player
{
protected:
    std::string name;
    int score;
    int level;

public:
    Player(std::string name="Player");
    ~Player(void);

    int get_score(void) const;
    int get_level(void) const;
    std::string get_name(void) const;
    int add_score(int);
    void level_up(void) {level++;}
    void set_level(int);
    virtual Move get_move(GameState*) = 0;
};

