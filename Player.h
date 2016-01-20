#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <iostream>
#include <stdlib.h> // abs

using namespace std;

class Player
{
    public:
        // Constructors
        Player();
        Player(string, int, int, bool);

        // Getters
        int GlobalPosition(int) const;
        string Name() const;
        bool IsHuman() const;
        bool IsAtHome(int) const;
        bool IsAtStart(int) const;
        bool IsInPlay(int) const;
        bool IsAtEnd(int) const;
        char Initial(bool) const;

        // Setters
        void MoveToHome(int);
        void MoveToStart(int);
        void AdvanceBy(int, int);

        // Regular Functions
        bool PossibleMove(int, int) const;

    private:
        static const int NUM_PEGS = 4;
        int peg[NUM_PEGS];
        int startPosition;
        int endPosition;
        string name;
        bool human;
};

// Default constructor
inline Player::Player() : Player("", 0, 40, true)
{

}

// Getters
// =======

// Gets global position on board
inline int Player::GlobalPosition(int i) const
{
    return (i > -1) && (i < NUM_PEGS) ? ((peg[i] + startPosition) % endPosition) : -1;
}

// Gets player's name
inline string Player::Name() const
{
    return name;
}

// Gets whether player is controlled by a human
inline bool Player::IsHuman() const
{
    return human;
}

// Gets whether peg is at home
inline bool Player::IsAtHome(int i) const
{
    return (i > -1) && (i < NUM_PEGS) ? (peg[i] == -1) : false;
}

// Gets whether peg is at start
inline bool Player::IsAtStart(int i) const
{
    return (i > -1) && (i < NUM_PEGS) ? (peg[i] == 0) : false;
}

// Gets whether peg is in play
inline bool Player::IsInPlay(int i) const
{
    return (i > -1) && (i < NUM_PEGS) ? (peg[i] > -1 && peg[i] < endPosition) : false;
}

// Gets whether peg is at end
inline bool Player::IsAtEnd(int i) const
{
    return (i > -1) && (i < NUM_PEGS) ? (peg[i] >= endPosition) : false;
}

// Setters
// =======

// Sets peg to home position
inline void Player::MoveToHome(int i)
{
    if ((i > -1) && (i < NUM_PEGS))
        peg[i] = -1;
}

// Sets peg to start position
inline void Player::MoveToStart(int i)
{
    if ((i > -1) && (i < NUM_PEGS))
        peg[i] = 0;
}

// Sets peg to new position determined by advancement value
inline void Player::AdvanceBy(int i, int adv)
{
    if ((i > -1) && (i < NUM_PEGS))
        peg[i] = peg[i] + abs(adv); // Advances by positive value
}

#endif // PLAYER_H_INCLUDED
