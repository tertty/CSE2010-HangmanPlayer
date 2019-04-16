/*

  Authors (group members): Taylor Ertrachter (Leader), Noah Wilson, Matthew Dawson, James Spies
  // Email addresses of group members: wilsonn2018@my.fit.edu, 
  Group name: ~Dream Team~

  Course: CSE 2010
  Section: 23

  Description of the overall algorithm: Lit af algorithm. Predicts the date of the end of the world


*/

#include "hangmanPlayer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define max_word_len 27

typedef struct PRIMARY{
	int word_count;
	int letter_freq[26];
	int long *array_loc;
}PRIMARY;

  // Create overarching structure "PRIMARY MANAGMENT STRUCT"
  // Even if max_word_len is much over the actual max word, the wasted data
  // is neglagible since we find the acutal longest len before we malloc the larger file location arrays
  PRIMARY PMS[max_word_len];

void WRITE(FILE *MASTER_FILE, PRIMARY *PMS);
void T_PRINT(FILE *MASTER_FILE, PRIMARY *PMS);
void rewrite_letter_freq(char *current_word);
void T_FREQ(PRIMARY *PMS);
// initialize data structures from the word file
void init_hangman_player(char* word_file)
{
	
	  int i, j;
  FILE * MASTER_FILE;

  // !!!!! MASTER FILE MUST ALWAYS STAY ASSINGED TO ITS CURRENT MEM ADDRESS
  // !!!!!! i.e. never fclose this unless a win or loss occcurs
  if((MASTER_FILE = fopen(word_file, "r")) == 0){
    printf("incorrect file 1 name \n");
    fclose(MASTER_FILE);
    return ;
  }

  for(i=0;i<max_word_len;i++){
    PMS[i].word_count = 0;
    PMS[i].array_loc = 0;
    for(j=0;j<26;j++){
      PMS[i].letter_freq[j] =0;
    }
  }
  
  WRITE(MASTER_FILE, PMS);
  T_PRINT(MASTER_FILE, PMS);
  T_FREQ(PMS);
  fseek(MASTER_FILE, 0, SEEK_SET);
  printf("\n");
  
  return ;
	
}

void WRITE(FILE *MASTER_FILE, PRIMARY *PMS) {
	
  int i = 0;
  int j = 0;
  int k = 0;
  int flag = 0;
  int l_var = 0;
  int true_max_len = 0;   // to find the "actual" longest word
  int cur_len = 0;
  int long pos;
  char c_word[max_word_len]; //current word scaned in, not the other word you pervert





  // loop through entire word list
  // For each word of length n, get its total number of occurence so we can malloc array after
  // Get the total frequency of letters occuring for each
  /////   // We just need lenght of words not to copy as entire string them has to be a faster way
  /////   // to get just the lenght should be able to just ++ char untill '\0' ??
  while (fscanf(MASTER_FILE, "%s", c_word) == 1) {

    // find the lenght of the word
    cur_len = strlen(c_word);

    if (cur_len > true_max_len) {
      true_max_len = cur_len;
    }

    // Increment total amount of words for the found length
    // Must subtract to offset index
    PMS[cur_len - 1].word_count++;
    printf("Len: %d Word: %s Len_Occurance:%d\n", cur_len, c_word, PMS[cur_len - 1].word_count);

  }
  /////////////////////////////
  /// END OF FIRST SCAN TO FIND
  /// 1. True Max Length
  /// 2. Find word totals for each length
  /// ///////////////////////////// ///////////////////////////// /////////////////////////////
  // Create arrays to fill w/ the file position of word w/ coresponding length. We use true max to save memory
  // in case max_word_len will liekly be larger
  for (i = 0; i < true_max_len; i++) {
    printf("%d", PMS[i].word_count);
    PMS[i].array_loc = (int long *) malloc(PMS[i].word_count * sizeof(int long));
    printf("\n");
  }
  ///////////////////////////// Loop through for each length,  /////////////////////////////
  // Here we are adding in the locations of the word, and generating frequency

  for (i = 0; i < true_max_len; i++) {
    // Reset the file pointer to the begining position
    fseek(MASTER_FILE, 0, 0);
    k = 0;
    flag = 0;
    pos = ftell(MASTER_FILE);
    fscanf(MASTER_FILE, "%s", c_word);
    while (flag != -1) {
      printf("**%s**\n", c_word);
      if (((cur_len = strlen(c_word)) == (i + 1))){
        c_word[0] = tolower(c_word[0]);
        //pos = ftell(MASTER_FILE);
        PMS[i].array_loc[k] = pos;
        printf("Struct[%d].array_loc[%d] Word: %s Posn: %ld \n", i, k, c_word, pos);
        k++;

        // Get frequency
        // to avoid massive swith statement for each case of a, b, c, d ect
        // convert letter to integer, subtract from asci value of a = 97
        // ex. b=98, 98-97 = 1, index 1 (correlates to b) is incremented
        // leave l_var as is, some reason you cant subtract 97 on same line...

        for (j = 0; j <= i; j++) {
          l_var = c_word[j];
          l_var = l_var - 97;
          printf("%c - a = %d \n", c_word[j], l_var);
          PMS[cur_len-1].letter_freq[l_var]++;
        }
        printf("\n");
      }

      pos = ftell(MASTER_FILE);
      if (fscanf(MASTER_FILE, "%s", c_word) != 1) {
        flag = -1;
        printf("c_word: %s \n", c_word);
      }

    }
    printf("\n\n\n");
  }
  
  return ;
}


