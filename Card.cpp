// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <string>
#include "Card.h"
// add any necessary #include or using directives here

// rank and suit names -- do not remove these
constexpr const char* const Card::RANK_TWO;
constexpr const char* const Card::RANK_THREE;
constexpr const char* const Card::RANK_FOUR;
constexpr const char* const Card::RANK_FIVE;
constexpr const char* const Card::RANK_SIX;
constexpr const char* const Card::RANK_SEVEN;
constexpr const char* const Card::RANK_EIGHT;
constexpr const char* const Card::RANK_NINE;
constexpr const char* const Card::RANK_TEN;
constexpr const char* const Card::RANK_JACK;
constexpr const char* const Card::RANK_QUEEN;
constexpr const char* const Card::RANK_KING;
constexpr const char* const Card::RANK_ACE;

constexpr const char* const Card::SUIT_SPADES;
constexpr const char* const Card::SUIT_HEARTS;
constexpr const char* const Card::SUIT_CLUBS;
constexpr const char* const Card::SUIT_DIAMONDS;

//EFFECTS Initializes Card to the Two of Spades
Card::Card() : rank(Card::RANK_TWO), suit(Card::SUIT_SPADES) {}

//REQUIRES rank is one of "Two", "Three", "Four", "Five", "Six", "Seven",
//  "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"
//  suit is one of "Spades", "Hearts", "Clubs", "Diamonds"
//EFFECTS Initializes Card to specified rank and suit
Card::Card(const std::string &rank_in, const std::string &suit_in) : 
            rank(rank_in), suit(suit_in) {}

//EFFECTS Returns the rank
std::string Card::get_rank() const {
    return rank;
}

//EFFECTS Returns the suit.  Does not consider trump.
std::string Card::get_suit() const {
    return suit;
}

//REQUIRES trump is a valid suit
//EFFECTS Returns the suit
//HINT: the left bower is the trump suit!
std::string Card::get_suit(const std::string &trump) const {
    if (is_left_bower(trump))
        return trump;

    return suit;
}

//EFFECTS Returns true if card is a face card (Jack, Queen, King or Ace)
bool Card::is_face() const {
    if (rank == Card::RANK_JACK || rank == Card::RANK_QUEEN 
        || rank == Card::RANK_KING || rank == Card::RANK_ACE)
        return true;

    return false;
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if card is the Jack of the trump suit
bool Card::is_right_bower(const std::string &trump) const {
    return (rank == Card::RANK_JACK && suit == trump);
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if card is the Jack of the next suit
bool Card::is_left_bower(const std::string &trump) const {
    bool same_color = false;
    if ((trump == Card::SUIT_SPADES && suit == Card::SUIT_CLUBS) 
        || (trump == Card::SUIT_CLUBS && suit == Card::SUIT_SPADES) 
        || (trump == Card::SUIT_DIAMONDS && suit == Card::SUIT_HEARTS) 
        || (trump == Card::SUIT_HEARTS && suit == Card::SUIT_DIAMONDS))
        same_color = true;

    return (same_color && rank == Card::RANK_JACK);
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if the card is a trump card.  All cards of the trump
// suit are trump cards.  The left bower is also a trump card.
bool Card::is_trump(const std::string &trump) const {
    return (get_suit(trump) == trump);
}

//EFFECTS Returns true if lhs is lower value than rhs.
//  Does not consider trump.
bool operator<(const Card &lhs, const Card &rhs) {
    int lhs_rank;
    int rhs_rank;

    for (int i = 0; i < NUM_RANKS; ++i) {
        if (lhs.get_rank() == RANK_NAMES_BY_WEIGHT[i])
            lhs_rank = i;
        if (rhs.get_rank() == RANK_NAMES_BY_WEIGHT[i])
            rhs_rank = i;
    }
    
    if (lhs_rank == rhs_rank) {
        int lhs_suit;
        int rhs_suit;

        for (int j = 0; j < NUM_SUITS; ++j) {
            if (lhs.get_suit() == SUIT_NAMES_BY_WEIGHT[j])
                lhs_suit = j;
            if (rhs.get_suit() == SUIT_NAMES_BY_WEIGHT[j])
                rhs_suit = j;
        }
        
        return (lhs_suit < rhs_suit);
    }

    return (lhs_rank < rhs_rank);
}

//EFFECTS Returns true if lhs is lower value than rhs or the same card as rhs.
//  Does not consider trump.
bool operator<=(const Card &lhs, const Card &rhs) {
    bool equal = (lhs.get_rank() == rhs.get_rank()
                    && lhs.get_suit() == rhs.get_suit());
    
    return (lhs < rhs || equal);
}

//EFFECTS Returns true if lhs is higher value than rhs.
//  Does not consider trump.
bool operator>(const Card &lhs, const Card &rhs) {
    return !(lhs <= rhs);
}

//EFFECTS Returns true if lhs is higher value than rhs or the same card as rhs.
//  Does not consider trump.
bool operator>=(const Card &lhs, const Card &rhs) {
    return !(lhs < rhs);
}

//EFFECTS Returns true if lhs is same card as rhs.
//  Does not consider trump.
bool operator==(const Card &lhs, const Card &rhs) {
    return (lhs <= rhs && lhs >= rhs);
}

//EFFECTS Returns true if lhs is not the same card as rhs.
//  Does not consider trump.
bool operator!=(const Card &lhs, const Card &rhs) {
    return (lhs < rhs || lhs > rhs);
}

//REQUIRES suit is a valid suit
//EFFECTS returns the next suit, which is the suit of the same color
std::string Suit_next(const std::string &suit) {
    if (suit == Card::SUIT_SPADES)
        return Card::SUIT_CLUBS;
    else if (suit == Card::SUIT_CLUBS)
        return Card::SUIT_SPADES;
    else if (suit == Card::SUIT_HEARTS)
        return Card::SUIT_DIAMONDS;
    else
        return Card::SUIT_HEARTS;
}

//EFFECTS Prints Card to stream, for example "Two of Spades"
std::ostream & operator<<(std::ostream &os, const Card &card) {
    os << card.get_rank() << " of " << card.get_suit();
    return os;
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if a is lower value than b.  Uses trump to determine
// order, as described in the spec.
bool Card_less(const Card &a, const Card &b, const std::string &trump) {
    if (b.is_trump(trump) && !a.is_trump(trump))
        return true;

    else if (b.is_trump(trump) && a.is_trump(trump)) {

        if (a.is_left_bower(trump) && b.is_right_bower(trump))
            return true;

        else if (!a.is_left_bower(trump) && !a.is_right_bower(trump)) {
            if (b.is_left_bower(trump) || b.is_right_bower(trump))
                return true;
            return (a < b);
        }
    }

    else if (!a.is_trump(trump) && !b.is_trump(trump))
        return (a < b);

    return false;
}

//REQUIRES trump is a valid suit
//EFFECTS Returns true if a is lower value than b.  Uses both the trump suit
//  and the suit led to determine order, as described in the spec.
bool Card_less(const Card &a, const Card &b, const Card &led_card,
               const std::string &trump) {
                    if (a.is_trump(trump) || b.is_trump(trump))
                        return Card_less(a, b, trump);

                    if (b.get_suit() == led_card.get_suit()
                        && a.get_suit() != led_card.get_suit())
                        return true;
                    else if (a.get_suit() == led_card.get_suit()
                        && b.get_suit() != led_card.get_suit())
                        return false;

                    return (a < b);
               }

// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec (see the appendices) before implementing
// the following operator overload functions:
//   operator<<
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=
