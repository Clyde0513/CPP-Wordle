#pragma warning(disable : 6262)
#define _CRT_SECURE_NO_WARNINGS
#include "utilities.h"
#include <iostream>
#include <cctype>
#include <iomanip>
#include <cstring>
#include <climits>
#include <cstdint>
using namespace std;

const char WORDFILENAME[] = "words.txt";
// Windows users:  Use / instead of \ for pathname separators.
int playOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum);
bool doesThisWordExists(const char wordList[][7], int wordNum, const char *target);

int main()
{
	char wordList[9000][7];
	int words = getWords(wordList, 10000, WORDFILENAME);
	if (words < 1)
	{
		cout << "No words were loaded, so I can't play the game.";
		return 0;
	}

	int numOfRounds = 0;
	int maximum = INT_MIN;
	int minimum = INT_MAX;
	double totalScores = 0;
	cout << "How many rounds do you want to play? ";
	cin >> numOfRounds;
	// cout << endl;
	cin.ignore(10000, '\n');
	if (numOfRounds <= 0)
	{
		cout << "The number of rounds must be positive.";
		return 0;
	}
	cout << endl;

	for (int i = 1; i <= numOfRounds; i++)
	{											// iterate through the rounds
		int hiddenWord = randInt(0, words - 1); // randomize a hidden word from the wordlist
		int wordLength = 0;						// initalizes a int variable that determines the length of the hidden word
		for (int j = 0; wordList[hiddenWord][j] != '\0'; j++)
		{ // loops through each of the hidden word to determine its length
			wordLength++;
		}
		//	cerr << wordList[hiddenWord] << endl; //this is a debugging purposes to check the hidden word at the start of the game
		cout << "Round " << i << endl;
		cout << "The hidden word is " << wordLength << " letters long." << endl;
		int score = playOneRound(wordList, words, hiddenWord); // plays one round of the game
		totalScores += score;								   // the amount of tries (not including invalid words) puts into the total scores variable int.
		if (score > maximum)
		{ // if the score (or amount of tries) is greater than the maximum, then maximum int becomes the score.
			maximum = score;
		}
		if (score < minimum)
		{
			minimum = score;
		}
		if (score == 1)
		{
			cout << "You got it in " << score << " try." << endl;
		}
		else if (score > 1)
		{
			cout << "You got it in " << score << " tries." << endl;
		}
		cout << fixed << setprecision(2) << "Average: " << totalScores / i << ", minimum: " << minimum << ", "
			 << "maximum: " << maximum << endl;
		cout << endl;
	}
}

int playOneRound(const char words[][MAXWORDLEN + 1], int nWords, int wordnum)
{
	if (nWords < 0 || wordnum < 0 || wordnum >= nWords)
	{
		return -1;
	}
	const char *secret_word = words[wordnum]; // stores the hidden word into a new char pointer variable secret word
	char guess_word[101];					  // declares a char named guess_word that holds 101 characters; the last char is a zero byte.
	int score = 0;

	do
	{ // in a do-while loop...
		cout << "Probe word: ";
		cin.getline(guess_word, 100); // enters a probe word up to 100 characters so that it can hold the zero byte.
		bool good = true;			  // I need this bool for the upcoming if statements
		if (strlen(guess_word) < 4 || strlen(guess_word) > 6)
		{
			cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
			good = false; // this returns to false so that we can check for it in the next if statement
						  // return 0;
		}
		for (int i = 0; guess_word[i] != '\0'; i++)
		{
			if (!islower(guess_word[i]) && good)
			{ // if the probe word is uppercased and if bool good is still true, then outputs the cout.
				cout << "Your probe word must be a word of 4 to 6 lower case letters." << endl;
				good = false; // if this if statement is true, then we make this to false too.
				// return 0;
				break;
			}
		}
		if (!doesThisWordExists(words, nWords, guess_word) && good)
		{ // similar tactic as the one above. If it can't find the probe word from wordlist and bool good is still true, then outputs the cout.
			cout << "I don't know that word." << endl;
			good = false; // if this if statement is true, then we make this to false too.
						  // return 0;
		}
		if (good)
		{ // if the above if statements does not execute, the bool good remains true, then increment score by one.
		  // another reason why I put this if statement for the entire logic for the "wordle" is because that it will only check the valid words and only increment based of that; it will not increment by one if the probe word is invalid.
			score++;

			int gold = 0;
			int silver = 0;
			char temp[100];
			char temp2[100];
			strcpy(temp, secret_word); // copies secret word into temp so that i can use it in my for loops more easily.
			strcpy(temp2, secret_word);

			if (strcmp(guess_word, secret_word) == 0)
			{
				return score;
			}

			for (int i = 0; guess_word[i] != '\0'; i++)
			{
				if (guess_word[i] == temp[i])
				{
					gold++;
					temp[i] = 'X';	// once it checks for the gold, the hidden word gold index was then replaced by 'X', so that it doesn't check later on (e.g duplicates)
					temp2[i] = 'X'; // stores the current index into another temp variable, so I can use it in the next for loop
				}
			}
			/*at this point, all of the golds has been checked off...
			  this nexted for loops will check the remaining silvers and if it finds a silver, it will replace that current index with 'X', so that it doesn't get checked
			  later on*/
			for (int i = 0; guess_word[i] != '\0'; i++)
			{
				if (temp2[i] != 'X')
				{
					for (int j = 0; temp[j] != '\0'; j++)
					{
						if (guess_word[i] == temp[j])
						{
							silver++;
							temp[j] = 'X';
							break;
						}
					}
				}
			}

			cout << "Golds: " << gold << ", Silvers: " << silver << endl;
		}
	} while (strcmp(guess_word, secret_word) != 0);
	return score;
}
/* checks if the word really does exists from the wordlist*/
bool doesThisWordExists(const char wordList[][7], int wordNum, const char *target)
{
	for (int i = 0; i < wordNum; i++)
	{
		if (strcmp(wordList[i], target) == 0)
		{
			return true;
		}
	}
	return false;
}