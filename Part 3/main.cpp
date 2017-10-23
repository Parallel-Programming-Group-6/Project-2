// Stephan Dimitrovski, Bony Patel, Jayan Ariyawansa, Michael Lacsamana
// Parallel & Distributed Programming
// Project 2 Part 3

// Contains a password generator, function to convert the password into a hash string, and a function
// to crack the hash string.
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <functional>
#include "passSerial.h"
#include "timing.h"
using namespace std;

static const int MAX_LENGTH = 8;
pthread_mutex_t print;

struct passVals{
	double temp;
	int startIndex;
	int endIndex;
};

void *thread_func(void *arg){
    passVals *args = (passVals *) arg;
	double temp = args->temp;
	int startIndex = args->startIndex;
	int endIndex = args->endIndex;
	string guess = crackPassword(temp, startIndex, endIndex);
	pthread_mutex_lock(&found_mutex);
	if(myhash(guess) == temp)
		found = 1;
	pthread_mutex_unlock(&found_mutex);
	string* threadGuess = new string;
	string* wrong = new string;
	*threadGuess = guess;
	if(myhash(*threadGuess) == temp)
        return threadGuess;
    else{
        *wrong = "not correct";
        return wrong;
    }
}


/* ASCII RANDOM PW GENERATOR */
/* Note that the string length will be random & between 1-8 */
/* Note that characters will also be either numerical or alphabetical */
string passwordGEN() {
	/* generate number between 1 and 8 to be used for length of password */
	long int passwordLength = rand() % 8 + 1;

	// Initialize variables
	int type = 0;
	int character = 0;
	string newPass;
	newPass.resize(passwordLength);

	// Ascii generator;
	for (int i = 0; i<passwordLength; i++)
	{
		type = rand() % (3 - 1 + 1) + 1;//Determines: 1 = Uppercase, 2 = Lowercase, 3 = Number
		if (type == 1)//If Uppercase
		{
			character = rand() % (90 - 65 + 1) + 65;// For uppercase letters

			newPass[i] = (char)character;
		}
		if (type == 2)//If Lowercase
		{
			character = rand() % (122 - 97 + 1) + 97;// For lowercase letters.
			newPass[i] = (char)character;
		}
		if (type == 3)// If numerical number
		{
			character = rand() % (57 - 48 + 1) + 48;// For numerical numbers
			newPass[i] = (char)character;
		}
	}

	// This returns new randomly generated password(string) to be used
	return newPass;

}
// Constants for used for the hash function

int main()
{
	/* random seed */
	srand(time(NULL));

	string test;
	size_t test2;
	double temp;
	string endGuessMain;
	void *threadGuess = NULL;
	string actualThread;

	test = passwordGEN();
	test2 = myhash(test);
	temp = test2;

	pthread_t tid[8];

	double startTime = Timer::currentSeconds();
	endGuessMain = crackPassword(temp, 1, 9);
	double endTime = Timer::currentSeconds();
    double serialTime = (endTime - startTime);
    cout << "\npassword serial:\t" << fixed << setprecision(3) << serialTime * 1000 << " ms\tGuess: " << endGuessMain;

    for(int x = 2; x < 9; x++){
        vector<passVals> range(x);
        found = 0;
        startTime = Timer::currentSeconds();
    	for(int taskIndex = 0; taskIndex < x; taskIndex++){
    		int span = MAX_LENGTH/x;
    		int startIndex = taskIndex * span;
    		int endIndex;
	    	if(MAX_LENGTH < startIndex + span)
	    		endIndex = MAX_LENGTH;
	    	else
	    		endIndex = startIndex + span;
            passVals vals;
	    	vals.temp = temp;
	    	vals.startIndex = startIndex;
	    	vals.endIndex = endIndex;
	    	range[taskIndex] = vals;
	    	pthread_create(&tid[taskIndex], NULL, thread_func, &range[taskIndex]);
    	}
    	for(int z = 0; z < x; z++){
            pthread_join(tid[z], &threadGuess);
            if(myhash(*(string *)threadGuess) == temp)
                actualThread = *(string *)threadGuess;
        }
        endTime = Timer::currentSeconds();
        cout << "\npassword threaded:\t" << fixed << setprecision(3) << (endTime - startTime) * 1000 << " ms\t";
        cout << "(" << fixed << setprecision(2) << serialTime/(endTime - startTime) << "x speedup from " << x << " threads)   \tGuess: " << actualThread;
    }

    cout << "\n\nREAL: " << test << endl << endl;
    return 0;
}

