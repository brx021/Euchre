// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <iostream>
#include <string>
#include <algorithm>
#include <cassert>
#include "Player.h"

using namespace std;

const static int find_lowest(const vector<Card> hand, const string trump) {
  Card lowest = hand.at(0);
  int lowest_index = 0;

  for (int i = 1; i < hand.size(); ++i) {
    if (Card_less(hand.at(i), lowest, trump)) {
      lowest = hand.at(i);
      lowest_index = i;
    }
  }

  return (lowest_index);
}

const static int find_highest(const vector<Card> hand, const string trump) {
  Card highest = hand.at(0);
  int highest_index = 0;

  for ( int i = 1; i < hand.size(); ++i) {
    if (!Card_less(hand.at(i), highest, trump)) {
      highest = hand.at(i);
      highest_index = i;
    }
  }

  return highest_index;
}

static void sort_and_print(vector<Card> &hand, const string name) {
  for (int j = 0; j < hand.size(); ++j) {
    for (int k = j + 1; k < hand.size(); ++k) {
      if (hand.at(k) < hand.at(j)) {
        Card temp = hand.at(k);
        hand.at(k) = hand.at(j);
        hand.at(j) = temp;
      }
    }
  }

  for (int i = 0; i < hand.size(); ++i) {
    cout << "Human player " << name << "'s hand: [" << i <<
            "] " << hand.at(i) << "\n";
  }
}

const static int to_num(string input) {
  if (input == "-1")
    return -1;
  else if (input == "0")
    return 0;
  else if (input == "1")
    return 1;
  else if (input == "2")
    return 2;
  else if (input == "3")
    return 3;
  else if (input == "4")
    return 4;
  
  return 100;
}

class SimplePlayer : public Player {
    public:
      SimplePlayer(const string &name_in) : name(name_in) {}

      //EFFECTS returns player's name
      const std::string & get_name() const {
        return name;
      }

      //REQUIRES player has less than MAX_HAND_SIZE cards
      //EFFECTS  adds Card c to Player's hand
      void add_card(const Card &c) {
        assert(static_cast<int>(hand.size()) < MAX_HAND_SIZE);
        
        hand.push_back(c);
      }

      //REQUIRES round is 1 or 2
      //MODIFIES order_up_suit
      //EFFECTS If Player wishes to order up a trump suit then return true and
      //  change order_up_suit to desired suit.  If Player wishes to pass, then do
      //  not modify order_up_suit and return false.
      bool make_trump(const Card &upcard, bool is_dealer,
                        int round, std::string &order_up_suit) const {
                          if (round == 1) {

                            int face_count = 0;
                            for (int i = 0; i < hand.size(); ++i) {

                              if (hand.at(i).is_trump(upcard.get_suit()) 
                                  && hand.at(i).is_face())
                                ++face_count;
                            }

                            if (face_count >= 2) {
                              order_up_suit = upcard.get_suit();
                              return true;
                            }

                          }
                          else {

                            string consider = Suit_next(upcard.get_suit());

                            if (is_dealer) {
                              order_up_suit = consider;
                              return true;
                            }

                            int face_count = 0;
                            for (int j = 0; j < hand.size(); ++j) {

                              if (hand.at(j).is_trump(consider) && 
                                  hand.at(j).is_face())
                                ++face_count;
                            }

                            if (face_count >= 1) {
                              order_up_suit = consider;
                              return true;
                            }

                          }

                          return false;
                        }

      //REQUIRES Player has at least one card
      //EFFECTS  Player adds one card to hand and removes one card from hand.
      void add_and_discard(const Card &upcard) {
        string trump = upcard.get_suit();
        int lowest_index = find_lowest(hand, trump);
        hand.at(lowest_index) = upcard;
      }

      //REQUIRES Player has at least one card, trump is a valid suit
      //EFFECTS  Leads one Card from Player's hand according to their strategy
      //  "Lead" means to play the first Card in a trick.  The card
      //  is removed the player's hand.
      Card lead_card(const std::string &trump) {
        bool all_trumps = true;
        vector<Card> non_trump;

        for (int i = 0; i < hand.size(); ++i) {
          if (!hand.at(i).is_trump(trump)) {
            all_trumps = false;
            non_trump.push_back(hand.at(i));
          }
        }

        if (all_trumps) {
          int highest = find_highest(hand, trump);
          Card c = hand.at(highest);
          hand.erase(hand.begin() + highest);
          return c;
        }
        else {
          int highest = find_highest(non_trump, trump);
          Card c = non_trump.at(highest);

          for (int j = 0; j < hand.size(); ++j) {
            if (hand.at(j) == c)
              hand.erase(hand.begin() + j);
          }

          return c;
        }
      }

