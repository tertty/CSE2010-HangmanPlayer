/***********************************************************
 *
 *   DO NOT MODIFY  
 *
 ************************************************************/

/*

  Usage: EvalHangmanPlayer wordFile hiddenWordFile 

  Description:

  The goal is to evaluate HangmanPlayer.  For each hidden word in the
  hiddenWordFile, the program asks HangmanPlayer for guesses for
  letters in the hidden word.  As in the Hangman game, the maximum
  number of incorrect guesses is 6 (body parts).  HangmanPlayer is
  provided with a list of known English words in wordFile for
  initialization.

  The performance of HangmanPlayer is measured by:

  a.  accuracy: average accuracy over the hidden words;
           the accuracy of each hidden words is 
	        [ 1 - (number of incorrect guesses / 6) ] * 100%.
  b.  speed: average time to guess a letter
  c.  space consumption: memory consumption
  d.  overall score--(accuracy * accuracy)/sqrt(time * memory)  


  --------Pseudocode for evaluating HangmanPlayer---------------

     while not end of hiddenWordFile
        read a hiddenWord
	currentWord = blank word with the same length as hiddenWord

	while not maxGuesses of 6 is not reached and currentWord is not hiddenWord
	   correctGuess = false
           guess = player_guess(currenWord) 
	   if guess is in hiddenWord
              correctGuess = true
	      update currentWord
	   player_feedback(correctGuess, currentWord)

     report performance
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include "evalHangmanPlayer.h"
#include "hangmanPlayer.h"

#define  MAX_LINELEN   1000

void  preprocessingInHangmanPlayer(char* wordFile);
void  guessingInHangmanPlayer(char *hiddenWordFile);
float getAccuTimeOfHangmanPlayer(char *word,
			     clock_t *totalCpuTimeUsed,
			     int *totalNumGuesses);
void  reportPerformance(float accuracySum, clock_t totalCpuTimeUsed,
			int totalNumWords, int totalNumGuesses);
long  getPeakMemory();



int main(int argc, char* argv[])
{
  if (argc != 3)
    {
      fprintf(stderr,
	      "Usage: evalHangmanPlayer wordFile hiddenWordFile\n");
      exit(-1);
    }
  preprocessingInHangmanPlayer(argv[1]);
  guessingInHangmanPlayer(argv[2]);
}


// call initHangmanPlayer() in hangmanPlayer
// report time and memory for preprocessing
// wordFile has the dictionary words

void preprocessingInHangmanPlayer(char* wordFile)
{
  printf("Preprocessing in hangmanPlayer...\n");

  clock_t startTime, endTime;
  startTime = clock();
  init_hangman_player(wordFile);
  endTime = clock();

  printf("cpu time in seconds (not part of score): %.4e\n",
	 ((double) (endTime - startTime)) / CLOCKS_PER_SEC);
  printf("memory in bytes (not part of score): %ld\n",
	 getPeakMemory());
}


// given a hidden word file, feed hangmanPlayer one word at a time 
// report accuracy, time, memory, and score
void guessingInHangmanPlayer(char *hiddenWordFile)
{
  printf("Guessing in HangmanPlayer...\n");
  FILE *hiddenFile = fopen(hiddenWordFile, "r");
  if (hiddenFile == NULL)
    {
      fprintf(stderr, "error opening %s\n", hiddenWordFile);
      perror("guessingInHangmanPlayer()");
      exit(-1);
    }

  char    line[MAX_LINELEN+2],  // a line from the message file
          word[MAX_LINELEN+1];  // a word 
  float   accuracySum = 0.0;    // sum of accuracy of all words
  int     totalNumWords = 0,    // total # of guesses in hiddenWordFile
          totalNumGuesses = 0;  // total # of guesses from hangmanPlayer
  clock_t totalCpuTimeUsed = 0; // total cpu time used by hangmanPlayer

  /// for each line in the file, check word length and get performance from hangmanPlayer   
  while (fgets(line, MAX_LINELEN, hiddenFile) != NULL)
    {
      sscanf(line, "%s", word);
      if (strlen(word) <= MAX_WORDLEN)
	{
	  accuracySum += getAccuTimeOfHangmanPlayer(word, &totalCpuTimeUsed, &totalNumGuesses);
	  totalNumWords++;
	}
      else
	{
	  fprintf(stderr, "hidden word is longer than MAX_WORDLEN=%d\n ", MAX_WORDLEN);
	  fprintf(stderr, "skipping: %s\n", word);
	}
    }
  fclose(hiddenFile);

  // we have valid words and guesses
  if ((totalNumWords > 0) && (totalNumGuesses > 0))
    {
      reportPerformance(accuracySum, totalCpuTimeUsed, totalNumWords, totalNumGuesses);
    }
  else
    printf("%s does not have words with letters\n", hiddenWordFile);
}


// report performance: accuracy, time, memory, score
// accuracySum: sum of the accuracy for all hidden words
// totalCpuTimeUsed: cpu time used for all guesses
// totalNumWords: total number of hidden words
// totalNumGuesses: total number of guesses
void reportPerformance(float accuracySum, clock_t totalCpuTimeUsed,
		       int totalNumWords, int totalNumGuesses)
{  
      // calculate average accuracy and cpu time
      // print accuracy, cpu time, memory and score
      float  accuracy = accuracySum * 100.0 / totalNumWords;
      double cpuTime = totalCpuTimeUsed / (double)totalNumGuesses / CLOCKS_PER_SEC;
      long   memory = getPeakMemory();

      if (cpuTime <= 0) // too small to measure, unlikely
	cpuTime = 1.0 / CLOCKS_PER_SEC;
      if (memory <= 0) // too small to measure, highly unlikely
	memory = 1;

      printf("Accuracy: %.4f\n", accuracy);
      printf("CPU time per guess in seconds: %.4e\n", cpuTime),
      printf("Memory in bytes: %ld\n", memory);
      printf("Score: %.4f\n", (accuracy * accuracy) / sqrt(cpuTime * memory));

}

//   given a hiddenWord
//   ask hangmanPlayer to make gussses
//   add to total cpu time used and total number of guesses
//   return accuracy (1 - % of incorrect guesses)
float getAccuTimeOfHangmanPlayer(char *hiddenWord, clock_t *totalCpuTimeUsed, int *totalNumGuesses)
{
  int     hiddenWordLen = strlen(hiddenWord), // length of hiddenWord
          numIncorrectGuesses = 0;            // number of incorrect guesses from hangmanPlayer
  bool    isCorrectWord = false,             // true if the entire word is guessed
          isCorrectLetter = false,           // treu if the guessed letter is correct
          isNewWord = true,                  // true if a new word for hangmanPlayer
          guessedLetters[26];                // true if a letter has been guessed
  float   accuracy = 0.0;                    // 1 - (numOfGusses/6)
  clock_t startTime, endTime;
  char    letter;                            // a guessed letter from hangmanPlayer
  char    guessWord[MAX_WORDLEN+1],          // guessed word by hangmanPlayer so far
          guessWordCopy[MAX_WORDLEN+1];      // a copy of guessWord

  int i = 0; 
  for (; i < hiddenWordLen; i++)
    {
      guessWord[i] = ' ';                    // initialize guessWord to be all blanks
      hiddenWord[i] = tolower(hiddenWord[i]); // ensure all lower case in hiddenWord 
    }
  guessWord[i] = '\0'; // terminate guessWord
  for (i = 0; i < 26; i++)
    guessedLetters[i] = false;

  while ((isCorrectWord == false) && (numIncorrectGuesses < 6)) // no more than 6 incorrect guesses
    {
      strcpy(guessWordCopy, guessWord); // to prevent hangman player from modifying guessWord
      startTime = clock();
      letter = tolower(guess_hangman_player(guessWordCopy, isNewWord));//ask hangmanPlayer to guess
      endTime = clock();
      (*totalCpuTimeUsed) += (endTime - startTime);
      (*totalNumGuesses)++;
      isNewWord = false;

      //printf("strchr: %s, %c\n", hiddenWord, letter);
      char* letterp = strchr(hiddenWord, letter);
      // if letter is in hiddenWord and not guessed before, add to guessWord
      if (isalpha(letter) && letterp != NULL && !guessedLetters[(int)letter - (int)'a'])  
	{
	  isCorrectLetter = true;
	  guessedLetters[(int)letter - (int)'a'] = true;
	  do 
	    {
	      guessWord[letterp - hiddenWord] = letter;
	      letterp = strchr(letterp + 1, letter); // the letter might appear more than once
	    }
	  while (letterp != NULL);
	  strcpy(guessWordCopy, guessWord);
	}
      else // feedback is incorrect letter, guessWord is not changed
	{
	  isCorrectLetter = false;
	  numIncorrectGuesses++;
	}

      startTime = clock();
      feedback_hangman_player(isCorrectLetter, guessWordCopy);
      endTime = clock();
      (*totalCpuTimeUsed) += (endTime - startTime);
      isCorrectWord = (strcmp(guessWord, hiddenWord) == 0) ? true : false; // the entire word is correct
    }
  accuracy = (1 - numIncorrectGuesses/6.0);
  //printf("  accuracy: %.5f\n", accuracy);
  return accuracy;
}




/**********************************************************************
Different versions of getPeakMemory() for different operating systems,
selected by #define at the top of this file.

LINUX
MAC_OS
WINDOWS

OTHERS: getPeakMemory() is not supported
***********************************************************************/

