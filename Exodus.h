#ifndef EXODUS_H_INCLUDED
#define EXODUS_H_INCLUDED

#include <iostream>
#include <stdlib.h> // rand
#include <iomanip> // setfill/setw/setprecision
#include <time.h> // time
#include "Player.h"

using namespace std;

class Exodus
{
    public:
        // Constructors
        Exodus(string[], int);

        // Regular Functions
        void Play();

    private:
        static const int BOARD_LENGTH = 40; // Board path
        static const int NUM_PLAYERS = 4;
        static const int NUM_PEGS = 4;
        Player player[NUM_PLAYERS];
        int roll;

        // Regular Functions
        void DrawBoard(int) const;
        void TakeTurn(int);
        int InputPegIndex(int, int) const; // Human
        int GuessPegIndex(int, int) const; // Computer
        void UpdateRoll();
        void UpdateConflicts(int);
        int CheckForWinner() const;
};

// Updates die roll
inline void Exodus::UpdateRoll()
{
    roll = rand() % 6 + 1;
}

#endif // EXODUS_H_INCLUDED
