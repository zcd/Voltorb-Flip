#ifndef MOVE_H
#define MOVE_H
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