#ifdef LINUX
// return peak usage of virtual memmory in bytes
long getPeakMemory()
{
  FILE *procFile = fopen("/proc/self/status", "r");
  if (!procFile)
    {
       perror("/proc/self/status: ");
       return -1;
    }

  long peakVirtualMemory=0;    // VmPeak
  char line[128];
  bool done = false;

  while ((fgets(line, 128, procFile) != NULL) && (done == false))
    {
      // "Peak virtual memory size"
      if (strncmp(line, "VmPeak:", 7) == 0)  // check the first 7 chars
	{
	  sscanf(line+7, "%ld", &peakVirtualMemory);  // in kB
	  done = true;
	}
    }
  fclose(procFile);
  return peakVirtualMemory * 1024;
}
#endif


#ifdef MAC_OS
#include <sys/time.h>
#include <sys/resource.h>
// return max resident set size
long getPeakMemory()
{
  struct rusage usage;
  
  if(getrusage(RUSAGE_SELF, &usage) == 0)
    //"the maximum resident set size utilized (in kilobytes)"
    return (long)usage.ru_maxrss * 1024;
  else 
    return -1;
}
#endif


#ifdef WINDOWS
#include <windows.h>
#include <psapi.h>

// return total amount of memory committed in bytes
long getPeakMemory()
{
  PROCESS_MEMORY_COUNTERS info;

  if (GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info)) != 0)
    {
      //"The peak value in bytes of the Commit Charge during the lifetime of this process."
      return (long)info.PeakPagefileUsage;
    }
  else
    { 
      fprintf(stderr, "GetProcessMemoryInfo() error\n");
      return -1;
    }
}
#endif


#ifdef OTHERS

// return total amount of memory committed in bytes
long getPeakMemory()
{  
  fprintf(stderr, "getPeakMemory() not supported\n");
  return -1;

}
#endif
