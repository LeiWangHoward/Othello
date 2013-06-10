#include <iostream>
#include <string>
using namespace::std;

class Wang_Zhou {
        int squares[8][8];
	int my_color;
	int pass;
public:
        Wang_Zhou();
        string toString();
        bool play_square(int, int, int);
        bool play_square(int&, int&);
	//void reset_square(int, int);
	int count_valid_moves(int);
        bool move_is_valid(int, int, int);
        bool check_or_flip_path(int, int, int, int, int, bool);
        int get_square(int, int);
        int score();
	int total_coins(); //return total amount of coins
        bool full_board();
        bool has_valid_move(int);
};

bool make_simple_cpu_move(Wang_Zhou * b, int cpuval);
bool alpha_beta_search(Wang_Zhou*, int, int, int&, int&);
int MinValue(Wang_Zhou*, int, int, int, int, int);
int MaxValue(Wang_Zhou*, int, int, int, int, int);
int Evaluate(Wang_Zhou*, int);
