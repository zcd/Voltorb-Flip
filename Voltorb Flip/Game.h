#include "Human.h"
#include "VFException.h"
#include <string>
#include <windows.h>

/*
The Game class manages the relationship between GameState and Player.  This clas
also serves as a liaison between game internals and the user interface.
*/

class Game
{
private:
    GameState* state;
    Player *player;
    int level; // level to display to the user
    bool win;

public:
    Game(std::string name="Player");
    ~Game();

    bool handle_input(int, int);
    bool refresh_game_state(void);
    int get_level(void) const;
    int get_round_score(void) const;
    int get_player_score(void) const;
    board_cell get_board_state(int, int) const;
    info get_row_info(int) const;
    info get_col_info(int) const;
};