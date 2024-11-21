#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 6262)

#include "utilities.h"
#include <iostream>
#include <cctype>
#include <iomanip>
#include <cstring>
#include <climits>

using namespace std;

int syntaxChecker(const char a[7], int nWords, const char words[][7]) {
    bool arrayChecker = false;
    if (strlen(a) == 0) {
        return 1;
    }
    for (int i = 0; i < strlen(a); i++) { // stars get priority 
        if (!islower(a[i]) || strlen(a) < 4 || strlen(a) > 6 || !isalpha(a[i])) {
            return 1;

        }
    }
    for (int p = 0; p < nWords; p++) {
        if (strcmp(a, words[p]) == 0) {//if str is in words then it is a valid word
            arrayChecker = true;
        }
    }
    if (!arrayChecker) {
        return 2;
    }
    return 0;
}

int runOneRound(const char words[][7], int nWords, int wordnum) {
    if (nWords < 1 || wordnum < 0 || wordnum>nWords) {
        return -1;
    }
    int tryCounter = 1;//start at 1 so that we can return without iterating

    //cout << words[wordnum]<<endl;//for testing purposes

    char str[7];//input string
    bool loop = true;
    while (loop) {
        cout << "Trial word: "; 
        char temp[101]; // temporary buffer to read input 
        cin.getline(temp, 101); 
        int check = syntaxChecker(temp, nWords, words);
        if (check == 1) {
            cout << "Your trial word must be a word of 4 to 6 lower case letters." << endl;
            continue;
        }
        if (check == 2) {
            cout << "I don't know that word." << endl;
            continue; //skip the rest and do loop again
        } 

        strcpy(str, temp);//copy from buffer only if it has passed check 1
        
        int planets = 0; // reset each loop 
        int stars = 0; // reset each loop 
        int starsList[6] = { -1, -1, -1, -1, -1, -1 }; 
        bool usedCharsInStr[7] = { false }; // Track used characters in str 
        bool usedCharsInWords[7] = { false }; // Track used characters in words[wordnum] 
 
        if (strcmp(str, words[wordnum]) == 0) {
            return tryCounter; //skip the rest and do loop again
        } 
        for (int i = 0; i < strlen(str); i++) {
            if (str[i] == words[wordnum][i]) { 
                stars++; 
                starsList[i] = i;
                usedCharsInStr[i] = true;
                usedCharsInWords[i] = true;
            } 
        } 
        for (int i = 0; i < strlen(str); i++) { 
            if (!usedCharsInStr[i] && starsList[i] == -1) { //is stars is still negative there is not a star at that index
                for (int j = 0; j < strlen(words[wordnum]); j++) { 
                    if (str[i] == words[wordnum][j] && !usedCharsInWords[j]) {//checking if char has been used if not then add to planets and
                        planets++; 
                        usedCharsInStr[i] = true; 
                        usedCharsInWords[j] = true; 
                        break; 
                    } 
                } 
            }
        } 
        cout << "Stars: " << stars << ", Planets: " << planets << endl; tryCounter++; 
    } 
    return -1;//for testing purposes. Should never get here.
}

int main() {
    const int MAX_WORDS = 8000;
    char words[MAX_WORDS][MAXWORDLEN + 1];
    const char WORDFILENAME[] = "C:/Users/torin/Downloads/words.txt";
    //if no words loaded terminate and return 0;

    int nWords = getWords(words, 8000, WORDFILENAME);

    if (nWords < 1) {
        cout << "No words were loaded, so I can't play the game.";
        return 0;//exit immediately
    }

    int max = 0;
    int min = INT_MAX;// Need a very large number!!!

    //max and min element index the array can hold
    int maximum = nWords - 1;
    int minimum = 0;

    //used for the loop
    int counter = 0;
    char roundsString[10000];
    int rounds = 0;//adds rounds later
    //used for average calculation
    double totalTries = 0;
    double average;

    cout << "How many rounds do you want to play? ";
    cin >> roundsString;
    cin.ignore(1000, '\n');

    bool zero = false;
    if ((roundsString[0] == '0')) {
        if (strlen(roundsString) >= 2) {
            for (int i = 0; i < strlen(roundsString); i++) {
                if (isdigit(roundsString[i]) && roundsString[i] != '0')
                    break;
                if (!isdigit(roundsString[i])){
                    zero = true;
                    break;
                }
            }
        }
        if (strlen(roundsString) == 1) {
            zero = true;
        }
    }
    for (int i = 0; i < strlen(roundsString); i++) {
        if (roundsString[i] == '0') {
            if (i == strlen(roundsString) - 1) {
                cout << "The number of rounds must be positive";
                return 0;
            }
            continue;
            
        }
        else {
            break;
        }
    }

    if ((roundsString[0] == '-' && isdigit(roundsString[1])) || (zero)) {
        cout << "The number of rounds must be positive";
        return 0;
    }
    for (int i = 0; i < strlen(roundsString); i++) {
        if (isdigit(roundsString[i])) {
            rounds = rounds * 10 + (roundsString[i] - '0');
        }
        else {
            break;
        }
    }
    cout << endl;

    for (int i = 1; i <= rounds; i++) {
        int wordNum = randInt(minimum, maximum);
        cout << "Round " << i << endl;
        cout << "The secret word is " << strlen(words[wordNum]) << " letters long." << endl;

        counter = runOneRound(words, nWords - 1, wordNum);
        /*if (counter == -1) {
            cout << "Error";
            return 0;
        }*/
        //how many tries message
        if (counter > 1) {
            cout << "You got it in " << counter << " tries." << endl;
        }
        if (counter == 1) {
            cout << "You got it in 1 try." << endl;
        }

        //math and average calc plus replacing min and max
        if (counter < min) {
            min = counter;
        }
        if (counter > max) {
            max = counter;
        }
        totalTries += counter;
        average = totalTries / i;//where i is the round number
        cout << "Average: " << fixed << setprecision(2) << average << ", minimum: " << min << ", maximum: " << max << endl;//fix this
        if (i != rounds) {
            cout << endl;//adding a space in between lines expect for last one
        }
    }
    return 0;
}

