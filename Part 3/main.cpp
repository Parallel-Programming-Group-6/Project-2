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
static const int NUM_THREADS = 8;

struct passVals{
	double hashNum;
	int startIndex;
	int endIndex;
};

void *pass_cracker(void *arg){

	// Get arguments for specific thread
    passVals *args = (passVals *) arg;
	double hashNum = args->hashNum;
	int startIndex = args->startIndex;
	int endIndex = args->endIndex;

	// Try to find the password given the range of password lengths
	string guess = crackPassword(hashNum, startIndex, endIndex);

	// If one thread finds the password, let the other threads know through a global variable
	pthread_mutex_lock(&found_mutex);
	if(myhash(guess) == hashNum)
		found = 1;
	pthread_mutex_unlock(&found_mutex);

	// store the password to pass to main function
	string* threadGuess = new string;
	string* wrong = new string;
	*threadGuess = guess;

	// If the guess is wrong (threads that are stopped), send a message to the main thread
	if(myhash(*threadGuess) == hashNum)
        return threadGuess;
    else{
        *wrong = "not correct";
        return wrong;
    }
}


// ASCII RANDOM PW GENERATOR
// Note that the string length will be random & between 1-8
// Note that characters will also be either numerical or alphabetical
string passwordGEN() {
	// generate number between 1 and 8 to be used for length of password
	long int passwordLength = rand() % 8 + 1;

	// Initialize variables
	int type = 0;
	int character = 0;
	string newPass;
	newPass.resize(passwordLength);

	// Ascii generator;
	for (int i = 0; i < passwordLength; i++){
		type = rand() % (3 - 1 + 1) + 1; // Determines: 1 = Uppercase, 2 = Lowercase, 3 = Number
		
		//If Uppercase
		if (type == 1){
			character = rand() % (90 - 65 + 1) + 65;// For uppercase letters
			newPass[i] = (char)character;
		}

		//If Lowercase
		if (type == 2){
			character = rand() % (122 - 97 + 1) + 97;// For lowercase letters.
			newPass[i] = (char)character;
		}

		// If numerical number
		if (type == 3){
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
	// Random seed
	srand(time(NULL));

	// Declare variables
	string password;
	size_t temp;
	double hashNum;
	string endGuessMain;
	void *threadGuess = NULL;
	string actualThread;
	found = 0;

	// Generate the password and the hash
	password = passwordGEN();
	temp = myhash(password);
	hashNum = temp;

	// Create an array of the max number of pthreads
	pthread_t tid[NUM_THREADS];

	// Time the serial implementation
	double startTime = Timer::currentSeconds();
	endGuessMain = crackPassword(hashNum, 1, 8);
	double endTime = Timer::currentSeconds();
    double serialTime = (endTime - startTime);
    cout << "\npassword serial:\t" << fixed << setprecision(3) << serialTime * 1000 << " ms\tGuess: " << endGuessMain;

    // Vector to store the values for each thread
    vector<passVals> range(NUM_THREADS);

    // Create 2 to 9 pthreads and time them
    for(int x = 2; x < 9; x++){
    	found = 0;
        startTime = Timer::currentSeconds();
    	for(int taskIndex = 0; taskIndex < x; taskIndex++){

    		// Calculate the range of lengths each thread should compute
    		int span = MAX_LENGTH/x;
    		int startIndex = taskIndex * span;
    		int endIndex;
	    	if(MAX_LENGTH < startIndex + span)
	    		endIndex = MAX_LENGTH;
	    	else
	    		endIndex = startIndex + span;

	    	// create a vector of structs to hold the values to be passed for each thread
            passVals vals;
	    	vals.hashNum = hashNum;
	    	vals.startIndex = startIndex;
	    	vals.endIndex = endIndex;
	    	range[taskIndex] = vals;

	    	// Spawn a thread with the correct vector index to pass values
	    	pthread_create(&tid[taskIndex], NULL, pass_cracker, &range[taskIndex]);
    	}

    	// For each pthread spawned, get the password it thought was correct
    	for(int z = 0; z < x; z++){
            pthread_join(tid[z], &threadGuess);
            if(myhash(*(string *)threadGuess) == hashNum) // store the right password
                actualThread = *(string *)threadGuess;
        }
        endTime = Timer::currentSeconds();
        cout << "\npassword threaded:\t" << fixed << setprecision(3) << (endTime - startTime) * 1000 << " ms\t";
        cout << "(" << fixed << setprecision(2) << serialTime/(endTime - startTime) << "x speedup from " << x << " threads)   \tGuess: " << actualThread;
    }

    // Print the actual password
    cout << "\n\nREAL: " << password << endl << endl;
    return 0;
}