// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <iostream>
#include <fstream>
#include <vector>

#include "Player.h"
#include "Pack.h"

using namespace std;

class Game {
    public:
        Game (vector<Player*> players_in,
                int team_1_points_in, int team_2_points_in,
                Pack pack_in)
                : players(players_in), 
                team_1_points(team_1_points_in), 
                team_2_points(team_2_points_in), pack(pack_in) {}

        void deal(int dealer_num, int hand_count) {
            cout << "\nHand " << hand_count << "\n";
            cout << players.at(dealer_num)->get_name() << " deals\n";

            int receive;
            if (dealer_num < 3)
                receive = dealer_num + 1;
            else
                receive = 0;

            for (int i = 0; i < 4; ++i) {
                if (i%2 == 0) {
                    for (int j = 0; j < 3; ++j) {
                        Card c = pack.deal_one();
                        players.at(receive)->add_card(c);
                    }
                }
                else
                {
                    for (int j = 0; j < 2; ++j) {
                        Card c = pack.deal_one();
                        players.at(receive)->add_card(c);
                    }
                }
                if (receive < 3)
                    ++receive;
                else    
                    receive = 0;
            }

            for (int i = 0; i < 4; ++i) {
                if (i%2 == 0) {
                    for (int j = 0; j < 2; ++j) {
                        Card c = pack.deal_one();
                        players.at(receive)->add_card(c);
                    }
                }
                else
                {
                    for (int j = 0; j < 3; ++j) {
                        Card c = pack.deal_one();
                        players.at(receive)->add_card(c);
                    }
                }
                if (receive < 3)
                    ++receive;
                else    
                    receive = 0;
            }
        }

        void shuffle() {
            pack.shuffle();
        }

        void reset() {
            pack.reset();
        }

        Card get_upcard() {
            Card upcard = pack.deal_one();

            reset();

            cout << upcard << " turned up\n";
            return upcard;
        }

        void play_round(Card upcard, int dealer_num) {
            string trump = ""; 
            make_trump(upcard, dealer_num, trump, 1);
            if (trump == "")
                make_trump(upcard, dealer_num, trump, 2);

            for (int i = 0; i < 5; ++i) {
                trick(trump);
            }
        }

        void make_trump(Card upcard, int dealer_num, string &trump,
                                int round) {
            int receive;
            if (dealer_num < 3) {
                receive = dealer_num + 1;
                lead = dealer_num + 1;
            }
            else {
                receive = 0;
                lead = 0;
            }
            bool is_dealer = false;

            for (int i = 0; i < 4; ++i) {
                Player* p = players.at(receive);

                if (i == 3)
                    is_dealer = true;

                if (p->make_trump(upcard, is_dealer, round, trump)) {
                    cout << *p << " orders up " << trump << "\n\n";
                    if (round == 1)
                        players.at(dealer_num)->add_and_discard(upcard);
                    trick_team = receive%2 + 1;
                    made_trump = receive;
                    return;
                }
                else
                    cout << *p << " passes\n";

                if (receive < 3)
                    ++receive;
                else
                    receive = 0;
            }
        }

        void trick(const string trump) {
            int receive = lead;
            vector<Card> tricks;

            Card lead_card = players.at(receive)->lead_card(trump);
            tricks.push_back(lead_card);
            cout << lead_card << " led by " << *players.at(receive) << "\n";

            for (int i = 0; i < 3; ++i) {
                if (receive < 3)
                    ++receive;
                else
                    receive = 0;

                Player* p = players.at(receive);
                Card c = p->play_card(lead_card, trump);
                tricks.push_back(c);

                cout << c << " played by " << *p << "\n";
            }

            Card highest = tricks.at(0);
            int index = 0;
            for (int j = 0; j < tricks.size(); ++j) {
                if (Card_less(highest, tricks.at(j), 
                    lead_card, trump)) {
                    highest = tricks.at(j);
                    index = j;
                }
            }
            index = (lead + index)%4;
            lead = index;
            cout << *players.at(index) << " takes the trick\n\n";

            if (index%2 == 0)
                ++team_1_tricks;
            else
                ++team_2_tricks;
        }

