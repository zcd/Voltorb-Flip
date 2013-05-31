#ifndef MOVE_H
#define MOVE_H

/*
The Move class represents a 2-component coordinate at which
a Voltorb Flip move will be made.
*/

class Move
{
private:
    friend std::ostream& operator<<(std::ostream&, const Move&);

public:
    int x;
    int y;

    Move(void);
    Move(int x, int y);
    void print_move();
};

#endif