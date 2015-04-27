#include "Enigma.h"
using namespace std;

int main()
{
	string input;
	cout<<"Welcome to the Enigma Cipher Machine. Press D to use default rotors, or R to use random rotors."<<endl;
	getline(cin,input);
	Enigma machine = Enigma();
	machine.setRings("AAA");
	machine.defaultRotors(input == "D");
	cout<<"    ABCDEFGHIJKLMNOPQRSTUVWXYZ"<<endl;
	machine.printRotorSettings();
	machine.printPlugBoard();
	cout<<"Enter message to be encrypted:"<<endl;
	getline(cin,input);
	string encrypted = machine.Encrypt(input);
	cout<<encrypted<<endl;
	string decrypted = machine.Encrypt(encrypted);
	cout<<decrypted<<endl;
	return 0;
}
