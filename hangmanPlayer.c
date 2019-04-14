/*

  Authors (group members):
  Email addresses of group members:
  Group name:

  Course:
  Section:

  Description of the overall algorithm:


*/

#include "hangmanPlayer.h"
#include <stdio.h>
#include <string.h>

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
  char guess = ' ';

  
  return guess;
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