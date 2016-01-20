#include "Exodus.h"

// Constructors
// ============

Exodus::Exodus(string name[], int nSize)
{
    srand (time(NULL)); // Sets seed based on clock time

    // Initializes each player - Can have more than one human
    player[0] = Player((nSize > 0) ? name[0] : "Yellow", 0, BOARD_LENGTH, (nSize > 0));
    player[1] = Player((nSize > 1) ? name[1] : "Green", 10, BOARD_LENGTH, (nSize > 1));
    player[2] = Player((nSize > 2) ? name[2] : "Red", 20, BOARD_LENGTH, (nSize > 2));
    player[3] = Player((nSize > 3) ? name[3] : "Blue", 30, BOARD_LENGTH, (nSize > 3));
}


// Regular Functions
// =================

// Starts game and alternates turn until winner is declared
void Exodus::Play()
{
    int win; // Game winner

    do
    {
        for (int i = 0; i < NUM_PLAYERS; i++)
        {
            win = CheckForWinner(); // Checks for a winner
            if (win != -1) break;

            UpdateRoll(); // Rolls die
            TakeTurn(i); // Starts turn
        }

    } while (win == -1);

    DrawBoard(0); // Draws it from player 1's POV

    cout << "Game Over - " << player[win].Name() << " wins!" << endl;
}

// Does all the logic to figure out allowed moves
// Works with both human and computer players
void Exodus::TakeTurn(int i)
{
    if (player[i].IsHuman())
        DrawBoard(i);

    if (player[i].IsHuman())
        cout << "Your turn. You rolled a " << roll << "." << endl;
    else
        cout << player[i].Name() << "'s turn. They rolled a " << roll << "." << endl;

    int moveType; // 1 = Move to start, 2 = Advance
    int pegIndex;

    // If player rolls a six, in order to continue they must either have
    // at least one peg in home or at least one peg in play, however, none
    // of the pegs in play can have the possibility of landing on another peg
    // in any of the end slots.
    if ((roll == 6) && (player[i].IsAtHome(0) || player[i].IsAtHome(1)
          || player[i].IsAtHome(2) || player[i].IsAtHome(3)
          || (player[i].IsInPlay(0) && player[i].PossibleMove(0, roll)
           || player[i].IsInPlay(1) && player[i].PossibleMove(1, roll)
           || player[i].IsInPlay(2) && player[i].PossibleMove(2, roll)
           || player[i].IsInPlay(3) && player[i].PossibleMove(3, roll))))
    {
        bool start = false, play = false;

        // Allows player to move a peg to start slot if
        // at least one peg is actually preset in home slots
        if (player[i].IsAtHome(0) || player[i].IsAtHome(1)
          || player[i].IsAtHome(2) || player[i].IsAtHome(3))
        {
            if (player[i].IsHuman())
                cout << "Press 1 to move a peg to start" << endl;
            start = true;
        }

        // Allows player to advance a peg in play if it does not
        // conflict with another peg in end slots
        if (player[i].IsInPlay(0) && player[i].PossibleMove(0, roll)
         || player[i].IsInPlay(1) && player[i].PossibleMove(1, roll)
         || player[i].IsInPlay(2) && player[i].PossibleMove(2, roll)
         || player[i].IsInPlay(3) && player[i].PossibleMove(3, roll))
        {
            if (player[i].IsHuman())
                cout << "Press 2 to move any other of your pegs" << endl;
            play = true;
        }

        do
        {
            if (player[i].IsHuman())
                cin >> moveType; // Gets move type from user
            else
            {
                // Calculates computer decision
                if (start && !play) moveType = 1;
                else if (!start && play) moveType = 2;
                else
                {
                    int chance = rand() % 10;

                    // It's 70% likely to move a peg out of home
                    if (chance < 7) moveType = 1;
                    else moveType = 2;
                }
            }
        } while (!((moveType == 1 && start) || (moveType == 2 && play)));

    }
    // Same thing as first conditional except it doesn't allow player
    // to move a peg from home slots.
    else if (player[i].IsInPlay(0) && player[i].PossibleMove(0, roll)
          || player[i].IsInPlay(1) && player[i].PossibleMove(1, roll)
          || player[i].IsInPlay(2) && player[i].PossibleMove(2, roll)
          || player[i].IsInPlay(3) && player[i].PossibleMove(3, roll))
    {
        moveType = 2; // The only possible move
    }
    // No possible moves to it ends turn here
    else
    {
        if (player[i].IsHuman())
        {
            cout << "No available moves - ";
            cout << "Press enter to continue" << endl;

            // Nice touch so console isn't constantly scrolling by itself
            cin.sync();
            cin.get();
        }

        return;
    }

    pegIndex = player[i].IsHuman() ? InputPegIndex(i, moveType) // Gets peg index from user
        : GuessPegIndex(i, moveType); // Otherwise calculates computer decision

    if (moveType == 1)
        // Moves peg to start position
        player[i].MoveToStart(pegIndex);
    else
        // Advances peg
        player[i].AdvanceBy(pegIndex, roll);

    // Kills off pegs as needed
    UpdateConflicts(i);

    if (roll == 6)
    {
        // Goes again when 6 is rolled
        UpdateRoll();
        TakeTurn(i);
    }
}

