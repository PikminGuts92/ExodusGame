// Created and developed by Francisco Martinez
// Initial Release: Nov. 22, 2015
// Last Updated: Jan. 20, 2016

#include <iostream>
#include "Exodus.h"

using namespace std;

int main()
{
    int nSize = -1; // Increase up to 4 for local multi-player action!
    string names[4];

    cout << "ExODUs Game v1.0" << endl << endl; // Displays title

    do
    {
        // Gets player count
        cout << "Enter number of players (0-4): ";
        cin >> nSize;
    } while (nSize < 0 || nSize > 4);

    for (int i = 0; i < nSize; i++)
    {
        // Gets name for each player
        cout << "Enter player " << (i + 1) << "'s name: ";
        cin >> names[i];
    }

    // Starts the actual game
    Exodus game = Exodus(names, nSize);
    game.Play();

    return 0;
}
