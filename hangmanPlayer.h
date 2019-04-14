/*

    DO NOT MODIFY THIS FILE

    This is the interface of functions provided by the hangmanPlayer module
    for the other modules.

    Internal hangmanPlayer functions not intended for other modules are
    NOT declared here.

 */

#ifndef HANGMAN_PLAYER_H
#define HANGMAN_PLAYER_H

#include <stdbool.h>

#define MAX_WORDLEN 100  // maximum word length

void init_hangman_player(char* word_file);

char guess_hangman_player(char* current_word, bool is_new_word);

void feedback_hangman_player(bool is_correct_guess, char* current_word);

#endif
