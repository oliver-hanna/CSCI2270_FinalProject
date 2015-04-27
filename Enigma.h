#ifndef ENIGMA_H
#define ENIGMA_H
#include <iostream>
#include <vector>
struct node
{
	char val;
	node *nextchar;
	node *nextstep;
//	node *prev;
};
struct Rotor
{
	std::vector<char> cipher;
};
class Enigma
{
	public:
		Enigma();
		void setRings(std::string);
		void defaultRotors(bool);
		std::string Encrypt(std::string);
		std::string Decrypt(std::string);
		void printEncrypt();
		void printDecrypt();
		void printRotorSettings();
		void printPlugBoard();
		void defaultPlugs(bool);
		void about();
	private:
		void createRotors();
		void ringMod(int,Rotor&);
		void createplugBoard();
		void createReflector();
		void segmentInput(std::string);
		void rotateRotor(Rotor&);
		char inverseMatch(char,Rotor);
		char plug(char);
		bool defaultP;
		bool defaultR;
		char letters[26];
		int count;
		std::string input;
		char cinput;
		std::string plugBoard[10];
		char ringSettings[3];
		Rotor R1, R2, R3, reflector;
		node *root;
		node *froot;
};
#endif
