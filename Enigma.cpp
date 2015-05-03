#include "Enigma.h"
#include <string>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <vector>
#include <ctype.h>
using namespace std;

//Base public contructor. This fills each rotator with the basic alphabet as well as a letters array with the alphabet. It will also create the reflector.

Enigma::Enigma()
{
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

//Simple print functions. Should be straightforward. Prints the elements of each rotator vector.
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

//Funcitions to determine whether or not we're using the default rotaters/plugboards or not. setRings will use the user's ringsettings.
//Inputs = user input deterimined in driver
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
//Function to create the rotors. If default rotors are not being used, the shuffle method will be called on each of the rotor vectors (which are currently just filled with the alphabet.)
//This means that each rotor will now consist of a scrambled alphabet to use as keys.
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
	else
	{
		string d1 = "EKMFLGDQVZNTOWYHXUSPAIBRCJ";
		string d2 = "AJDKSIRUXBLHWTMCQGZNPYFVOE";
		string d3 = "BDFHJLCPRTXVZNYEIWGAKMUSQO";
		R1.cipher.empty();
		R2.cipher.empty();
		R3.cipher.empty();
		copy(d1.begin(),d1.end(),back_inserter(R1.cipher));
		copy(d2.begin(),d2.end(),back_inserter(R2.cipher));
		copy(d3.begin(),d3.end(),back_inserter(R3.cipher));
		ringMod(0,R1);
		ringMod(1,R2);
		ringMod(2,R3);
	}
}

//Rings essentially are an extra layer of complexity to offset each rotor. After each rotor is created or scrambled, then they are offset based on the ringmod settings.
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
		temp = r.cipher[place];
		r.cipher[place] = temp1;
	}
}

//The random plugboard is created by shuffling a vector of the alphabet. The first 20 letters of that shuffled vector are mapped to eachother, so we get 10 pairings. Those 10 pairings are then put into an array
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
	else
	{
		string dp = "AVBSCGDLFUHZINKMOWRX";
		for(int i=0;i<10;i++)
		{
			plugBoard[i] = dp[i*2] + dp[i*2+1];
		}
	}
}

void Enigma::createReflector()
{
	string sreflector = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
	for(int i=0;i<26;i++)
		reflector.cipher[i] = sreflector[i];
}

//Here we're putting the message into a linked list to be passed through the encryption machine. This lets us easily move through each character.
//The output will be a link list created in the class instance.
void Enigma::segmentInput(string input)
{
	root = new node;
	node *x = new node;
	root = x;
	root->nextstep = NULL;
	if(islower(input[0])){//if the character is lower case
        input[0] = toupper(input[0]);//toupper returns the ASCII value of the uppercase letter
        x->lower = true;//used in the string comparison latter
	}
	x->val = input[0];
	x->nextchar = NULL;
	for(int i=1;i<input.length();i++)
	{
		if(input[i] != ' ')
		{
			node *y = new node;
			if(islower(input[i])){//if the character is lower case
                input[i] = toupper(input[i]);//toupper returns the ASCII value of the uppercase letter
                y->lower = true;//used in the string comparison latter
            }
			y->val = input[i];
			y->nextchar = NULL;
			x->nextchar = y;
			x = x->nextchar;
		}
	}
}

//First segment the input into a linked list
//Next, create copies of the original rotors so that we don't update the original rotor positions. This is essential if we want to decrypt
//Finally, we're going to encrypt each letter at a time by passing them through the different components of the machine.
//There is a linked list that contains all of the original letters starting at the root
//Each element of the linked list has another linked list pointing downards, to make a tree-like structure
//This second linked list is to keep track of the encryption process for each character
//This means that afterwards we can view how each character changed each time it was passed through a certain part of the machine.
//Our output is formed by replacing each character of the string with the corresponding character from the final linked list.
string Enigma::Encrypt(string input)
{
    lower_array = new bool[input.length()]; //array which will store the lowercase information
	segmentInput(input);
	node *x = new node;
	froot = new node;
	node *f = new node;
	froot = f;
	x = root;
	Rotor R1c,R2c,R3c;
	R1c = R1;
	R2c = R2;
	R3c = R3;
	while(x != NULL)
	{
	    lower_array[count] = x->lower;
		node *n = new node;
		node *y = new node;
		n = x;
		rotateRotor(R3c);
		count++;
		if(count%26 == 0)
			rotateRotor(R2c);
		if(count%26%26 == 0)
			rotateRotor(R1c);
		cinput = x->val;
		cinput = plug(cinput);

		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;

		cinput = R3c.cipher[cinput - 'A'];
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;
		cinput = R2c.cipher[cinput - 'A'];
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;
		cinput = R1c.cipher[cinput - 'A'];
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;
		cinput = reflector.cipher[cinput - 'A'];
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;
		cinput = inverseMatch(cinput,R1c);
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;
		cinput = inverseMatch(cinput,R2c);
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;
		cinput = inverseMatch(cinput,R3c);
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;
		cinput = plug(cinput);
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		n->nextstep = NULL;

		f->val = cinput;
		node *ft = new node;
		f->nextchar = ft;
		f = ft;
		x=x->nextchar;
	}
	f = NULL;
	x = froot;
	for(int i=0;i<input.length();i++)
	{
		if(input[i] != ' ')
		{
			input[i] = x->val;
			x = x->nextchar;
		}
	}
	count = 0;
	return input;
}

