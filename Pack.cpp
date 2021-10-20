// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <string>
#include "Pack.h"

using namespace std;

// EFFECTS: Initializes the Pack to be in the following standard order:
//          the cards of the lowest suit arranged from lowest rank to
//          highest rank, followed by the cards of the next lowest suit
//          in order from lowest to highest rank, and so on.
// NOTE: The standard order is the same as that in pack.in.
// NOTE: Do NOT use pack.in in your implementation of this function
Pack::Pack() {
    next = 0;

    int count = 0;
    for (int suit = 0; suit < NUM_SUITS; ++suit) {
        for (int rank = 7; rank < NUM_RANKS; ++rank) {
            Card c(RANK_NAMES_BY_WEIGHT[rank], SUIT_NAMES_BY_WEIGHT[suit]);
            cards[count] = c;
            ++count;
        }
    }
}

// REQUIRES: pack_input contains a representation of a Pack in the
//           format required by the project specification
// MODIFIES: pack_input
// EFFECTS: Initializes Pack by reading from pack_input.
Pack::Pack(std::istream& pack_input) {
    next = 0;

    string rank;
    string suit;
    string temp;
    int count = 0;
    int loop = 0;

    while (pack_input >> temp) {
        if (count%3 == 0)
            rank = temp;
        else if (count%3 == 2) {
            suit = temp;
            Card c(rank, suit);
            cards[loop] = c;
            ++loop;
        }
        ++count;
    }
}

// REQUIRES: cards remain in the Pack
// EFFECTS: Returns the next card in the pack and increments the next index
Card Pack::deal_one() {
    ++next;
    return cards[next - 1];
}

// EFFECTS: Resets next index to first card in the Pack
void Pack::reset() {
    next = 0;
}

// EFFECTS: Shuffles the Pack and resets the next index. This
//          performs an in shuffle seven times. See
//          https://en.wikipedia.org/wiki/In_shuffle.
void Pack::shuffle() {
    array<Card, PACK_SIZE/2> half1;
    array<Card, PACK_SIZE/2> half2;
    int count1;
    int count2;

    for (int i = 0; i < 7; ++i) {
        count1 = 0;
        count2 = 0;
        
        for (int j = 0; j < PACK_SIZE; ++j) {
            if (j < PACK_SIZE/2) {
                half1[count1] = cards[j];
                ++count1;
            }
            else
            {
                half2[count1 - PACK_SIZE/2] = cards[j];
                ++count1;
            }
        }

        for (int k = 0; k < PACK_SIZE; k += 2) {        
            cards[k] = half2[count2];
            cards[k + 1] = half1[count2];
            ++count2;
        }
    }
}

// EFFECTS: returns true if there are no more cards left in the pack
bool Pack::empty() const {
    return (next >= PACK_SIZE);
}