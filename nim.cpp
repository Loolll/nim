#include <iostream>
#include <vector>
#include <climits>
#include <string>
#include <cmath>

using namespace std;
#define INIT {3,4,5} // we can add the new stack of chips or modify their count

// returns binary string of decimal number (only int datatype) 
string decimal_to_binary(int number){
    string result = "";
    for(int degree=32; degree >= 0; degree--){
        int exist = number/(pow(2,degree));
        number -= exist*(pow(2,degree));
        result += to_string(exist);
    }
    return result;
}

// returns decimal number corresponding to a binary string 
int binary_to_decimal(string binary){
    int result = 0;
    for(int degree=0; degree<binary.size(); degree++){
        int index = binary.size()-degree-1;
        result += int(binary[index]-'0')*pow(2, degree);
    }
    return result;
}

// returns string as number digits sum
int string_sum(string str){
    int sum = 0;
    for(auto digit: str){
        sum+= int(digit-'0');    
    }
    return sum;
}

// returns XOR of all binary (in vector) numbers. All numbers must be the same len.
string binary_from_vector_xor(vector<string>& numbers){
    string result = "";
    for(int index=0; index<numbers[0].size(); index++){
        int sum = 0;
        for(string binval: numbers)sum += int(binval[index]);
        result += to_string(sum%2);
    }
    return result;
}

// inits rows vector
void _init(vector<int>& rows){
    rows = INIT;
}

// returns true if game can be continued (sum of chips > 0) and false if not
bool _game_continue(vector<int>& rows){
    int sum{0};
    for(int i: rows) sum+=i;
    return (sum!=0);
}

// prints info of rows with chips in format:
// count_of_chips_in_first_row count_of_chips_in_second_row ...
void _info_print(vector<int>& rows){
    for(int i: rows) cout << i << " ";
}

// checks valid of move
// move struct {row from first index, count}
void move_valid(vector<int>& rows, vector<int>& move){
    if(move[0] < 1 or move[0] > rows.size()) throw runtime_error{"Invalid row index."};
    if(move[1] < 1 or move[1] > rows[move[0]-1]) throw runtime_error{"Invalid count of chips."};
}

// cin wrapper. result is input vector {row from first index, count}
// handles all cin exeptions
void input_move(vector<int>& rows, vector<int>& input){    
    try{
        cout << "\nMake your move\n";
        int number, count;
        if(!(cin >> number >> count))
            throw runtime_error{"Only numbers!"}; // check valid of data type
        input = {number, count};
        move_valid(rows, input); // check valid of row number and existing of the chips
    }
    catch(runtime_error& err){
        cout << err.what() << "\nPlease type valid move. Now situatuion is: ";
        _info_print(rows);
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        input_move(rows, input);
    }
}

// calculates move
// move struct {row from first index, count}
void _move_calculate(vector<int>& rows, vector<int>& move){
    rows[move[0]-1] -= move[1];
}

// makes your move from cin input in format *row_index_from_first count_of_chips*
void your_move(vector<int>& rows){
    vector<int> input;
    input_move(rows, input);
    _move_calculate(rows, input);
}

// solutes binary equation for find deduction binary line and number of row to make effect.
// returns string vector {row_number_from_first_index, deducate_line}
vector<string> binary_equation_solution(vector<string>& binary_rows){
    // equation for ex:
    // 0011 + deduction_effect_1
    // (XOR)
    // 0100 + deduction_effect_2
    // (XOR)
    // 0101 + deduction_effect_3
    // (XOR)
    // = 0000
    // answer -- deduction_effect_1 = "0010"; deduction_effect_2, 3 = 0
    // return (1, 0010)
    string differance = binary_from_vector_xor(binary_rows); // calculating first XOR
    for(int try_index=0; try_index<binary_rows.size(); try_index++){ // try to use this string 
        string try_string{binary_rows[try_index]}; // string which try to deformate
        string new_string = "";
        // calculating the new string
        for(int symbol_index=0; symbol_index<differance.size(); symbol_index++){
            if(differance[symbol_index]=='1'){
                if(try_string[symbol_index]=='0'){new_string+="1";}
                else {new_string+="0";}
            }
            else{new_string += try_string[symbol_index];}
        }
        // try to use new string instead of real
        vector<string> new_binary_rows(binary_rows);
        new_binary_rows[try_index] = new_string;
        string new_xor = binary_from_vector_xor(new_binary_rows);
        if(string_sum(new_xor)==0 && (
            binary_to_decimal(new_string) <= binary_to_decimal(try_string))){
            // if okey return row number and deducate line
            string deducate_line = decimal_to_binary(
                binary_to_decimal(try_string)-binary_to_decimal(new_string));
            vector<string> result = {to_string(try_index+1), deducate_line};
            return result;
        }
    }
    // every equation will have solution. If not this is exception!
    throw runtime_error{"WRONG ARGS"};
}

// use alg to find the best move
void find_optimal_move(vector<int>& rows, vector<int>& move){
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

    move = {row_of_the_solution, deduct_chips};
}

// deals computer move
void comp_move(vector<int>& rows){
    //your_move(rows); // plug with your move
    vector<int> move;
    find_optimal_move(rows, move);
    cout << "\nComputer makes move:... " << move[0] << " " << move[1] << "\n";
    _move_calculate(rows, move);
}

// starts game
// return true if you win and false if computer
bool game(){ 
    vector<int>rows;
    _init(rows);
    _info_print(rows);
    while(true){
        // your move
        your_move(rows);
        _info_print(rows);
        if(!_game_continue(rows)) return true; 
        
        // comp move
        comp_move(rows);
        _info_print(rows);
        if(!_game_continue(rows)) return false;
        
    }
}

int main(){
    try{
        bool play = true;
        while(play){
            bool result = game();
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