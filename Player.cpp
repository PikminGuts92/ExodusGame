#include "Player.h"

// Main constructor
Player::Player(string inName, int inStart, int inEnd, bool inHuman)
{
    for (int i = 0; i < NUM_PEGS; i++)
    {
        // Sets each to home position
        peg[i] = -1;
    }

    name = inName;
    startPosition = inStart;
    endPosition = inEnd;
    human = inHuman;
}

// Getters
// =======

// Returns first letter of player name
char Player::Initial(bool upper) const
{
    char c = name[0];

    if ((upper && isupper(c)) || (!upper && !isupper(c)))
        return c;
    else if (upper && !isupper(c))
        return (toupper(c));
    else // !upper && isupper(c)
        return tolower(c);
}

// Regular Functions
// =================

// Determines whether move conflicts with other pegs in end slots
bool Player::PossibleMove(int i, int adv) const
{
    if ((peg[i] + adv) > endPosition + (NUM_PEGS - 1))
        return false; // Can't go past the four end slots

    for (int p = 0; p < NUM_PEGS; p++)
    {
        if (i == p) continue; // Doesn't compare with itself

        if (IsAtEnd(p) && peg[p] == (peg[i] + adv))
            // Prevents peg from landing on space occupied by another peg in end slots
            return false;
    }

    return true;
}