// Gets peg index from user
int Exodus::InputPegIndex(int i, int moveType) const
{
    int pegIndex;
    bool avPeg[NUM_PEGS]; // Available pegs
    string avPegName = "";

    for (int p = 0; p < NUM_PEGS; p++)
    {
        // Determines available index values
        avPeg[p] = ((moveType == 1) ? player[i].IsAtHome(p)
           : player[i].IsInPlay(p) && player[i].PossibleMove(p, roll));

        if (avPeg[p])
        {
            // Creates display of available inputs
            avPegName += char(0x31 + p);
            avPegName += " ";
        }
    }

    cout << "Enter the index of the peg you want to move" << endl;
    cout << "Available pegs are: " << avPegName << endl;

    do
    {
        cin >> pegIndex; // Gets input from user
        pegIndex--; // To match index of peg

    } while (!(pegIndex >= 0 && pegIndex < NUM_PEGS && avPeg[pegIndex]));

    return pegIndex;
}

// Calculates peg index for computer
int Exodus::GuessPegIndex(int i, int moveType) const
{
    int pegIndex = rand() % 4;
    bool avPeg[NUM_PEGS]; // Available pegs

    for (int p = 0; p < NUM_PEGS; p++)
    {
        // Determines available index values
        avPeg[p] = ((moveType == 1) ? player[i].IsAtHome(p)
           : player[i].IsInPlay(p) && player[i].PossibleMove(p, roll));
    }


    // ~This is a pretty stupid AI~
    // It justs picks a random starting index then goes
    // with the first valid (true) move.
    while(!avPeg[pegIndex])
    {
        pegIndex++;
        if (pegIndex == 4)
            pegIndex = 0; // Resets to 0
    }

    return pegIndex;
}

