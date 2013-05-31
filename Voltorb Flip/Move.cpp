#include <iostream>
#include "Move.h"

Move::Move()
{
}

Move::Move(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Move::print_move(void) {
    std::cout << "Flip card at " << x << ", " << y << std::endl;
}

/*
Overloaded for cout.
*/
std::ostream& operator<<(std::ostream &strm, const Move &m) {
    return strm << "Move<" << m.x << ", " << m.y << ">" ;
}