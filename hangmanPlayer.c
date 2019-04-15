/*

  Authors (group members): Taylor (Leader), Noah Wilson, Matt, James
  // Email addresses of group members: wilsonn2018@my.fit.edu, 
  Group name: Dream Team

  Course: CSE 2010
  Section: 23

  Description of the overall algorithm: Lit af algorithm. Predicts the date of the end of the world


*/

#include "hangmanPlayer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Primary{
	int word_count;
	int letter_freq[26];
	int *array_loc;
}Primary;

// initialize data structures from the word file
void init_hangman_player(char* word_file)
{
	
	
}

// based on the current (partially filled or intitially blank) word, guess a letter
// current_word: current word -- may contain blanks, terminated by a null char as usual
// is_new_word: indicates a new hidden word (current_word has blanks)
// returns the guessed letter
// Assume all letters in a word are in lower case
char guess_hangman_player(char* current_word, bool is_new_word)
{
	//Initialize variables
	char guess = ' ';
	int check = 0; //The int that stores the index of the frequency we are checking
	int best_guess = 0; //The variable that stores the index of the highest frequency letter
	int length = strlen(current_word); //The length of the word
	
	// The for loop loops through every letter frequency of PMS[word_length] and compares the frequency of that letter
	// with the frequency of the next letter. Whichever is greater, the index of that letter frequency is stored to best_guess.
	for(int i = 0; i < 26; i++){
		if(PMS[length].letter_freq[i] > PMS[length].letter_freq[check]){
			best_guess = i;
		}
		check = i;
	}
	
	//This switch case sets the letter based on the cooresponding index. i.e. (1=a) (2=b) (3=c)
	switch(best_guess){
		case 1: guess = 'a';
				break;
		case 2: guess = 'b';
				break;
		case 3: guess = 'c';
				break;
		case 4: guess = 'd';
				break;
		case 5: guess = 'e';
				break;
		case 6: guess = 'f';
				break;
		case 7: guess = 'g';
				break;
		case 8: guess = 'h';
				break;
		case 9: guess = 'i';
				break;
		case 10: guess = 'j';
				 break;
		case 11: guess = 'k';
				 break;
		case 12: guess = 'l';
				 break;
		case 13: guess = 'm';
				 break;
		case 14: guess = 'n';
				 break;
		case 15: guess = 'o';
				 break;
		case 16: guess = 'p';
				 break;
		case 17: guess = 'q';
				 break;
		case 18: guess = 'r';
				 break;
		case 19: guess = 's';
				 break;
		case 20: guess = 't';
				 break;
		case 21: guess = 'u';
				 break;
		case 22: guess = 'v';
				 break;
		case 23: guess = 'w';
				 break;
		case 24: guess = 'x';
				 break;
		case 25: guess = 'y';
				 break;
		case 26: guess = 'z';
				 break;
		default: return(guess);
	}
	
	return(guess); //Returns the new guess
	
}

// feedback on the guessed letter
// is_correct_guess: true if the guessed letter is one of the letters in the hidden word
// current_word: partially filled or blank word
//   
// Case       is_correct_guess    current_word   
// a.         true                partial word with the guessed letter
//                                   or the whole word if the guessed letter was the
//                                   last letter needed
// b.         false               partial word without the guessed letter
void feedback_hangman_player(bool is_correct_guess, char* current_word)
{
	//James' work
	if(is_correct_guess==true){
		rewrite_letter_freq(current_word);
	}
	else{
		//guess again
		
}

void rewrite_letter_freq(char *current_word){
	
	//go to primary array with word len=current_word len
	int len=strlen(current_word);
	int counter=0;
	char copy[len];
	
	
	for(int i=0;i<PMS[len].word_count;i++){
		strcpy(copy, PMS[len].array_loc[i];
		for(int k=0;k<len;k++){
			if(current_word[k]!=NULL){
				if(current_word[k]==copy[k]){
					strcpy(PMS[len].array_loc[counter],copy);
					counter++;
				}
			}
			counter=0;
		}
	}
	
}