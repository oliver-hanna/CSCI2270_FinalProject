#include "Enigma.h"
#include <string>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <vector>
using namespace std;

Enigma::Enigma()
{
	inputRoot = new node;
	root = new node;
	for(int i=0;i<26;i++)
	{
		letters[i] = i + 65;
		R1.cipher.push_back(char(i + 65));
		R2.cipher.push_back(char(i + 65));
		R3.cipher.push_back(char(i + 65));
		reflector.cipher.push_back(char(i + 65));
	}
	createReflector();
	count = 0;
}

void Enigma::printRotorSettings()
{
	cout<<"R1 :";
	for(int i=0;i<26;i++)
		cout<<R1.cipher[i];
	cout<<endl;	
	cout<<"R2 :";
	for(int i=0;i<26;i++)
		cout<<R2.cipher[i];
	cout<<endl;
	cout<<"R3 :";
	for(int i=0;i<26;i++)
		cout<<R3.cipher[i];
	cout<<endl;
}

void Enigma::printPlugBoard()
{
	for(int i=0;i<10;i++)
		cout<<plugBoard[i]<<" ";
	cout<<endl;
}

void Enigma::defaultRotors(bool input)
{
	defaultR = input;
	createRotors();
}

void Enigma::defaultPlugs(bool input)
{
	defaultP = input;
	createplugBoard();
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
		random_device rd;
		mt19937 g(rd());
		shuffle(R1.cipher.begin(),R1.cipher.end(),g);
		ringMod(0,R1);
		shuffle(R2.cipher.begin(),R2.cipher.end(),g);
		ringMod(1,R2);
		shuffle(R3.cipher.begin(),R3.cipher.end(),g);
		ringMod(2,R3);
	}
}

void Enigma::ringMod(int number, Rotor& r)
{
	int offset = ringSettings[number] - 65;
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
		cout<<place<<endl;
		temp = r.cipher[place];
		r.cipher[place] = temp1;
	}
}

void Enigma::createplugBoard()
{
	if(defaultP == false)
	{
		vector<char> temp;
		for(int i=0;i<26;i++)
		{
			temp.push_back(letters[i]);
		}
		random_device rd;
		mt19937 g(rd());
		shuffle(temp.begin(),temp.end(),g);
		for(int i=0;i<10;i++)
		{
			plugBoard[i] = "";
			plugBoard[i] += temp[i];
			plugBoard[i] += temp[19-i];
		}
	}
}

void Enigma::createReflector()
{
	string sreflector = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
	for(int i=0;i<26;i++)
		reflector.cipher[i] = sreflector[i];
}

void Enigma::segmentInput(string input)
{
	node *x = new node;
	root = x;
	x->val = input[0];
	x->nextchar = NULL;
	for(int i=1;i<input.length();i++)
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
	Rotor R1c,R2c,R3c;
	R1c = R1;
	R2c = R2;
	R3c = R3;
	while(x != NULL)
	{
		node *n = new node;
	//	node *y = new node;
		n = x;
		rotateRotor(R3c);
		count++;
		if(count%26 == 0)
			rotateRotor(R2c);
		if(count%26%26 == 0)
			rotateRotor(R1c);
		cinput = x->val;
		cinput = plug(cinput);
		cinput = R3c.cipher[cinput - 'A'];
		cinput = R2c.cipher[cinput - 'A'];
		cinput = R1c.cipher[cinput - 'A'];
		cinput = reflector.cipher[cinput - 'A'];
		cinput = inverseMatch(cinput,R1c);
		cinput = inverseMatch(cinput,R2c);
		cinput = inverseMatch(cinput,R3c);
		cinput = plug(cinput);
		x->val = cinput;
		x=x->nextchar;
	}
	x = root;
	for(int i=0;i<input.length();i++)
	{
		if(input[i] != ' ')
		{
			input[i] = x->val;
			x = x->nextchar;
		}
	}
	return input;
}

void Enigma::rotateRotor(Rotor &r)
{
	char temp = r.cipher[25];
	for(int i=25;i>0;i--)
	{
		r.cipher[i] = r.cipher[i-1];
	}
	r.cipher[0] = temp;
}

char Enigma::inverseMatch(char ch,Rotor r)
{
	int i=-1;
	bool found = false;
	while(!found)
	{
		i++;
		if(ch == r.cipher[i])
			found = true;
	}
	return letters[i];
}

char Enigma::plug(char ch)
{
	for(int i=0;i<10;i++)
	{
		string plug = plugBoard[i];
		if(ch == plug[0])
			return plug[1];
		else if(ch == plug[1])
			return plug[0];
	}
	return ch;
}
