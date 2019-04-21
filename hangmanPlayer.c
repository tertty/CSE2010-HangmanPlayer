/*

  // Try JUST GUESSING E for fist letter, increased other ppls score a shit ton
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
  int *array_loc;
}PRIMARY;

  // Create overarching structure "PRIMARY MANAGMENT STRUCT"
  // Even if max_word_len is much over the actual max word, the wasted data
  // is neglagible since we find the acutal longest len before we malloc the larger file location arrays
  PRIMARY PMS[max_word_len];
  PRIMARY TEMP;
  int LETTER_ARR[26];
  FILE * MASTER_FILE;

void WRITE(FILE *MASTER_FILE, PRIMARY *PMS);
void T_PRINT(FILE *MASTER_FILE, PRIMARY *PMS);
int rewrite_letter_freq(char *current_word);
void T_FREQ(PRIMARY *PMS, int);
// initialize data structures from the word file
void init_hangman_player(char* word_file){
int i, j;

  // !!!!! MASTER FILE MUST ALWAYS STAY ASSINGED TO ITS CURRENT MEM ADDRESS
  // !!!!!! i.e. never fclose this unless a win or loss occcurs
  if((MASTER_FILE = fopen(word_file, "r")) == 0){
    //printf("incorrect file 1 name \n");
    fclose(MASTER_FILE);
    return ;
  }

  //INITIALIZE LETTER FREQUENCY
  for(i=0;i<max_word_len;i++){
    PMS[i].word_count = 0;
    PMS[i].array_loc = 0;
    //INITIALIZE LETTER FREQUENCY
    for(j=0;j<26;j++){
      PMS[i].letter_freq[j]=0;
    }
  }

  //INITIALIZE TEMP STRUCTURE THAT WILL CONTAIN INTIAL VALUES
  // OF PMS[I], THEN WILL BE MODIFY/RESORTED
  TEMP.word_count = 0;
  TEMP.array_loc = 0;
  for(j=0;j<26;j++){
    TEMP.letter_freq[j]=0;
  }

  WRITE(MASTER_FILE, PMS);
  //T_PRINT(MASTER_FILE, PMS);
  //T_FREQ(PMS);
  fseek(MASTER_FILE, 0, SEEK_SET);
  //printf("\n");

  return ;

}

void WRITE(FILE *MASTER_FILE, PRIMARY *PMS) {

  int i = 0;
  int j = 0;
  int k = 0;
  int flag = 0;
  int l_var = 0;
  int true_max_len = 0;   // to find the "actual" longest word
  int cur_len = 0;        // removed int long
  int pos;
  char c_word[max_word_len]; //current word scaned in, not the other word you pervert


  // loop through entire word list
  // For each word of length n, get its total number of occurence so we can malloc array after
  // Get the total frequency of letters occuring for each
  /////   // We just need lenght of words not to copy as entire string them has to be a faster way
  /////   // to get just the lenght should be able to just ++ char untill '\0' ??
  while (fscanf(MASTER_FILE, "%s", c_word) == 1) {

    // find the lenght of the word
    cur_len = strlen(c_word);
    TEMP.array_loc = (int *) malloc(max_word_len * sizeof(int)); // removed int long
    if (cur_len > true_max_len) {
      true_max_len = cur_len;
    }

    // Increment total amount of words for the found length
    // Must subtract to offset index
    PMS[cur_len - 1].word_count++;
    //printf("Len: %d Word: %s Len_Occurance:%d\n", cur_len, c_word, PMS[cur_len - 1].word_count);

  }

  /////////////////////////////
  /// END OF FIRST SCAN TO FIND
  /// 1. True Max Length
  /// 2. Find word totals for each length
  /// ///////////////////////////// ///////////////////////////// /////////////////////////////
  // Create arrays to fill w/ the file position of word w/ coresponding length. We use true max to save memory
  // in case max_word_len will liekly be larger
  for (i = 0; i < true_max_len; i++) {
    //printf("%d", PMS[i].word_count);
    PMS[i].array_loc = (int *) malloc(PMS[i].word_count * sizeof(int)); // removed int long
    //printf("\n");
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
      ////printf("**%s**\n", c_word);
      if (((cur_len = strlen(c_word)) == (i + 1))){
        c_word[0] = tolower(c_word[0]);
        //pos = ftell(MASTER_FILE);
        PMS[i].array_loc[k] = pos;
        //printf("Struct[%d].array_loc[%d] Word: %s Posn: %d \n", i, k, c_word, pos);
        k++;

        // Get frequency
        // to avoid massive swith statement for each case of a, b, c, d ect
        // convert letter to integer, subtract from asci value of a = 97
        // ex. b=98, 98-97 = 1, index 1 (correlates to b) is incremented
        // leave l_var as is, some reason you cant subtract 97 on same line...

        for (j = 0; j <= i; j++) {
          l_var = c_word[j];
          l_var = l_var - 97;
          ////printf("%c - a = %d \n", c_word[j], l_var);
          PMS[cur_len-1].letter_freq[l_var]++;
        }
        //printf("\n");
      }

      pos = ftell(MASTER_FILE);
      if (fscanf(MASTER_FILE, "%s", c_word) != 1) {
        flag = -1;
        ////printf("c_word: %s \n", c_word);
      }

    }
    ////printf("\n\n\n");
  }

  return ;
}


// based on the current (partially filled or intitially blank) word, guess a letter
// current_word: current word -- may contain blanks, terminated by a null char as usual
// is_new_word: indicates a new hidden word (current_word has blanks)
// returns the guessed letter
// Assume all letters in a word are in lower case

// **************** MAJOR ISSUE *******************
// guess_hangman_player w/in the eval.c prog calls in a loop, therefore
// FLAGS
// 1.
// 2. We made a correct guess or are out of guesses, reset temp
//

char guess_hangman_player(char* current_word, bool is_new_word){
  //Initialize variables
  int q=0, w=0;
  char guess = ' ';
  char last_guess = ' ';
  int counter = 0;
  int best_freq = 0;
  int guess_letter_index = 0;
  int best_guess = 0;                    //The variable that stores the index of the highest frequency letter
  int length = strlen(current_word) - 1; //The length of the word

  //printf("Len:%d Bool:%d WORD |*%s*| TOTAL WORD COUNT:%d\n", length+1, is_new_word, current_word, PMS[length].word_count);

  // we know if its a new words we need to reset temp and fill with stuct of that len
  if(is_new_word == true){
    //printf("NEW WORD\n");

    // COPY WORD COUNT
    TEMP.word_count=PMS[length].word_count;

    // COPY ALL WORD LOCATIONS (text positions) TO TEMP.array_loc
    for(q=0; q < TEMP.word_count; q++){
      TEMP.array_loc[q]=PMS[length].array_loc[q];
    }
    // // // DELETE PRIN STATEMENT
    // for(q=0; q < PMS[length].word_count; q++){
    //  //printf("PMS[%d] TEMP.array_loc[%d] %d\n", length, q, TEMP.array_loc[q]);
    //  }

    //"Flag" that no more locations exist beyond array index q, set word count
    TEMP.array_loc[q] = -1;

      // COPY OVER PER PMS[LENGTH]FRQUENCIES
      for(w=0;w<26;w++){
        TEMP.letter_freq[w]=PMS[length].letter_freq[w];
		LETTER_ARR[w]=PMS[length].letter_freq[w];
        // ALSO GRAB THE HIGHEST OCCURING FREQUENCY WORD for inital guess

    if(length > 0){
  		// Guesses highest freq vowel on first guess ************
  		if(((w == 0) || (w == 4) || (w == 8) || (w == 14) || (w == 20) || (w == 24)) &&(TEMP.letter_freq[w] > TEMP.letter_freq[best_guess])){
  			best_guess = w;
  			best_freq = TEMP. letter_freq[w];
  		}
    }else{

      if(TEMP.letter_freq[w] > TEMP.letter_freq[best_guess]){

        best_guess = w;

      }

    }

      }
      T_FREQ(PMS, length);
      //printf("best initial guess[%d] = %c\n", best_guess, 97+best_guess);
	  //Once best_guess is found, input -1 into the freq arrays so not guessing same letter again
	  LETTER_ARR[best_guess] = -1;
	  TEMP.letter_freq[best_guess] = -1;

  //if not a new word == FALSE, we can refine our next guess here, generate new freq
  }else{
      //printf("SAME WORD\n");
      // Call rewrite_letter_freq to get new best freq
      best_guess = rewrite_letter_freq(current_word);
    // *************************************************************
    //  // COPY OVER FRQUENCIES
    // for(int w=0;w<26;w++){
    //  TEMP.letter_freq[w]=PMS[length].letter_freq[w];
    //  // ALSO GRAB THE HIGHEST OCCURING FREQUENCY WORD for inital guess
    //  if(TEMP.letter_freq[w] > TEMP.letter_freq[best_guess]){
    //    ////printf("best_guess[%d] = %c\n", best_guess, 97+best_guess);
    //    best_freq = TEMP.letter_freq[w];
    //    best_guess = w;
    //  }
    //  // ZERO OUT LETTER_ARR
    //  LETTER_ARR[w]=0;
    // }
    // *************************************************************
  }
    //T_FREQ(PMS);
    //printf("GET BEST GUESS[%d]: %c, FREQ: %d \n", best_guess, 97+best_guess, TEMP.letter_freq[best_guess]);

    //This switch case sets the letter based on the cooresponding index. i.e. (1=a) (2=b) (3=c)
    switch(best_guess){

      case 0: guess = 'a';
          break;
      case 1: guess = 'b';
          break;
      case 2: guess = 'c';
          break;
      case 3: guess = 'd';
          break;
      case 4: guess = 'e';
          break;
      case 5: guess = 'f';
          break;
      case 6: guess = 'g';
          break;
      case 7: guess = 'h';
          break;
      case 8: guess = 'i';
          break;
      case 9: guess = 'j';
          break;
      case 10: guess = 'k';
          break;
      case 11: guess = 'l';
          break;
      case 12: guess = 'm';
          break;
      case 13: guess = 'n';
          break;
      case 14: guess = 'o';
          break;
      case 15: guess = 'p';
          break;
      case 16: guess = 'q';
          break;
      case 17: guess = 'r';
          break;
      case 18: guess = 's';
          break;
      case 19: guess = 't';
          break;
      case 20: guess = 'u';
          break;
      case 21: guess = 'v';
          break;
      case 22: guess = 'w';
          break;
      case 23: guess = 'x';
          break;
      case 24: guess = 'y';
          break;
      case 25: guess = 'z';
          break;
      default:
          //printf("ERROR\n");
          return(guess);
    }

    // HERE WE HAVE TO NOW FLAG THE LETTER WE GUESSED FROM OUR CURRENT FREQ
    // FLAG WITH VALUE OF -1
    TEMP.letter_freq[best_guess] = - 1;
    //LETTER_ARR[best_guess] = 1;
    return (guess);
}




// THIS FUNCTION IS USED TO RE-SORT THE ARRAY/LIST W/E INTO
void
feedback_hangman_player(bool is_correct_guess, char* current_word){
int bad_word = 0;
int good_word = 0;
int key_position = 0;
int counter = 0;
int key_value = 0;
int length = strlen(current_word) - 1;
char key_letter = ' ';
char copy[max_word_len];
int i=0, K=0;

// GET LETTER GUESSED IN GUESS_HANG_MAN_PLAYER WERE WE FLAGED
// TEMP.letter_freq[best_guess] = - 1;
////////////////
for(i=0; i<26; i++){
  if(TEMP.letter_freq[i] == -1){
    key_position = i;
    //not sure here
    key_letter = (char*) (i+97);
    key_value =i+97;
    //printf("FEEDBACK: index_position:%d letter:%c value%d\n",key_position, key_letter, key_value);
    break;
  }
}

////////////////////////IF WRONG GUESS  ///////////////////////////////////
// ALGORITHM: SORTING. RE-WRITES FILE LOCATIONS ONLY CONTAING WORDS W/ OUT THE
// BAD LETTER.
// HERE IS TO "DELETE" EVERY LOCATION CONTAINING A WORD WITH THE WRONG
// LETTER GUESS. WE USE THE INDEXED TEMP.array_loc[] "LIST" TO DO THIS, AND OVER WRITE
// THE INDEXED LOCATIONS CONTAINING AN INCORRECT WORD. ONCE THE SEARCH THOUGH ALL
// TEMP.word_count IS COMPLETE, WORD COUNT WHICH IS THE "LENGTH" OF OUR LIST IS UPDATED
// TO THE VALUE OF TIMES WORDS WERE FOUND NOT CONTAINING THE BAD LETTER.
  if(is_correct_guess == false){

    // FLAG THE VALUE AS NO LONGER CURRENT GUESS OR AVAILABLE GUESS
    TEMP.letter_freq[key_position] = -3;

    //printf("WRONG GUESS: REMOVING WORDS CONTAINING : %c\n", key_letter);

    // 1ST LOOP: USED TO COPY IN EVERY WORD IN TEMP.array_loc[]
    for(i=0;i<TEMP.word_count;i++){

      if(TEMP.array_loc[i] == -1){
        //printf("TEMP.array_loc[i] == -1 ////ERROR");
        break;
      }

      // COPY WORD AT FILE POSTION
      fseek(MASTER_FILE, TEMP.array_loc[i], 0);
      fscanf(MASTER_FILE, "%s", copy);

      // 2nd LOOP: CHECK ALL THE LETTERS FOR THE BAD LETTER
      for(int k=0; k<=length; k++){
        bad_word = 0;
          // WE CAN SPEED THIS UP TO BREAK WHEN IF IS MET BUT FLAG IS SAFER FOR NOW
          if(key_letter == copy[k]){
            bad_word = 1;
          }
      }
      // CHECK FLAG
      if(bad_word != 1){
          TEMP.array_loc[counter] = TEMP.array_loc[i];
          counter++;
      }
    }
    // FOR LOOP SEARCH COMPLETE, SET NEW WORD COUNT TO A NEWLY "SORTED" LENGTH
    TEMP.array_loc[counter]=-1;

    TEMP.word_count = counter;

//////////////////////// IF CORRECT GUESS  ///////////////////////////////////
  }else{
    //printf("Correct GUESS : %c\n", key_letter);
    for(i=0;i<TEMP.word_count;i++){
      // RESET FLAGS
      bad_word =0;
      good_word = 0;
      if(TEMP.array_loc[i] == -1){
        //printf("TEMP.array_loc[i] == -1 ////ERROR");
        break;
      }

      fseek(MASTER_FILE, TEMP.array_loc[i], 0);
      fscanf(MASTER_FILE, "%s", copy);

      // ENSURE ALL POSITIONS OF LETTER ARE CHECKED

      for(int k=0; k<=length; k++){

        if((current_word[k] == key_letter) && (copy[k] != key_letter)){
            bad_word = 1;

        }else if(copy[k] == key_letter){
            good_word = 1;

        }
      }
      // CHECK FLAG
      if(good_word == 1){
        if(bad_word == 0){
         TEMP.array_loc[counter] = TEMP.array_loc[i];
          counter++;
        }
      }
    }
      TEMP.array_loc[counter] = -1;
      // FLAG THE VALUE AS NO LONGER CURRENT GUESS OR AVAILABLE GUESS

      TEMP.letter_freq[key_position] = -2;
    //printf("key posn%d should be -2: %d\n", key_position,  TEMP.letter_freq[key_position]);
    }
  return;
}

/*
/ FIND THE LOCATIONS OF THE CURRENT WORD WHERE THE CORRECT LETTER RESIDES

      for(int j=0; j<26; j++){
        if(current_word[i]==LETTER_ARR[j]){

      if(-1 == LETTER_ARR[key_position]){
        // this does nothing the letter was already flagged as used

      }if(1 == LETTER_ARR[key_position]){
        // letter in that position was already guessed
        LETTER_ARR[key_position] = -1;

      // We found the letter that was guessed correctly
      // AND the position is empty (this should never be the case but yea)
      }else if((current_word[i] == key_letter) && (0 == LETTER_ARR[key_position])){
        // We set equal to 1
        LETTER_ARR[key_position] = 1;
        //// We can also generate new frequencies based off this ///
        //**********************************************************
        //
        //
        //
      }
    }
  // END of IF statement for blank values
  }
}
*/