// Draws the entire game board in the console
void Exodus::DrawBoard(int currentIdx) const
{
    const int BA_SIZE = 11; // Board array size
    string displayName[NUM_PLAYERS]; // "Player 1", "Player 2", etc.
    string displayHome[NUM_PLAYERS]; // "yyyy", "gggg", etc.

    cout << endl;
    cout << setfill(' ');

    // Creates displayed names for each player
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        displayName[i] = player[i].Name();

        // Appends "Corner (initial)" to name
        // for competing players
        if (i != currentIdx)
        {
            displayName[i] += " Corner (";
            displayName[i] += player[i].Initial(false);
            displayName[i] += ")";
        }
    }

    // Creates displayed peg letters in home
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        // If peg is home and matches current index: Index is displayed
        // Else if peg is home and doesn't match current index: Abbreviation is displayed
        // Else: Empty space is displayed
        displayHome[i] += (player[i].IsAtHome(0)) ? ((i == currentIdx) ? char(0x30 + 1) : player[i].Initial(false)) : ' ';
        displayHome[i] += (player[i].IsAtHome(1)) ? ((i == currentIdx) ? char(0x30 + 2) : player[i].Initial(false)) : ' ';
        displayHome[i] += (player[i].IsAtHome(2)) ? ((i == currentIdx) ? char(0x30 + 3) : player[i].Initial(false)) : ' ';
        displayHome[i] += (player[i].IsAtHome(3)) ? ((i == currentIdx) ? char(0x30 + 4) : player[i].Initial(false)) : ' ';
    }

    // Displays names and pegs in home for player 1 and 2 (Top half)
    cout << left << setw(28) << displayName[0] << " " << right << setw(28) << displayName[1] << endl;
    cout << left << setw(28) << displayHome[0] << " " << right << setw(28) << displayHome[1] << endl;

    // Creates default board content
    char board[BA_SIZE][BA_SIZE] =
    {
    //    0    1    2    3    4    5    6    7    8    9   10
        {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'}, //  0
        {'*', 'o', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'o', '*'}, //  1
        {'*', ' ', 'o', ' ', ' ', ' ', ' ', ' ', 'o', ' ', '*'}, //  2
        {'*', ' ', ' ', 'o', ' ', ' ', ' ', 'o', ' ', ' ', '*'}, //  3
        {'*', ' ', ' ', ' ', 'o', '_', 'o', ' ', ' ', ' ', '*'}, //  4
        {'*', ' ', ' ', ' ', ' ', 'z', ' ', ' ', ' ', ' ', '*'}, //  5
        {'*', ' ', ' ', ' ', 'o', ' ', 'o', ' ', ' ', ' ', '*'}, //  6
        {'*', ' ', ' ', 'o', ' ', ' ', ' ', 'o', ' ', ' ', '*'}, //  7
        {'*', ' ', 'o', ' ', ' ', ' ', ' ', ' ', 'o', ' ', '*'}, //  8
        {'*', 'o', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'o', '*'}, //  9
        {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'}, // 10
    };

    board[5][5] = 0x30 + roll; // Roll Display - Converts to ASCII

    // Determines where to draw pegs on board array
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        for (int ii = 0; ii < NUM_PEGS; ii++)
        {
            int posX, posY; // Position on drawn board

            if (player[i].IsAtHome(ii)) // Home position - Not drawn on board array
                continue;
            else if (player[i].IsAtEnd(ii)) // End position
            {
                if (ii > 3) continue; // Don't wanna break anything
                posX = posY = (player[i].GlobalPosition(ii) % 10) + 1; // Top-left corner

                if (i == 1)
                {
                    // Top-right corner
                    posX = 10 - posX;
                }
                else if (i == 2)
                {
                    // Bottom-right corner
                    posX = 10 - posX;
                    posY = 10 - posY;
                }
                else if (i == 3)
                {
                    // Bottom-left corner
                    posY = 10 - posY;
                }
            }
            else // In-play position
            {
                int pos = player[i].GlobalPosition(ii);

                if (pos > -1 && pos <= 10)
                {
                    // Top of board
                    posX = pos;
                    posY = 0;
                }
                else if (pos > 10 && pos <= 20)
                {
                    // Right side of board
                    posX = 10;
                    posY = pos - 10;
                }
                else if (pos > 20 && pos <= 30)
                {
                    // Bottom of board
                    posX = 10 - (pos - 20);
                    posY = 10;
                }
                else if (pos > 30 && pos < 40)
                {
                    // Left side of board
                    posX = 0;
                    posY = 10 - (pos - 30);
                }
                else
                    continue;
            }

            // Where char in board array is actually replaced
            // Will be peg index if index match
            // Otherwise will be player's initial
            if (i == currentIdx)
                board[posY][posX] = char(0x30 + (ii + 1));
            else
                board[posY][posX] = player[i].Initial(false);
        }
    }

    // Draws board array content
    for (int i = 0; i < BA_SIZE; i++)
    {
        for (int ii = 0; ii < BA_SIZE; ii++)
        {
            // Draws each space
            // First two conditionals draw upper outline for die enclosure
            // Following two conditionals draw left/right outline for die enclosure
            if (i == 4 && ii == 5)
                cout << right << setw(4) << '_' << board[i][ii];
            else if (i == 4 && ii == 6)
                cout << left << setw(4) << '_' << board[i][ii];
            else if (i == 5 && ii == 5)
                cout << left << " |  " << board[i][ii];
            else if (i == 5 && ii == 6)
                cout << right << "  | " << board[i][ii];
            else
                cout << right << ((ii == 0) ? setw(4) : setw(5)) << board[i][ii];
        }

        if (i == 4)
        {
            // Draws remaining upper outline for die enclosure
            cout << endl << right << setw(32) << "/    \\";
            cout << endl << right << setw(32) << "|     |";
            cout << endl;
        }
        else if (i == 5)
        {
            // Draws lower outline for die enclosure
            cout << endl << right << setw(32) << "|     |";
            cout << endl << right << setw(32) << "\\___/ ";
            cout << endl;
        }
        else
            cout << endl << endl << endl;
    }

    // Displays names and pegs in home for player 3 and 4 (Bottom half)
    cout << left << setw(28) << displayName[3] << " " << right << setw(28) << displayName[2] << endl;
    cout << left << setw(28) << displayHome[3] << " " << right << setw(28) << displayHome[2] << endl;
}