      //REQUIRES Player has at least one card, trump is a valid suit
      //EFFECTS  Plays one Card from Player's hand according to their strategy.
      //  The card is removed from the player's hand.
      Card play_card(const Card &led_card, const std::string &trump) {
        bool has_led = false;
        vector<Card> led;

        for (int i = 0; i < hand.size(); ++i) {
          if (hand.at(i).get_suit(trump) == led_card.get_suit(trump)) {
            has_led = true;
            led.push_back(hand.at(i));
          }
        }

        if (has_led) {
          int highest = find_highest(led, trump);
          Card c = led.at(highest);

          for (int j = 0; j < hand.size(); ++j) {
            if (hand.at(j) == c) {
                hand.erase(hand.begin() + j);
            }
          }

          return c;
        }
        else {
          int lowest = find_lowest(hand, trump);
          if (lowest < hand.size()) {
            Card c = hand.at(lowest);
            hand.erase(hand.begin() + lowest);
            return c;
          }
        }
        Card c;
        return c;
      }

    private:
      string name;
      vector<Card> hand;
};

class HumanPlayer : public Player {
    public: 
      HumanPlayer(const string &name_in) : name(name_in) {}

      //EFFECTS returns player's name
      const std::string & get_name() const {
        return name;
      }

      //REQUIRES player has less than MAX_HAND_SIZE cards
      //EFFECTS  adds Card c to Player's hand
      void add_card(const Card &c) {
        hand.push_back(c);
      }

      //REQUIRES round is 1 or 2
      //MODIFIES order_up_suit
      //EFFECTS If Player wishes to order up a trump suit then return true and
      //  change order_up_suit to desired suit.  If Player wishes to pass, then do
      //  not modify order_up_suit and return false.
      bool make_trump(const Card &upcard, bool is_dealer,
                        int round, std::string &order_up_suit) const {

                          vector<Card> temp = hand;
                          for (int j = 0; j < temp.size(); ++j) {
                            for (int k = j + 1; k < temp.size(); ++k) {
                              if (temp.at(k) < temp.at(j)) {
                                Card tc = temp.at(k);
                                temp.at(k) = temp.at(j);
                                temp.at(j) = tc;
                              }
                            }
                          }
                          for (int i = 0; i < temp.size(); ++i) {
                            cout << "Human player " << name << "'s hand: [" << i <<
                                    "] " << temp.at(i) << "\n";
                          }
                          

                          string input;
                          cout << "Human player " << get_name() << 
                                  ", please enter a suit" <<
                                  ", or \"pass\":\n";
                          cin >> input;

                          if (input == "pass") {
                            return false;
                          }
                          
                          order_up_suit = input;
                          return true;
                        }

      //REQUIRES Player has at least one card
      //EFFECTS  Player adds one card to hand and removes one card from hand.
      void add_and_discard(const Card &upcard) {
        sort_and_print(hand, get_name());

        string input;
        cout << "Discard upcard: [-1]\n";
        cout << "Human player " << get_name() << ", please select a card to discard:\n";
        cin >> input;
        int num = to_num(input);
        cout << "\n";
        
        if (num != -1)
          hand.at(num) = upcard;
      }

      //REQUIRES Player has at least one card, trump is a valid suit
      //EFFECTS  Leads one Card from Player's hand according to their strategy
      //  "Lead" means to play the first Card in a trick.  The card
      //  is removed the player's hand.
      Card lead_card(const std::string &trump) {
        sort_and_print(hand, get_name());

        string input;
        cout << "Human player " << get_name() << ", please select a card:";
        cin >> input;
        int num = to_num(input);
        cout << "\n";

        Card c = hand.at(num);
        hand.erase(hand.begin() + num);
        return c;
      }

      //REQUIRES Player has at least one card, trump is a valid suit
      //EFFECTS  Plays one Card from Player's hand according to their strategy.
      //  The card is removed from the player's hand.
      Card play_card(const Card &led_card, const std::string &trump) {
        sort_and_print(hand, get_name());

        string input;
        cout << "Human player " << get_name() << ", please select a card:";
        cin >> input;
        int num = to_num(input);
        cout << "\n";

        Card c = hand.at(num);
        hand.erase(hand.begin() + num);
        return c;
      }

    private:
      string name;
      vector<Card> hand;
};

Player * Player_factory(const std::string &name, const std::string &strategy) {
  // We need to check the value of strategy and return 
  // the corresponding player type.
  if (strategy == "Simple") {
    // The "new" keyword dynamically allocates an object.
    return new SimplePlayer(name);
  }
  else if (strategy == "Human") {
    return new HumanPlayer(name);
  }
  
  // Invalid strategy if we get here
  assert(false);
  return nullptr;
}

std::ostream & operator<<(std::ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}