int
rewrite_letter_freq(char *current_word){

  //go to primary array with word len=current_word len
  int length=strlen(current_word) - 1;
  int counter = 0;
  int i=0, k=0, flag=0;
  int letters_found;
  char copy[max_word_len];
  char key_letter = ' ';
  char key_value = 0;
  char key_position = ' ';
  int word_length = 0;

  // WE ALREADYT RESORTED ALL OF OUR VALUES WE JUST NEED TO GENERATE NEW FREQ AND BEST GUESS
  // WE KNOW THAT VALUES of LETTER ARRAY[i] = -2 indicate already guessed values

  // RESET ALL FREQ VALUES FOR EVERY LETTER OTHER THAN THOSE ALREADY GUESSED
  for(i=0; i<26; i++){
    if((TEMP.letter_freq[i] != -3) && (TEMP.letter_freq[i] != -2)){
      TEMP.letter_freq[i] = 0;
      letters_found++;
    }else{
      //printf("TEMP.letter_freq[%d]" , i, TEMP.letter_freq[i]);
    }
  }

//printf("****************************************************\n");
  T_FREQ(PMS,length);

  for(int t = 0; t < TEMP.word_count; t++){

    if(TEMP.array_loc[t] != -1){

      fseek(MASTER_FILE, TEMP.array_loc[t], 0);
      fscanf(MASTER_FILE, "%s", copy);

      for(int q = 0; q<=length; q++){

        if((current_word[q] - 97 >= 0) && (copy[q] != current_word[q])){

          TEMP.array_loc[t] = -1;

        }

      }

    }

  }

  // CALUCLATE NEW FREQ

  for(int p = 0; p<TEMP.word_count; p++){

    if(TEMP.array_loc[p] != -1){

      fseek(MASTER_FILE, TEMP.array_loc[p], 0);
      fscanf(MASTER_FILE, "%s", copy);
      //printf("-++++++-Word:%s Word_Count%d lenght%d\n",copy, TEMP.word_count, length+1);
      for(int q = 0; q<=length; q++){

        key_letter=copy[q];
        key_value = key_letter - 0;
        key_position = key_value - 97;
        if((TEMP.letter_freq[key_position] < 0)){
          //printf("do nothing\n");

        }else{
            //printf("%d--------Word:%s %c\n",q, copy, copy[q]);
  		switch(copy[q]){
          case 'a': TEMP.letter_freq[0]++;
                break;
          case 'b': TEMP.letter_freq[1]++;
                break;
          case 'c': TEMP.letter_freq[2]++;
                break;
          case 'd': TEMP.letter_freq[3]++;
                break;
          case 'e': TEMP.letter_freq[4]++;
                break;
          case 'f': TEMP.letter_freq[5]++;
                break;
          case 'g': TEMP.letter_freq[6]++;
                break;
          case 'h': TEMP.letter_freq[7]++;
                break;
          case 'i': TEMP.letter_freq[8]++;
                break;
          case 'j': TEMP.letter_freq[9]++;
                break;
          case 'k': TEMP.letter_freq[10]++;
                break;
          case 'l': TEMP.letter_freq[11]++;
                break;
          case 'm': TEMP.letter_freq[12]++;
                break;
          case 'n': TEMP.letter_freq[13]++;
                break;
          case 'o': TEMP.letter_freq[14]++;
                break;
          case 'p': TEMP.letter_freq[15]++;
                break;
          case 'q': TEMP.letter_freq[16]++;
                break;
          case 'r': TEMP.letter_freq[17]++;
                break;
          case 's': TEMP.letter_freq[18]++;
                break;
          case 't': TEMP.letter_freq[19]++;
                break;
          case 'u': TEMP.letter_freq[20]++;
                break;
          case 'v': TEMP.letter_freq[21]++;
                break;
          case 'w': TEMP.letter_freq[22]++;
                break;
          case 'x': TEMP.letter_freq[23]++;
                break;
          case 'y': TEMP.letter_freq[24]++;
                break;
          case 'z': TEMP.letter_freq[25]++;
                break;
  		}
  	  }
  	}
  }
  }
  int best_guess = 0;
  int best_freq = 0;

  int vowel_counter = 0;
  for(int q=0; q<sizeof(current_word); q++){
	  if(current_word[q] == 'a' || current_word[q] == 'e' || current_word[q] == 'i' || current_word[q] == 'o' || current_word[q] == 'u'){
		  vowel_counter++;
	  }
  }
  
  for(i=0; i<26; i++){
    if((TEMP.letter_freq[i] != -3) && (TEMP.letter_freq[i] != -2)){
      if(TEMP.letter_freq[i] > TEMP.letter_freq[best_guess]){
          best_guess = i;
          best_freq = TEMP.letter_freq[i];
          ////printf("best_guess: %d best_freq: %d current_freq: %d", best_guess, best_freq,TEMP.letter_freq[i]);
      }
	  else if(TEMP.letter_freq[i] == TEMP.letter_freq[best_guess]){
		if(length <= 2){
			if(vowel_counter <= 1){
				if((i == 0) || (i == 4) || (i == 8) || (i == 14) || (i == 20) || (i == 24)){
					best_guess = i;
				}
			}
			else{
				best_guess = best_guess;
			}
		}
		else if(length > 2 && length < 6){
			if(vowel_counter <= 2){
				if((i == 0) || (i == 4) || (i == 8) || (i == 14) || (i == 20) || (i == 24)){
					best_guess = i;
				}
			}
			else{
				best_guess = best_guess;
			}
		}
		else if(length > 5 && length < 9){
			if(vowel_counter <= 3){
				if((i == 0) || (i == 4) || (i == 8) || (i == 14) || (i == 20) || (i == 24)){
					best_guess = i;
				}
			}
			else{
				best_guess = best_guess;
			}
		}
		else{
			if(vowel_counter <= 4){
				if((i == 0) || (i == 4) || (i == 8) || (i == 14) || (i == 20) || (i == 24)){
					best_guess = i;
				}
			}
			else{
				best_guess = best_guess;
			}
		}
	}
    }
      //printf("best_guess: %d best_freq: %d current_freq: %d\n", best_guess, best_freq,TEMP.letter_freq[i]);
  }
  

  T_FREQ(PMS, length);
  //printf("************************ BEST GUESS: %d\n",best_guess );
  return best_guess;
}

