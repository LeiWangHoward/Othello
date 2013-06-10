// othello.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sstream>
#include <exception>
#include <string>
#include <stdlib.h>
#include <cctype>
#include "Wang_Zhou.h"
using namespace std;

//initialize the board
Wang_Zhou::Wang_Zhou() {
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			squares[i][j] = 0;
	squares[3][3]=-1;//white
	squares[4][4]=-1;
	squares[3][4]=1;//black
	squares[4][3]=1;
	my_color = -1;//default, we play white
	pass = 0;
}
//print out board for check
string Wang_Zhou::toString() {
	stringstream s;
	char cforvalplusone[] = {'W', '_', 'B'};
	s << "  1 2 3 4 5 6 7 8" << endl;
	for(int i=0; i<8;i++) {
		s << i+1 << '|';
		for(int j=0; j<8; j++)
			s << cforvalplusone[squares[i][j]+1] << '|';
		s << endl;
	}
	return s.str();
}

//returns if player with val has some valid move in this configuration
bool Wang_Zhou::has_valid_move(int val) {
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			if(move_is_valid(i+1, j+1, val))
				return true;
	return false;
}
//return number of valid moves
int Wang_Zhou::count_valid_moves(int val) {
	int count=0;
	for(int i=0; i<8;i++)
                for(int j=0; j<8; j++)
                        if(move_is_valid(i+1, j+1, val))
                                ++count;
	return count;
}
//r and c zero indexed here
//checks whether path in direction rinc, cinc results in flips for val
//will actually flip the pieces along path when doFlips is true
bool Wang_Zhou::check_or_flip_path(int r, int c, int rinc, int cinc, int val, bool doFlips) {
	int pathr = r + rinc;
	int pathc = c + cinc;
	if(pathr < 0 || pathr > 7 || pathc < 0 || pathc > 7 || squares[pathr][pathc]!=-1*val)
		return false;
	//check for some chip of val's along the path:
	while(true) {
		pathr += rinc;
		pathc += cinc;
		if(pathr < 0 || pathr > 7 || pathc < 0 || pathc > 7 || squares[pathr][pathc]==0)
			return false;
		if(squares[pathr][pathc]==val) {
			if(doFlips) {
				pathr=r+rinc;
				pathc=c+cinc;
				while(squares[pathr][pathc]!=val) {
					squares[pathr][pathc]=val;
					pathr += rinc;
					pathc += cinc;
				}
			}
			return true;
		}
	}
	return false;	
}


//returns whether given move is valid in this configuration
bool Wang_Zhou::move_is_valid(int row, int col, int val) {
	int r = row-1;
	int c = col-1;
	if(r < 0 || r > 7 || c < 0 || c > 7)
		return false;
	//check whether space is occupied:
	if(squares[r][c]!=0)
		return false;
	//check that there is at least one path resulting in flips:
	for(int rinc = -1; rinc <= 1; rinc++)
		for(int cinc = -1; cinc <= 1; cinc++) {
			if(check_or_flip_path(r, c, rinc, cinc, val, false))
				return true;
		}
	return false;
}

//executes move if it is valid.  Returns false and does not update board otherwise
bool Wang_Zhou::play_square(int row, int col, int val) {
	if(!move_is_valid(row, col, val))
		return false;
	squares[row-1][col-1] = val;
	for(int rinc = -1; rinc <= 1; rinc++)
		for(int cinc = -1; cinc <= 1; cinc++) {
			check_or_flip_path(row-1, col-1, rinc, cinc, val, true);
		}
	return true;
}

//executes move if it is valid. Returns false and does not update board otherwise
//Makes computer make its move and returns the computer's move in row, and col
bool Wang_Zhou::play_square(int &row, int &col){
    	if((row ==0) && (col == 0))
	{
		this->my_color = 1;//we play black
	}
	else if ((row == -1) && (col == -1))
	{
		++this->pass;
	}
	else
	{
		this->play_square(row, col, 0-this->my_color); 
	}
	if (alpha_beta_search (this, this->my_color, 0-this->my_color, row, col))
	 pass = 0;
}

bool Wang_Zhou::full_board() {
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			if(squares[i][j]==0)
				return false;
	return true;
}

