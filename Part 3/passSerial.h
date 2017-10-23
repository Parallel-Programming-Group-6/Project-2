#ifndef PASS_SERIAL_H_
#define PASS_SERIAL_H_
#include <iostream>
#include <string>
using namespace std;

static const size_t InitialFNV = 2166136261U;
static const size_t FNVMultiple = 16777619;
pthread_mutex_t found_mutex;
int found;

size_t myhash(const string &s)
{
	size_t hash = InitialFNV;
	for (size_t i = 0; i < s.length(); i++)
	{
		hash = hash ^ (s[i]);       // xor  the low 8 bits
		hash = hash * FNVMultiple;  // multiply by the magic number
	}
	return hash;
}

// pass is the random password after being converted to a hash and as a type double
string crackPassword(double pass, int startIndex, int endIndex) {
	// The guess that is returned
	string correct;
	string guess;
	for (int i = startIndex; i < endIndex; i++) // iterate through the possible hash passwords
	{
		if(i == 0)
            continue;
		correct.resize(i);
		guess.resize(i);
		if(myhash(correct) == pass || found)
			break;
		else if(i == 1){
			for(int x = 48; x < 123; x++){
				guess[0] = char(x);
				if(myhash(guess) == pass || found){
					correct = guess;
					found = 1;
					break;
				}
			}
		}

		else if(i == 2){
			for(int x = 48; x < 123; x++){
				guess[0] = char(x);
				for(int y = 48; y < 123; y++){
					guess[1] = char(y);
					if(myhash(guess) == pass || found){

						correct = guess;
						break;
					}
				}
				if(myhash(correct) == pass || found)
					break;
			}
		}

		else if(i == 3){
			for(int x = 48; x < 123; x++){
				guess[0] = char(x);
				for(int y = 48; y < 123; y++){
					guess[1] = char(y);
					for(int z = 48; z < 123; z++){
						guess[2] = char(z);
						if(myhash(guess) == pass || found){
							correct = guess;
							break;
						}
					}
					if(myhash(correct) == pass || found)
						break;
				}
				if(myhash(correct) == pass || found)
					break;
			}
		}

		else if(i == 4){
			for(int a = 48; a < 123; a++){
				guess[0] = char(a);
				for(int b = 48; b < 123; b++){
					guess[1] = char(b);
					for(int c = 48; c < 123; c++){
						guess[2] = char(c);
						for(int d = 48; d < 123; d++){
							guess[3] = char(d);
							if(myhash(guess) == pass || found){
								correct = guess;
								break;
							}
						}
						if(myhash(correct) == pass || found)
							break;
					}
					if(myhash(correct) == pass || found)
						break;
				}
				if(myhash(correct) == pass || found)
					break;
			}
		}

		else if(i == 5){
			for(int a = 48; a < 123; a++){
				guess[0] = char(a);
				for(int b = 48; b < 123; b++){
					guess[1] = char(b);
					for(int c = 48; c < 123; c++){
						guess[2] = char(c);
						for(int d = 48; d < 123; d++){
							guess[3] = char(d);
							for(int e = 48; e < 123; e++){
								guess[4] = char(e);
								if(myhash(guess) == pass || found){
									correct = guess;
									break;
								}
							}
							if(myhash(correct) == pass || found)
								break;
						}
						if(myhash(correct) == pass || found)
							break;
					}
					if(myhash(correct) == pass || found)
						break;
				}
				if(myhash(correct) == pass || found)
					break;
			}
		}

		else if(i == 6){
			for(int a = 48; a < 123; a++){
				guess[0] = char(a);
				for(int b = 48; b < 123; b++){
					guess[1] = char(b);
					for(int c = 48; c < 123; c++){
						guess[2] = char(c);
						for(int d = 48; d < 123; d++){
							guess[3] = char(d);
							for(int e = 48; e < 123; e++){
								guess[4] = char(e);
								for(int f = 48; f < 123; f++){
									guess[5] = char(f);
									if(myhash(guess) == pass || found){
										correct = guess;
										break;
									}
								}
								if(myhash(correct) == pass || found)
									break;
							}
							if(myhash(correct) == pass || found)
								break;
						}
						if(myhash(correct) == pass || found)
							break;
					}
					if(myhash(correct) == pass || found)
						break;
				}
				if(myhash(correct) == pass || found)
					break;
			}
		}

		else if(i == 7){
			for(int a = 48; a < 123; a++){
				guess[0] = char(a);
				for(int b = 48; b < 123; b++){
					guess[1] = char(b);
					for(int c = 48; c < 123; c++){
						guess[2] = char(c);
						for(int d = 48; d < 123; d++){
							guess[3] = char(d);
							for(int e = 48; e < 123; e++){
								guess[4] = char(e);
								for(int f = 48; f < 123; f++){
									guess[5] = char(f);
									for(int g = 48; g < 123; g++){
										guess[6] = char(g);
										if(myhash(guess) == pass || found){
											correct = guess;
											break;
										}
									}
									if(myhash(correct) == pass || found)
										break;
								}
								if(myhash(correct) == pass || found)
									break;
							}
							if(myhash(correct) == pass || found)
								break;
						}
						if(myhash(correct) == pass || found)
							break;
					}
					if(myhash(correct) == pass || found)
						break;
				}
				if(myhash(correct) == pass || found)
					break;
			}
		}

		else if(i == 8){
			for(int a = 48; a < 123; a++){
				guess[0] = char(a);
				for(int b = 48; b < 123; b++){
					guess[1] = char(b);
					for(int c = 48; c < 123; c++){
						guess[2] = char(c);
						for(int d = 48; d < 123; d++){
							guess[3] = char(d);
							for(int e = 48; e < 123; e++){
								guess[4] = char(e);
								for(int f = 48; f < 123; f++){
									guess[5] = char(f);
									for(int g = 48; g < 123; g++){
										guess[6] = char(g);
										for(int h = 48; h < 123; h++){
											guess[7] = char(h);
											if(myhash(guess) == pass || found){
												correct = guess;
												break;
											}
										}
										if(myhash(correct) == pass || found)
											break;
									}
									if(myhash(correct) == pass || found)
										break;
								}
								if(myhash(correct) == pass || found)
									break;
							}
							if(myhash(correct) == pass || found)
								break;
						}
						if(myhash(correct) == pass || found)
							break;
					}
					if(myhash(correct) == pass || found)
						break;
				}
				if(myhash(correct) == pass || found)
					break;
			}
		}
		if (myhash(correct) == pass || found)
			break;
	}
	return correct;
}

#endif