// Determines occupied space conflicts and resolves them.
// i.e. Kills original occupants (Sends back to home)
void Exodus::UpdateConflicts(int i)
{
    for (int p = 0; p < NUM_PLAYERS; p++)
    {
        if (p == i) continue; // Won't compare with itself

        for (int pp = 0; pp < NUM_PEGS; pp++)
        {
            if (!player[p].IsInPlay(pp)) continue; // Won't compare pegs not in play (i.e. In home/end slots)

            if (player[i].IsInPlay(0) && player[p].GlobalPosition(pp) == player[i].GlobalPosition(0))
            {
                cout << player[i].Name() << " killed " << player[p].Name() << "'s #" << (pp + 1) << " peg.";
                cout << endl;

                player[p].MoveToHome(pp);
            }

            if (player[i].IsInPlay(1) && player[p].GlobalPosition(pp) == player[i].GlobalPosition(1))
            {
                cout << player[i].Name() << " killed " << player[p].Name() << "'s #" << (pp + 1) << " peg.";
                cout << endl;

                player[p].MoveToHome(pp);
            }

            if (player[i].IsInPlay(2) && player[p].GlobalPosition(pp) == player[i].GlobalPosition(2))
            {
                cout << player[i].Name() << " killed " << player[p].Name() << "'s #" << (pp + 1) << " peg.";
                cout << endl;

                player[p].MoveToHome(pp);
            }

            if (player[i].IsInPlay(3) && player[p].GlobalPosition(pp) == player[i].GlobalPosition(3))
            {
                cout << player[i].Name() << " killed " << player[p].Name() << "'s #" << (pp + 1) << " peg.";
                cout << endl;

                player[p].MoveToHome(pp);
            }
        }
    }
}

// Checks for winner
int Exodus::CheckForWinner() const
{
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        // Checks whether all of player's pegs are in the end
        if (player[i].IsAtEnd(0) && player[i].IsAtEnd(1)
            && player[i].IsAtEnd(2) && player[i].IsAtEnd(3))
                return i;
    }

    return -1;
}
