#ifndef ENIGMA_H
#include <iostream>
struct node
{
	char val;
	node *next;
	node *prev;
};
struct Rotor
{
	char cipher[26];
};
class Enigma
{
	public:
		Enigma();
		void setRings(std::string);
		std::string Encrypt(std::string);
		std::string Decrypt(std::string);
		void printCharEncrypt(char);
		void printRotorSettings();
	private:
		void createRotors();
		void createplugBoard();
		void createReflector();
		int checkStep();
		void segmentInput(std::string, char* arr);
		void rotateRotor();
		char inverseMatch(Rotor);
		std::string letters[26];
		std::string input;
		char cinput;
		std::string plugBoard[20];
		char ringSettings[3];
		Rotor R1, R2, R3, reflector;
		node *track;
};
#endif
