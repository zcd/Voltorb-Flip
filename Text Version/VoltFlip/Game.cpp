#include "Human.h"
#include "VFException.h"
#include <iostream>
#include <iomanip>

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