//returns score, positive for X player's advantage
int Wang_Zhou::score() {
	int sum =0;
	for(int i=0; i<8;i++)
		for(int j=0; j<8; j++)
			sum+=squares[i][j];
	return sum;
}

int Wang_Zhou::total_coins(){
	int sum =0;
        for(int i=0; i<8;i++)
                for(int j=0; j<8; j++)
                        sum+=abs(squares[i][j]);
        return sum;
}
int Wang_Zhou::get_square(int row, int col) {
	return squares[row-1][col-1];
}

bool make_simple_cpu_move(Wang_Zhou * b, int cpuval) {
	for(int i=1; i<9;i++)
		for(int j=1; j<9; j++)
			if(b->get_square(i, j)==0)
				if(b->play_square(i, j, cpuval)) 
					return true;
	cout << "Computer passes." << endl;
	return false;
}

/*
void Wang_Zhou::reset_square(int row, int col) {
	squares[row-1][col-1]=0;
}*/

int Evaluate(Wang_Zhou *b, int cpu)
{
	int rival_color = 0 - cpu;//if we play black(1), rival will play white(-1)
	//1st, we handle mobility
	double rival_move = b->count_valid_moves(rival_color);
	double cpu_move = b->count_valid_moves(cpu);
	double actual_mobility =0.0;
	if((rival_move + cpu_move)!=0)
	actual_mobility =100 * (cpu_move - rival_move) / (cpu_move + rival_move); 
	//2nd, we handle corner
	double corner_value = 0.0;
	double cpu_corner = 0.0;
	double rival_corner = 0.0;
	//upper left
	if (b->get_square(1,1)==cpu)
		cpu_corner +=cpu;
	else if (b->get_square(1,1)==rival_color)
		rival_corner +=rival_color;
	//lower left
	if (b->get_square(8,1)==cpu)
                cpu_corner +=cpu;
        else if (b->get_square(8,1)==rival_color)
		rival_corner +=rival_color;	
	//upper right
	if (b->get_square(1,8)==cpu)
                cpu_corner +=cpu;
        else if (b->get_square(1,8)==rival_color)
		 rival_corner +=rival_color;
	//lower right
	if (b->get_square(8,8)==cpu)
                cpu_corner +=cpu;
        else if (b->get_square(8,8)==rival_color)
		rival_corner +=rival_color;
	cpu_corner = abs(cpu_corner);
	rival_corner = abs(rival_corner);
	if ((cpu_corner+rival_corner)!= 0)
	corner_value = 100*(cpu_corner - rival_corner)/(cpu_corner + rival_corner);
	else corner_value =0;
	//double stability = 100*abs(b->score()) / b->total_coins();
	return (int)(actual_mobility + corner_value);
}
bool alpha_beta_search (Wang_Zhou * b, int cpu, int rival, int &row, int &col) {
	int best_score=-9999;//at the very beginning, best score is negative infinity
	int min=-9999;
	int max=9999;
	int max_row=0;
	int max_col=0;
	int level = 0;//a counter that used to check how many level we wanna check
	
	/*cout<< "******" <<endl 
	    << level
	    << endl
	    <<b->toString()
	    <<"*******"
	    <<endl; //test
	*/
	//Wang_Zhou *copy = new Wang_Zhou(*b);	
	for (int i = 1; i < 9; i++)
	{
		for (int j = 1; j < 9; j++)
		{
			Wang_Zhou *copy = new Wang_Zhou(*b);
			if(copy->get_square(i,j)==0){
				if(copy->play_square(i,j,cpu))
				{
				  int tmp_best = MinValue(copy, min, max, cpu, rival,++level);
                		  //b->reset_square(i,j);//redo the step
				  if (best_score < tmp_best){
					best_score=tmp_best;
					max_row=i;
					max_col=j;
				  }
				}
				else continue;
			}
		}
	}
	/*cout<< "******" <<endl 
            << level
            << endl
            <<b->toString()
            <<"*******"
            <<endl;*/ //test
	if(b->play_square(max_row,max_col,cpu))
	{
		row = max_row;
		col = max_col;
		return true;
	}
	cout << "Computer passes." <<endl;
	row = -1;
	col = -1;
	return false;
}