        int calc_score() {
            if (team_1_tricks > team_2_tricks) {
                cout << *players.at(0) << " and " << *players.at(2) <<
                        " win the hand\n";

                if (trick_team == 1) {
                    if (team_1_tricks == 3 || team_1_tricks == 4)
                        team_1_points = 1;
                    else {
                        team_1_points = 2;
                        cout << "march!\n";
                    }
                }
                else {
                    team_1_points = 2;
                    cout << "euchred!\n";
                }
                team_1_tricks = 0;
                team_2_tricks = 0;
                return 1;
            }
            else {
                cout << *players.at(1) << " and " << *players.at(3) <<
                        " win the hand\n";

                if (trick_team == 2) {
                    if (team_2_tricks == 3 || team_2_tricks == 4)
                        team_2_points = 1;
                    else {
                        team_2_points = 2;
                        cout << "march!\n";
                    }
                }
                else {
                    team_2_points = 2;
                    cout << "euchred!\n";
                }
                team_1_tricks = 0;
                team_2_tricks = 0;
                return 2;
            }
        }

        void calc_total(int &team_1_points_in, int &team_2_points_in,
                        vector<Player*> players) {
            int trick_win = calc_score();

            if (trick_win == 1) {
                team_1_points_in += team_1_points;
            }
            else if (trick_win == 2) {
                team_2_points_in += team_2_points;
            }

            cout << *players.at(0) << " and " << *players.at(2) <<
                            " have " << team_1_points_in << " points\n";
            cout << *players.at(1) << " and " << *players.at(3) <<
                            " have " << team_2_points_in << " points\n";
        }

        int get_team_1_points() {
            return team_1_points;
        }

        int get_team_2_points() {
            return team_2_points;
        }

    private:
        vector<Player*> players;
        int team_1_points;
        int team_2_points;
        Pack pack;
        int trick_team = 0;
        int lead = 0;
        int made_trump = 0;
        int team_1_tricks = 0;
        int team_2_tricks = 0;
};

static bool check_cmd_ln_args(int argc, char * argv[]) {
    if (argc != 12 ||
        atoi(argv[3]) < 1 || atoi(argv[3]) > 100 ||
        (string(argv[2]) != "shuffle" && string(argv[2]) != "noshuffle") ||
        (string(argv[5]) != "Simple" && string(argv[5]) != "Human") ||
        (string(argv[7]) != "Simple" && string(argv[7]) != "Human") ||
        (string(argv[9]) != "Simple" && string(argv[9]) != "Human") ||
        (string(argv[11]) != "Simple" && string(argv[11]) != "Human")) {
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
                 << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
                 << "NAME4 TYPE4" << endl;
            return false;
        }
    return true;
}

static void print_input(int argc, char * argv[]) {
    for (int i = 0; i < argc; ++i) {
        cout << argv[i] << " ";
    }
}

static void delete_players(vector<Player*> players) {
    for (int i = 0; i < players.size(); ++i) {
        delete players.at(i);
    }
}

int main(int argc, char * argv[]) {
    if(!check_cmd_ln_args(argc, argv))
        return 1;

    string pack_filename = argv[1];
    ifstream fin(pack_filename);
    if (!fin.is_open()) {
        cout << "Error opening " << pack_filename << endl;
        return 2;
    }
    Pack pack = Pack(fin);
    fin.close();

    print_input(argc, argv);

    vector<Player*> players;
    players.push_back(Player_factory(argv[4], argv[5]));
    players.push_back(Player_factory(argv[6], argv[7]));
    players.push_back(Player_factory(argv[8], argv[9]));
    players.push_back(Player_factory(argv[10], argv[11]));

    int hand_count = 0;
    int dealer_num = 0;
    int team_1_points = 0;
    int team_2_points = 0;
    const static int points_to_win = atoi(argv[3]);

    Game g = Game(players, team_1_points, team_2_points, pack);

    while (team_1_points < points_to_win && team_2_points < points_to_win) {
        if (string(argv[2]) == "shuffle")
            g.shuffle();  
        else
            g.reset();

        g.deal(dealer_num, hand_count);

        Card upcard = g.get_upcard();   

        g.play_round(upcard, dealer_num);

        g.calc_total(team_1_points, team_2_points, players);

        dealer_num = (dealer_num + 1)%4;
        ++hand_count;
    }

    if (team_1_points > team_2_points)
        cout << "\n" << *players.at(0) << " and " << *players.at(2) << " win!\n";
    else
        cout << "\n" << *players.at(1) << " and " << *players.at(3) << " win!\n";

    delete_players(players);

    return 0;
}