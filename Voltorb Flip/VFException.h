#include <string>
#include "Move.h"
#pragma once

/*
Exception thrown by the GameState class.  This represents something
wrong with the move just made.
*/

class VFException: public std::exception
{
private:
    std::string message;
    Move move;

    friend std::ostream& operator<<(std::ostream&, const VFException&);

public:
    VFException(std::string, Move);
    ~VFException(void);
};