#include <iostream>
#include <vector>
#include <climits>
#include <string>
#include <cmath>
#include "tools.h"

using namespace std;
#define INIT {3,4,5,6,7} // we can add the new stack of chips or modify their count


class Move{
    private:
        vector<int>& rows;
    public:
        int row_index;
        int chips_count;
        Move(int _row_index, int _chips_count, vector<int>& _rows) 
            : rows{_rows}
        {
            row_index = _row_index;
            chips_count = _chips_count;
        }

        Move(vector<int>& _rows): rows{_rows} {}

        // return true if move is valid.
        bool is_valid(){
            if(row_index < 1 or row_index > rows.size()) return false;
            if(chips_count < 1 or chips_count > rows[row_index-1]) return false;
            return true;
        }

        // cout info about rows
        void print_info(){
            for(int i: rows) cout << i << " ";
        }

        // cin wrapper. result is input vector {row from first index, count}
        // handles all cin exeptions
        void interactive_input(){    
            try{
                cout << "\nMake your move\n";
                int number, count;
                if(!(cin >> number >> count))
                    throw runtime_error{"Only numbers!"}; // check valid of data type
                row_index = number;
                chips_count = count;
                if(!is_valid())throw runtime_error{"Invalid move"}; // check valid of row number and existing of the chips
            }
            catch(runtime_error& err){
                cout << err.what() << "\nPlease type valid move. Now situatuion is: ";
                print_info();
                cin.clear();
                cin.ignore(INT_MAX, '\n');
                interactive_input();
            }
        }

        // use alg to find the best move
        void find_optimal_move(){
            vector<string> binary_rows{};
            for(int number: rows) binary_rows.push_back(decimal_to_binary(number));
            vector<string> result_of_the_solution = binary_equation_solution(binary_rows);
            int row_of_the_solution = stoi(result_of_the_solution[0]); // int from string val
            int deduct_chips = binary_to_decimal(result_of_the_solution[1]); // int from binary string

            // if our position is winning, computer will delete one chip from first not empty row
            if(deduct_chips == 0){
                for(int i=0; i<rows.size();i++){
                    if(rows[i]!=0){
                        deduct_chips = 1;
                        row_of_the_solution = i+1;
                        break;
                    }
                }
            }

            row_index = row_of_the_solution;
            chips_count = deduct_chips;
        }

        // calculates move
        void calculate(){
            if(is_valid()) rows[row_index-1] -= chips_count;
            else throw runtime_error{"Invalid move"};
        }
};

class Game{
    private:
        vector<int> rows;
        
        // makes your move from cin input in format *row_index_from_first count_of_chips*
        void your_move(){
            Move move = Move(rows);
            move.interactive_input();
            move.calculate();
        }

        // deals computer move
        void comp_move(){
            Move move = Move(rows);
            move.find_optimal_move();
            cout << "\nComputer makes move:... " << move.row_index << " " << move.chips_count << "\n";
            move.calculate();
        }
    public:
        Game(vector<int> _rows){
            rows = _rows;
        }

        // returns true if game is finished
        bool is_finished(){
            int sum{0};
            for(int i: rows) sum+=i;
            return (sum==0);
        }

        // cout info about rows
        void print_info(){
            for(int i: rows) cout << i << " ";
        }

        // starts game, returns true if you won, false if comp
        bool play(){
            print_info();
            while(true){
                your_move();// your move
                print_info();
                if(is_finished()) return true; 
                
                comp_move(); // computer move
                print_info();
                if(is_finished()) return false;
            }
        }
};


int main(){
    try{
        bool play = true;
        vector<int> ROWS INIT;
        while(play){
            Game game = Game(ROWS);
            bool result = game.play();
            if(result) cout << "\nGood job! You won!\n";
            else cout << "\nComputer won :( Try again!\n";
            
            cout << "\nDo you want to play again? (y/n)\n";
            string temp;
            cin >> temp;
            if(temp != "y" and temp != "Y") play = false;
        }
    }
    catch(...){
        cerr << "Unexpected error!";
    }
}


//end