//This is the place where we can view how each letter was encrypted. How the 2D-linked list or tree-like structure is used is more obvious here
void Enigma::printEncrypt()
{
	node *f = new node;
	node *fs = new node;
	f = root;
	cout<<"ORIGINAL LETTER->PLUG->ROTOR3->ROTOR2->ROTOR1->REFLECTOR->INVERSE ROTOR1->INVERSE ROTOR2->INVERSE ROTOR 3->PLUG(FINAL OUTPUT)"<<endl;
	while(f != NULL)
	{
		fs = f;
		cout<<f->val<<"->";
		while(fs->nextstep != NULL)
		{
			fs = fs->nextstep;
			cout<<fs->val;
			if(fs->nextstep != NULL)
				cout<<"-->";
		}
		cout<<endl;
		f = f->nextchar;
	}
}

void Enigma::printDecrypt()
{
	printEncrypt();
}

string Enigma::Decrypt(string s)
{
	delete root;
	return DecryptH(s);
}

//Helper function for Decrypt. Same as Encrypt, only doesn't change the values of lower_array.
string Enigma::DecryptH(string input)
{
	segmentInput(input);
	node *x = new node;
	froot = new node;
	node *f = new node;
	froot = f;
	x = root;
	Rotor R1c,R2c,R3c;
	R1c = R1;
	R2c = R2;
	R3c = R3;
	while(x != NULL)
	{
		node *n = new node;
		node *y = new node;
		n = x;
		rotateRotor(R3c);
		count++;
		if(count%26 == 0)
			rotateRotor(R2c);
		if(count%26%26 == 0)
			rotateRotor(R1c);
		cinput = x->val;
		cinput = plug(cinput);

		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;

		cinput = R3c.cipher[cinput - 'A'];
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;
		cinput = R2c.cipher[cinput - 'A'];
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;
		cinput = R1c.cipher[cinput - 'A'];
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;
		cinput = reflector.cipher[cinput - 'A'];
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;
		cinput = inverseMatch(cinput,R1c);
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;
		cinput = inverseMatch(cinput,R2c);
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;
		cinput = inverseMatch(cinput,R3c);
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		y = new node;
		cinput = plug(cinput);
		y->val = cinput;
		n->nextstep = y;
		n = n->nextstep;
		n->nextstep = NULL;

		f->val = cinput;
		node *ft = new node;
		f->nextchar = ft;
		f = ft;
		x=x->nextchar;
	}
	f = NULL;
	x = froot;
	for(int i=0;i<input.length();i++)
	{
		if(input[i] != ' ')
		{
		    if(lower_array[i]){//if the character is lower case
                x->val = tolower(x->val);//toupper returns the ASCII value of the uppercase letter which is what a char really is
            }
			input[i] = x->val;
			x = x->nextchar;
		}
	}
	count = 0;
	return input;
}

//Rotating a rotor essentially offsets it by one.
void Enigma::rotateRotor(Rotor &r)
{
	char temp = r.cipher[25];
	for(int i=25;i>0;i--)
	{
		r.cipher[i] = r.cipher[i-1];
	}
	r.cipher[0] = temp;
}
//Getting an inverse of a rotor's character means that we have to match the position of the character in the rotor to the corresponding letter of the alphabet in that position
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
//This returns the modified character based on the plugs, and if that character isn't one of the 20 plugboard characters then it is returned as itself
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

void Enigma::about()
{
	cout<<"The Enigma Machine is a cipher machine that the Germans used in WWII."<<endl;
	cout<<"It consists of a few basic components: 3 rotors, 1 plugboard, and 1 reflector."<<endl;
	cout<<"When you put in a character to the machine, it will pass through the 3 rotors. Each rotor changes the character based on a specific cipher that each rotor has."<<endl;
	cout<<"The plugboard maps 10 characters to 10 other chaaracters, the character will be put through the plugboard at the beginning and ending of encryption/decryption."<<endl;
	cout<<"Each rotor has a ringsetting, that will offset it for further complexity."<<endl;
	cout<<"The reflector sends the character back through the machine after it has been through the first 3 rotors."<<endl;
	cout<<"Once through the reflector, the character will be changed again to a unique different character. It will then go through the inverse of the 3 rotors."<<endl;
	cout<<"Finally, you will have your encrypted character."<<endl;
}
