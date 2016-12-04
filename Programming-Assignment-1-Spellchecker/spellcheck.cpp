#include <iostream>
#include <string>
#include "hash.h"
#include <istream>
#include <fstream>
#include <ctime>

using namespace std;


unsigned int average_size = 50000;
hashTable dict(average_size);


// Convert upper case character to lower case
void lower_case(char &c){
	if( (c >= 65) && (c <= 90) ){ c += 32; }
}


// Check if the character is a digit
void check_digit(char &c, int &num_flag){
	if( (c >= 48) && (c <= 57) ) { num_flag = 1; }
}


// Load dictionary into hash table
void load_dict(string &dict_name){
	
	ifstream f_dict;
	f_dict.open(dict_name, ios::in);
	
	char curr_character;
	string curr_word;
	
	while (!f_dict.eof() ) {
			
		f_dict.get(curr_character);
		
		// If newline character, skip it
		if( curr_character == '\n' ){
			continue;
		}
		
		// If upper case, convert to lower case.
		lower_case(curr_character);
		
		curr_word += curr_character;
		
		// Insert word into hash table when newline is reached or EOF
		if  ( (f_dict.peek() == '\n') || (f_dict.peek() == -1)  ){
			dict.insert(curr_word);
			curr_word.clear();
			f_dict.get(curr_character);
		}
	}
}

// Spellcheck specified text document & write to output file
void spellCheck(string &in_file, string &out_file){

	// Open input file to be spell checked
	ifstream input_file;
	input_file.open(in_file, ios::in);
	
	// Open file for writing output to
	ofstream output_file;
	output_file.open(out_file);
	
	
	char curr_character;
	string curr_word;
	
	// Flag that specifies if the current word has a digit in it
	int num_flag = 0;
	int line_number = 1;

	while ( !input_file.eof() ) {
		
		input_file.get(curr_character);
		
		// If upper case, convert to lower case.
		lower_case(curr_character);

		// Check to see if current character is a digit & update num_flag
		check_digit(curr_character, num_flag);
		
		// Checks to see if the character is invalid
		if  (!( ((curr_character == 39) || (curr_character == 45)) ||
				((curr_character >= 48) && (curr_character <= 57)) || 
				((curr_character >= 97) && (curr_character <= 122)))) {
			
			int curr_word_line = line_number;
			if( curr_character == '\n' ){ line_number++; }
			
			// If the current word has a digit in it or is empty, we ignore & proceed
			if( (num_flag == 1) || (curr_word.size() == 0) ){
				curr_word.clear();
				num_flag = 0;
				continue;
			}
			
			// Check to see if the current word is greater than 20 characters
			if( curr_word.size() > 20 ){
				output_file << "Long word at line " << curr_word_line << flush;
				output_file<< ", starts: " << curr_word.substr(0, 20) << endl;
			}
			
			// Check to see if the current word is in the dictionary
			else if( !dict.contains(curr_word) ){
				output_file << "Unknown word at line " << curr_word_line << ": " << flush;
				output_file << curr_word << endl;
			}
			curr_word.clear();
			num_flag = 0;
		}
		
		// If the character is valid, append to the current word
		else{ curr_word += curr_character; }	
	}
}


int main() {  
	
	// Request user to enter name of dictionary to be used
	string dictionary_name;
	cout << "Enter name of dictionary: " << flush;
	cin >> dictionary_name;
	
	// Load in dictionary
	clock_t t1 = clock();
	load_dict(dictionary_name);
	clock_t t2 = clock();
	double time_diff = ((double)(t2 - t1)) / CLOCKS_PER_SEC;
	
	cout << "Total time (in seconds) to load dictionary: " << time_diff << endl;
	
	// Request user to enter names of input & output files
	string input_name;
	cout << "Enter name of input file: " << flush;
	cin >> input_name;
	
	string output_name;
	cout << "Enter name of output file: " << flush;
	cin >> output_name;
	
	// Spell check document & write to output file
	t1 = clock();
	spellCheck(input_name, output_name);
	t2 = clock();
	time_diff = ((double)(t2 - t1)) / CLOCKS_PER_SEC;
	
	cout<< "Total time (in seconds) to check document: "<< time_diff << endl;

	return 0;
}