void T_PRINT(FILE *MASTER_FILE, PRIMARY *PMS){
  int i = 0,j;
  int long pos;
  char temp[max_word_len];

  while (i < max_word_len){

    for(j=0;j<PMS[i].word_count;j++) {
      pos = PMS[i].array_loc[j];
      fseek(MASTER_FILE, pos, 0);
      fscanf(MASTER_FILE, "%s\n", temp);
      printf("Struct[%d].array_loc[%d] total: %d Word: %s Posn %ld \n",i, j, PMS[i].word_count, temp, pos);
    }

    i++;
  }
  
  return ;

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
		guess_hangman_player(current_word, false);
	}
}

void rewrite_letter_freq(char *current_word){
	
	//go to primary array with word len=current_word len
	int len=strlen(current_word);
	int counter=0;
	char copy[len];
	
	
	for(int i=0;i<PMS[len].word_count;i++){
		fseek(MASTER_FILE, PMS[len].array_loc[i], 0);
		fscanf(MASTER_FILE, "%s", copy);
		for(int k=0;k<len;k++){
			if(current_word[k]==copy[k]){
				strcpy(PMS[len].array_loc[counter],copy);
				counter++;
			}
			counter=0;
		}
	}
	
	for(int p = 0; p<PMS[len].word_count; p++){
		fseek(MASTER_FILE, PMS[len].array_loc[i], 0);
		fscanf(MASTER_FILE, "%s", copy);
		for(int q = 0; q<len; q++){
			switch(q){
				case 'a': PMS[len].letter_freq[0]++;
						  break;
				case 'b': PMS[len].letter_freq[1]++;
						  break;
				case 'c': PMS[len].letter_freq[2]++;
						  break;
				case 'd': PMS[len].letter_freq[3]++;
						  break;
				case 'e': PMS[len].letter_freq[4]++;
						  break;
				case 'f': PMS[len].letter_freq[5]++;
						  break;
				case 'g': PMS[len].letter_freq[6]++;
						  break;
				case 'h': PMS[len].letter_freq[7]++;
						  break;
				case 'i': PMS[len].letter_freq[8]++;
						  break;
				case 'j': PMS[len].letter_freq[9]++;
						  break;
				case 'k': PMS[len].letter_freq[10]++;
						  break;
				case 'l': PMS[len].letter_freq[11]++;
						  break;
				case 'm': PMS[len].letter_freq[12]++;
						  break;
				case 'n': PMS[len].letter_freq[13]++;
						  break;
				case 'o': PMS[len].letter_freq[14]++;
						  break;
				case 'p': PMS[len].letter_freq[15]++;
						  break;
				case 'q': PMS[len].letter_freq[16]++;
						  break;
				case 'r': PMS[len].letter_freq[17]++;
						  break;
				case 's': PMS[len].letter_freq[18]++;
						  break;
				case 't': PMS[len].letter_freq[19]++;
						  break;
				case 'u': PMS[len].letter_freq[20]++;
						  break;
				case 'v': PMS[len].letter_freq[21]++;
						  break;
				case 'w': PMS[len].letter_freq[22]++;
						  break;
				case 'x': PMS[len].letter_freq[23]++;
						  break;
				case 'y': PMS[len].letter_freq[24]++;
						  break;
				case 'z': PMS[len].letter_freq[25]++;
						  break;
			}
		}
	}
	
}


void
T_FREQ(PRIMARY *PMS){
  int i = 0,j;
  int long pos;
  char temp[max_word_len];

  while (i < max_word_len){
    printf("Total Words: %d\n",PMS[i].word_count);
    for(j=0;j<26;j++) {
      printf("Struct[%d].letter_freq[%d] letter: %c total %d \n",i, j, 'a'+j, PMS[i].letter_freq[j]);
    }
    printf("\n");
    i++;
  }

}