#include <string>
#include <windows.h>
#include "GameState.h"
#include "Human.h"
#include "VFException.h"

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
    bool markers_1[height][width];
    bool markers_2[height][width];
    bool markers_3[height][width];
    bool markers_v[height][width];

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
    void reveal_board(void);
};