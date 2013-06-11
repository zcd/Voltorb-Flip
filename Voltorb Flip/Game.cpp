#include <iostream>
#include <iomanip>
#include <sstream>
#include "Game.h"

Game::Game(std::string name)
{
    this->player = new Human(name);
    this->level = player->get_level();
    this->state = new GameState();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            markers_1[i][j] = false;
            markers_2[i][j] = false;
            markers_3[i][j] = false;
            markers_v[i][j] = false;
        }
    }
}

Game::~Game(void)
{
}

/*
Given user input from the UI, this method sends modification details down
to the GameState and Player classes. Returns true if and only if the state 
should be refreshed.
*/
bool Game::handle_input(int i, int j)
{
    Move move(i, j);
    try {
        bool lost = state->do_move(move);
        if (lost) {
            player->set_level(max(1, min(state->get_cards_flipped(), player->get_level())));
            this->win = false;
            reveal_board();
            return true;
        } else if (state->level_done()) {
            if (player->get_level() < 7)
                player->level_up();
            this->win = true;
            reveal_board();
            return true;
        }
    } catch (VFException e) {
    }
    return false;
}

/*
Completely deletes the state object and creates a new one of the next round's
level.  Returns true if and only if the last refresh was for a winning move.
*/
bool Game::refresh_game_state(void)
{
    player->add_score(state->get_points());
    delete this->state;
    this->state = new GameState(player->get_level());
    this->level = player->get_level();
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            markers_1[i][j] = false;
            markers_2[i][j] = false;
            markers_3[i][j] = false;
            markers_v[i][j] = false;
        }
    }
    return win;
}

int Game::get_level(void) const
{
    return this->level;
}

int Game::get_round_score(void) const
{
    return this->state->get_points();
}

int Game::get_player_score(void) const 
{
    return this->player->get_score();
}

board_cell Game::get_board_state(int i, int j) const
{
    return this->state->board[i][j];
}

info Game::get_row_info(int i) const
{
    return this->state->col[i];
}

info Game::get_col_info(int j) const
{
    return this->state->row[j];
}

void Game::reveal_board(void)
{
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            state->board[i][j].flipped = true;
}

/*
Used only in the text version.
*/
void refresh_screen(Player* player, GameState* state)
{
    using namespace std;

    system("cls");

    cout << endl;
    state->disp_board();
    cout << "\nLevel: " << player->get_level() 
         << "\nPoints: " << setfill('0') << setw(4) << state->get_points() 
         << "\nScore: " << setfill('0') << setw(6) << player->get_score() << endl;
}

int main()
{
    using namespace std;
    string name;
    bool lose, game_loop = true, get_move_loop = true, restart_flag = false;
    GameState *state;
    Move m;
    
    cout << "Enter your name: ";
    getline(cin, name);
    cout << endl;
    Human player = Human(name);
    player.set_level(1);
    
    while(game_loop) {
        state = new GameState(player.get_level());
        if (restart_flag) {
            get_move_loop = true;
            restart_flag = false;
            cout << "Restarting at level " << player.get_level() << endl;
        }

        do {
            refresh_screen(&player, state);
            m = player.get_move(state);
            try {
                lose = state->do_move(m);

                if (lose) {
                    refresh_screen(&player, state);
                    cout << "Game over! " << player.get_name() << " scored " << player.get_score() << endl;
                    cout << "Enter \'q\' to exit or any other key to restart: ";
                    char c;
                    cin.get(c);
                    
                    get_move_loop = false;
                    if (c == 'q')
                        game_loop = false;
                    else {
                        restart_flag = true;
                        player.set_level(max(1, min(state->get_cards_flipped(), player.get_level())));
                        delete state;
                    }
                } else if (state->level_done()) {
                    player.add_score(state->get_points());
                    if (player.get_level() < 7)
                        player.level_up();
                    delete state;
                    get_move_loop = false;
                }
            } catch (VFException e) {
                cout << e;
            }
        } while (get_move_loop);
    }
}