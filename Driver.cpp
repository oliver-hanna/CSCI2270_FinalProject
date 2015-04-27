#include "Enigma.h"
using namespace std;

int main()
{
	string input = "fdsf";
	while(input != "q")
	{
		Enigma machine = Enigma();
		string plugz;
		cout<<"Welcome to the Enigma Cipher Machine. All inputs must be uppercase."<<endl;
		cout<<"Please enter a 3 letter string (ex: AAA) to set the rings."<<endl;
		getline(cin,plugz);
		while(plugz.length() != 3)
			getline(cin,plugz);
		cout<<"Press 1 for full default machine settings."<<endl;
		cout<<"Press 2 for full random machine settings."<<endl;
		getline(cin,input);
		if(input == "1")
		{
			machine.setRings(plugz);
			machine.defaultPlugs(true);
			machine.defaultRotors(true);
		}
		else if(input == "2")
		{
			machine.setRings(plugz);
			machine.defaultPlugs(false);
			machine.defaultRotors(false);
		}
		while(input != "n" && input != "q")
		{
			cout<<"Enter E to Encrypt a message."<<endl;
			cout<<"Enter V to view the Rotor settings."<<endl;
			cout<<"Enter P to view the Plugboard settings."<<endl;
			cout<<"Enter n to make a new machine."<<endl;
			cout<<"Enter q to quit."<<endl;
			getline(cin,input);
			if(input == "E")
			{
				cout<<"Enter message: "<<endl;
				string msg;
				getline(cin,msg);
				string e = machine.Encrypt(msg);
				cout<<"Encrypted message: "<<e<<endl;
				cout<<"Press D to attempt to Decrypt your message. c to continue."<<endl;
				cout<<"Press P to view the encrtpytion path."<<endl;
				getline(cin,input);
				if(input == "P")
					machine.printEncrypt();
				cout<<"Decrypt now? PRESS D"<<endl;
				getline(cin,input);
				if(input == "D")
				{
					string d = machine.Decrypt(e);
					if(d == msg)
						cout<<"Success!"<<endl;
					else
						cout<<"Decryption Failed."<<endl;
					cout<<"Decrypted message: "<<d<<endl;
					cout<<"Print decryption path? PRESS P"<<endl;
					getline(cin,input);
					if(input == "P")
						machine.printDecrypt();
				}
			}
			else if(input == "V")
				machine.printRotorSettings();
			else if(input == "P")
				machine.printPlugBoard();
			else if(input == "q")
				cout<<"Bye"<<endl;
		}
	}
	return 0;
}
