#include "Enigma.h"
#include <string>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
using namespace std;

Enigma::Enigma()
{
	inputRoot = new node;
	root = new node;
	for(int i=0;i<26;i++)
	{
		letters[i] = i + 65;
		R1.cipher[i] = i + 65;
		R2.cipher[i] = i + 65;
		R3.cipher[i] = i + 65;
		reflector.cipher[i] = i + 65;
	}
	createReflector();
	count = 0;
}

void Enigma::defaultRotors(bool input)
{
	defaultR = input;
}

void Enigma::setRings(string rings)
{
	for(int i=0;i<3;i++)
		ringSettings[i] = rings[i];
}

void Enigma::createRotors()
{
	if(defaultR == false)
	{
		srand(time(NULL));
		shuffle(R1.cipher[0],R1.cipher[25],rand());
		ringMod(1,R1);
		srand(time(NULL));
		shuffle(R2.cipher[0],R2.cipher[25],rand());
		ringMod(2,R2);
		srand(time(NULL));
		shuffle(R3.cipher[0],R3.cipher[25],rand());
		ringMod(3,R3);
	}
}

void Enigma::ringMod(int number, Rotor& r)
{
	int offset = ringSettings[number] - 'A';
	if(offset == 0)
		return;
	char temp = 'A';
	char temp1 = 'A';
	temp = r.cipher[0];
	int place = 0;
	for(int i=0;i<26;i++)
	{
		temp1 = temp;
		if(place+offset > 25)
			place = (place+offset)-26;
		else
			place += offset;
		temp = r.cipher[place];
		r.cipher[place] = temp1;
	}
}

void Enigma::createplugBoard()
{
	if(defaultR == false)
	{
		char temp[26];
		for(int i=0;i<26;i++)
		{
			temp[i] = letters[i];
		}
		srand(time(NULL));
		shuffle(temp[0],temp[25],rand());
		for(int i=0;i<10;i++)
		{
			plugBoard[i] = temp[i] + temp[19-i];
		}
	}
}

void Enigma::createReflector()
{
	string sreflector = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
	for(int i=0;i<25;i++)
		reflector.cipher[i] = sreflector[i];
}

void Enigma::segmentInput(string input)
{
	node *x = new node;
	root = x;
	for(int i=0;i<input.length();i++)
	{
		if(input[i] != ' ')
		{
			node *y = new node;
			y->val = input[i];
			y->nextchar = NULL;
			x->nextchar = y;
			x = x->nextchar;
		}
	}
}

string Enigma::Encrypt(string input)
{
	segmentInput(input);
	node *x = new node;
	x = root;
	while(x != NULL)
	{
		node *n = new node;
		node *y = new node;
		n = x;
		rotateRotor(R3);
		count++;
		if(count%26 == 0)
			rotateRotor(R2);
		if(count%26%26 == 0)
			rotateRotor(R1);
		cinput = x->val;

		cinput = plug(cinput);
		cinput = R3.cipher[cinput - 'A'];
		cinput = R2.cipher[cinput - 'A'];
		cinput = R1.cipher[cinput - 'A'];

		cinput = reflector.cipher[cinput - 'A'];

		cinput = inverseMatch(cinput,R1);
		cinput = inverseMatch(cinput,R2);
		cinput = inverseMatch(cinput,R3);
		cinput = plug(cinput);
		x=x->nextchar;
	}
	return input;
}

char Enigma::inverseMatch(char ch,Rotor r)
{
	int place = ch - 'A';
	place = 25 - place;
	ch = r.cipher[place];
	return ch;
}

char Enigma::plug(char ch)
{
	for(int i=0;i<10;i++)
	{
		string plug = plugBoard[i];
		if(ch == plug[1])
			return plug[2];
		else if(ch == plug[2])
			return plug[1];
	}
	return ch;
}