// first update the LETTER_ARR table where
//  0 means not found
//  1 means just found
// -1 means it was already found

////  Idea is later we can implement a better guessing algoritm
//    This would allow us to tally frequences to left and right
//    of every
//    |0|1|2|3|4|5|6|7| 8|9|10|11|12|....
//    |a|b|c|d|e|f|g|h| i|j|k |l |m |.....
//    |1|0|1|0|0|0|0|1|-1|0|-1|0 |0 |.....
//
//    actuall word:       Caleham
//    what our prog sees: ca__ha_
//
//    From that we can calculate frequencies to the left and right of
//    the areas that are non-zero
//
////



void T_PRINT(FILE *MASTER_FILE, PRIMARY *PMS){
  int i = 0,j;
  int pos;
  char temp[max_word_len];

  while (i < max_word_len){

    for(j=0;j<PMS[i].word_count;j++) {
      pos = PMS[i].array_loc[j];
      fseek(MASTER_FILE, pos, 0);
      fscanf(MASTER_FILE, "%s\n", temp);
      //printf("Struct[%d].array_loc[%d] total: %d Word: %s Posn %d \n",i, j, PMS[i].word_count, temp, pos);
    }

    i++;
  }
  return ;
}

void
T_FREQ(PRIMARY *PMS, int length){
  int i = length,j;
  int pos;
  char temp[max_word_len];

  //while (i < max_word_len){
    //printf("Total Words: %d\n",TEMP.word_count);
    for(j=0;j<26;j++) {
      //printf("Struct[%d].letter_freq[%d] letter: %c letter_freq %d \n",i, j, 'a'+j, TEMP.letter_freq[j]);
    }
    //printf("\n");
    i++;
  //}

}