int MaxValue(Wang_Zhou * b, int min, int max, int cpuval,int humval, int level){
	if(b->full_board()||level >6)//> 8)//we only search 8 levels
          return Evaluate(b,cpuval);
        int themax = -9999;
	//Wang_Zhou *copy = new Wang_Zhou(*b); 
        for (int i = 1; i < 9; i++)
        {
                for (int j = 1; j < 9; j++)
                {
			Wang_Zhou *copy = new Wang_Zhou(*b);
			if(copy->get_square(i,j)==0){
				if (copy->play_square(i,j,cpuval))
				{
					int tmp=MinValue(copy, min, max, cpuval, humval,level+1);
					if(tmp > themax)
						themax=tmp;
					//b->reset_square(i,j);//redo
					if (themax >= max) 
					  return themax;
					else if(themax > min)
					  min = themax;
				}
				else continue;
			}
		}
	}
	return themax;
}

int MinValue( Wang_Zhou * b, int min, int max,int cpuval, int humval, int level){
	if(b->full_board()||level >6)//> 8)//we only search 8 levels
	  return Evaluate(b,cpuval);
	int themin = 9999;
	//Wang_Zhou *copy = new Wang_Zhou(*b);
	//cout << level <<endl << copy->toString(); //test
	for (int i = 1; i < 9; i++)
	{
		for (int j = 1; j < 9; j++)
		{
			Wang_Zhou *copy = new Wang_Zhou(*b);
			if(copy->get_square(i,j)==0){
				if(copy->play_square(i, j, humval))
				{
					int tmp_best= MaxValue(copy,min,max,cpuval,humval,level+1);
					if(tmp_best< themin)
						themin=tmp_best;
					//b->reset_square(i,j);//return one step
					if (themin <= min) 
					 return themin;
					else if(themin < max)
					 max = themin;
				}
				else continue;
			}
		}
	}
	return themin;
}


void play() {
	Wang_Zhou * b = new Wang_Zhou();
	int humanPlayer = 1;
	int cpuPlayer = -1;
	int r;
	int c;
	cout << b->toString();
	int consecutivePasses = 0;
	char which_first=0;
	cout<<endl<<"Please decide which one shall play first('h':human, 'c':computer)"<<endl;
	cin >> which_first;
	while(which_first!='h' &&which_first!='c')
	{
	 cout<<"Illegal input, please choose 'c' or 'h'"<<endl;
	 cin >> which_first;
	}
	if (which_first == 'c')
	{
	 humanPlayer = -1;
	 cpuPlayer = 1;
	 alpha_beta_search(b,cpuPlayer,humanPlayer,r,c);
	 cout<<b->toString();
	}
	while(!b->full_board() && consecutivePasses<2) {
		int row, col;
		//check if player must pass:
		if(!b->has_valid_move(humanPlayer)) {
			cout << "You must pass." << endl;
			consecutivePasses++;
		}
		else {
		//make_simple_cpu_move(b, humanPlayer);
		//alpha_beta_search(b,humanPlayer, cpuPlayer,r,c);
			
            		consecutivePasses = 0;
			cout << "Your move row (1-8): ";
			cin >> row;
			if(cin.fail()){
                 	std::cerr<< "Illegal move."<<endl;
                 	cin.clear();
                 	cin.ignore();
                 	continue;
            		}
			cout << "Your move col (1-8): ";
			cin >> col;
			if(cin.fail()){
                		std::cerr<< "Illegal move."<<endl;
                 		cin.clear();
                 		cin.ignore();
                 		continue;
            		}
			if(!b->play_square(row, col, humanPlayer)) {
                		cout << "Illegal move." << endl;
				continue;
            		}      
		}
		//move for computer:
		if(b->full_board())
			break;
		else {
			cout << b->toString() << "..." << endl;
			if(alpha_beta_search(b, cpuPlayer, humanPlayer,r,c))//(make_simple_cpu_move(b, cpuPlayer))
				consecutivePasses=0;
			else
				consecutivePasses++;
			cout << b->toString();
		}
	}
	int score = b->score();
	if(score==0)
		cout << "Tie game." << endl;
	else if(score>0)
		cout << "Black wins by " << score << endl;
	else if(score<0)
		cout << "White wins by " << -score << endl;
	//char a;
	//cin >> a;
	//if (a == 'q')
	//return 0;
}


int main(int argc, char * argv[])
{
	play();
	return 0;
}
