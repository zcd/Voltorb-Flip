#include <iostream>
#include <string>
#include <regex>
#include "Human.h"

Human::Human(std::string name="Player") : Player()
{
    this->name = name;
}

Human::~Human(void)
{
}

/*
Accepts user input in the form 'i j' and parses it into two ints.
Returns the two ints in the form of a Move object.
*/
Move Human::get_move(GameState* state) 
{
    using namespace std;

    string input;
    const regex r("\\s*(\\d+)\\s+(\\d+)\\s*");
    smatch m;
    int i, j;
    bool success = false;

    while (!success) {
        cout << "Input coordinates: ";
        getline(cin, input);
        success = regex_match(input, m, r);
        if (success) {
            i = atoi(m[1].str().c_str());
            j = atoi(m[2].str().c_str());
        } else {
            cout << "Failed to parse input. ";
        }
    }
    return Move(i, j);
}

int main1(void)
{
    Human h = Human();
    while (true) 
        h.get_move(0);

    std::cin.get();
    return 0;
}