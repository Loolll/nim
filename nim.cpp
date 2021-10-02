#include <iostream>
#include <vector>
#include <climits>
#include <string>
#include <cmath>
#include <sstream>
#include "tools.h"

using namespace std;

// TODO swap cin >> to getlines

// we can add the new stack of chips or modify their count.
//This options will be used in default field generation.
#define BASE_ROWS {3,4,5}
#define STRING_ROWS "{3,4,5}"


class Move{
    private:
        vector<int>& rows;
    public:
        int row_index;
        int chips_count;

        // creates current move with own row index and chips count
        Move(int _row_index, int _chips_count, vector<int>& _rows) 
            : rows{_rows}
        {
            row_index = _row_index;
            chips_count = _chips_count;
        }

        // creates empty move
        Move(vector<int>& _rows): rows{_rows} {}

        // return true if move is valid.
        bool is_valid()const{
            if(row_index < 1 or row_index > rows.size()) return false;
            if(chips_count < 1 or chips_count > rows[row_index-1]) return false;
            return true;
        }

        // cout info about rows
        void print_info()const{
            for(int i: rows) cout << i << " ";
        }

        // interactive move input.
        // cin wrapper. handles all cin exeptions.
        void interactive_input(){    
            try{
                cout << "\nMake your move like: ROW COUNT\n";
                if(!(cin >> row_index >> chips_count))
                    throw runtime_error{"Only numbers!"}; // check valid of data type
                if(!is_valid())throw runtime_error{"Invalid move"}; // check valid of row number and existing of the chips
            }
            catch(runtime_error& err){
                cout << "\n" << err.what() << "\nPlease type valid move. Now situatuion is: ";
                print_info();
                cin_reload();
                interactive_input();
            }
        }

        // use alg to find the best move
        void find_optimal_move(){
            vector<string> binary_rows{};
            for(int number: rows)
                binary_rows.push_back(decimal_to_binary(number));
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
        
        // tests
        static void tests(){
            // test case 1
            // 3 4 5
            // 1 4 5
            // 0 4 5
            // 0 4 4
            // 0 4 0
            // 0 0 0
            vector<int> rows_case_1{3,4,5}; 
            Move move{rows_case_1};
            move.find_optimal_move();
            move.calculate();
            if(rows_case_1 != vector<int>{1,4,5}) throw runtime_error{"Move tests case 1 move 1 error"};
            move.row_index = 1;
            move.chips_count = 1;
            move.calculate();
            if(rows_case_1 != vector<int>{0,4,5}) throw runtime_error{"Move tests case 1 move 2 error"};
            move.find_optimal_move();
            move.calculate();
            if(rows_case_1 != vector<int>{0,4,4}) throw runtime_error{"Move tests case 1 move 3 error"};
            move.row_index = 3;
            move.chips_count = 4;
            move.calculate();
            if(rows_case_1 != vector<int>{0,4,0}) throw runtime_error{"Move tests case 1 move 4 error"};
            move.find_optimal_move();
            move.calculate();
            if(rows_case_1 != vector<int>{0,0,0}) throw runtime_error{"Move tests case 1 move 5 error"};
            
            // test case 2
            // 7 2 9 1
            // 7 2 4 1
            // 5 2 4 1
            // 5 0 4 1
            // 4 0 4 1
            // 4 0 4 0
            vector<int> rows_case_2{7,2,9,1};
            Move move2{rows_case_2};
            move2.find_optimal_move();
            move2.calculate();
            if(rows_case_2 != vector<int>{7,2,4,1}) throw runtime_error{"Move tests case 2 move 1 error"};
            move2.row_index = 1;
            move2.chips_count = 2;
            move2.calculate();
            if(rows_case_2 != vector<int>{5,2,4,1}) throw runtime_error{"Move tests case 2 move 2 error"};
            move2.find_optimal_move();
            move2.calculate();
            if(rows_case_2 != vector<int>{5,0,4,1}) throw runtime_error{"Move tests case 2 move 3 error"};
            move2.row_index = 1;
            move2.chips_count = 1;
            move2.calculate();
            if(rows_case_2 != vector<int>{4,0,4,1}) throw runtime_error{"Move tests case 2 move 4 error"};
            move2.find_optimal_move();
            move2.calculate();
            if(rows_case_2 != vector<int>{4,0,4,0}) throw runtime_error{"Move tests case 2 move 5 error"};
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
            cout << "\nComputer makes move:...\n" << move.row_index << " " << move.chips_count << "\n";
            move.calculate();
        }

        // checks valid of rows.
        // Throws the exceptions.
        void rows_valid(){
            if(rows.empty()) throw runtime_error{"Empty rows!"};
            for(int stack: rows) if(stack <= 0) throw runtime_error{"Numbers must be only pozitive!"};
            if(is_finished()) throw runtime_error{"Empty stacks!"};
        }

        // cin wrapper. handles all exeptions
        void custom_rows_input(){
            try{
                //cin_reload(); // Need to clear cin buffer
                cout << "\nWrite stacks like:\n3 4 5\nIt will create game with three rows: 3 4 5\n";
                vector<int> _rows{};
                string input_string;
                getline(cin, input_string, '\n');
                stringstream words(input_string);
                int number;
                while(words >> number) _rows.push_back(number);
                rows = _rows;
                rows_valid();
            }
            catch(runtime_error& ex){
                cout << "\n" << ex.what() << " Please type valid stacks line.\n";
                custom_rows_input();
            }
            
        }

        // interactive initialization of rows
        void interactive_init(){
            cout << "\nDo you want to play custom nim?(y/n). If not, rows'll be " << STRING_ROWS << "\n";
            string temp;
            getline(cin, temp, '\n');
            if(temp == "y" || temp == "Y") custom_rows_input();
            else rows = BASE_ROWS;
        }
    public:
        // Game inition for current rows (in param)
        Game(vector<int> _rows){
            rows = _rows;
        }
        
        // Game inition with user preferences
        Game(){
            interactive_init();
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
            cout << "\nStart! \n";
            print_info();
            while(true){
                your_move(); // your move
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
        Move::tests(); // test Move class
        bool play = true;
        while(play){
            Game game = Game();
            bool result = game.play();
            if(result) cout << "\nGood job! You won!\n";
            else cout << "\nComputer won :( Try again!\n";
            
            cout << "\nDo you want to play again? (y/n)\n";
            string temp;
            cin >> temp;
            if(temp != "y" and temp != "Y") play = false;
        }
    }
    catch(runtime_error& ex){
        cerr << "\n" << ex.what() << "\n";
        return 2; 
    }
    catch(...){
        cerr << "\nUnexpected error!\n";
        return 3;
    }
}


//end