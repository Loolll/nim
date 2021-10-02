#include <iostream>
#include <vector>
#include <climits>
#include <string>
#include <cmath>
#include <bitset>

using namespace std;

// cin clean & repair
void cin_reload(){
    cin.ignore(INT_MAX, '\n');
    cin.clear();
}

// returns binary string of decimal number (only int datatype) 
string decimal_to_binary(int number){
    bitset<32> temp{unsigned(number)};
    string result = "";
    for(int degree=32; degree >= 0; --degree)
        result += temp[degree] ? '1' : '0';
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
