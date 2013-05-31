#include "Player.h"

Player::Player()
{
    score = 0;
    level = 1;
}

Player::~Player(void)
{
}

int Player::get_score(void) const
{
    return score;
}

int Player::get_level(void) const
{
    return level;
}

std::string Player::get_name(void) const
{
    return name;
}

int Player::add_score(int amount){
    score += amount;
    return score;
}

void Player::set_level(int level) {
    this->level = level;
}

void Player::level_up(void) {
    this->level++;
}

/*
Overload me!
*/
//Move Player::get_move(GameState* state)
//{
//    return Move();
//}
