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

// returns bitset of int positive number
bitset<32> decimal_to_binary(int number){
    return bitset<32>{unsigned(number)};
}

// returns decimal number corresponding to a binary set
int binary_to_decimal(bitset<32> binary){
    int result = (int)(binary.to_ulong());
    return result;
}


// returns count of 1 in binary set
int binary_digit_sum(bitset<32> binary){
    int sum = 0;
    for(int i=0; i<binary.size(); i++) sum += binary[i];
    return sum;
}


// returns XOR of module 2 of all bitset (in vector) numbers.
bitset<32> binary_from_vector_xor(vector<bitset<32>> numbers){
    bitset<32> result;
    for(int index=0; index<numbers[0].size(); index++){
        int sum = 0;
        for(bitset<32> binary: numbers) sum += binary[index];
        result[index] = sum%2;
    }
    return result;
}

// solutes binary equation for find deduction binary line and number of row to make effect.
// returns int vector {row_number_from_first_index, deducate_count}
vector<int> binary_equation_solution(vector<int> rows){
    vector<bitset<32>> binary_rows{};
    for(int number: rows)
        binary_rows.push_back(decimal_to_binary(number));
    bitset<32> differance = binary_from_vector_xor(binary_rows); // calculating first XOR
    for(int try_index=0; try_index<binary_rows.size(); try_index++){ // try to use this row 
        bitset<32> try_bitset{binary_rows[try_index]}; // bitset which try to deformate
        bitset<32> new_bitset;
        // calculating the new bitset
        for(int symbol_index=0; symbol_index<differance.size(); symbol_index++){
            if(differance[symbol_index]==1){
                if(try_bitset[symbol_index]==0){new_bitset[symbol_index]=1;}
                else {new_bitset[symbol_index]=0;}
            }
            else{new_bitset[symbol_index] = try_bitset[symbol_index];}
        }
        // try to use new bitset instead of real
        vector<bitset<32>> new_binary_rows(binary_rows);
        new_binary_rows[try_index] = new_bitset;
        bitset<32> new_xor = binary_from_vector_xor(new_binary_rows);
        if(binary_digit_sum(new_xor)==0 && (
            binary_to_decimal(new_bitset) <= binary_to_decimal(try_bitset))){
            // if okey return row number and deducate count
            int deducate_count = binary_to_decimal(try_bitset)-binary_to_decimal(new_bitset);
            vector<int> result = {try_index+1, deducate_count};
            return result;
        }
    }
    // every equation will have solution. If not this is exception!
    throw runtime_error{"WRONG ARGS"};
}