# CSCI-2270-Final-Project
Summary:
This program will replicate the WWII German Enigma Cipher Machine. I will use a linked list to keep track of the rotor replacements. I will include the default rotors that the Germans used along with an option to generate random rotors using some new C++ random libraries. The machine is fairly hard to explain, so if you wish to read more about it here: http://users.telenet.be/d.rijmenants/en/enigmatech.htm

BACKGROUND Update: A little more that should explain a bit of the code and how the machine works: Each rotor "steps". Rotor 3 will step every character input, rotor 2 steps with every 26 rotor 3 steps, and rotor 1 steps every 26 rotor 2 steps. Each step will offset the rotor's cipher by one. 
This means that the overall cipher is constantly changing - but to run an encrypted message back through the machine properly the original rotor settings have to be in tact (aka, it must be run through the rotors before any steps have been made). This makes sense in the context that different machines were being used to send/recieve messages.
You can essentially think of the ringsettings as initial steps (adding even more complexity to finding the correct settings to decrypt a message). Overall, the enigma has a potential 1.074586873273 x 10^23 settings. 
If you've seen the movie "The Imitation Game", you'll know that the job of the allied people working to crack the machine was to find a single working setting within a day's time frame. The Germans would reset the machine every day with different settings, making it extremely difficult to find any kind of solution. 
The Germans made a huge mistake though, they would send out a weather report in the same format at the beginning of every day. Using around 150 Bombes (a machine designed by Alan turing to simulate hundreds of diferent rotor settings simultaneously), the british were able to use the weather report's format to get the rotor settings each day. Figuring out this flaw played a large part in ending WWII.

How to Run:
**PLEASE MAKE SURE ALL OF YOUR INPUTS ARE CAPITAL LETTERS. SPACES ARE OKAY.**
Copy the repository to its own directory. I've already compiled the program, so run ./Driver in the linux terminal, or alternatively compile it yourself by running:
make
and then ./Driver afterwards.
The Driver runs the whole program and main!

Dependencies:
None

System Requirements:
Any OS that runs C++ programs

Group Members:
Oliver Hanna

Improvements:
For those who want to add to the program it currently does not support
any lowercase inputs. There are also almost no safeguards to check for punctuation or odd inputs (like numbers). These have the potential to break the program.
Adding safeguards to prevent the program from crashing and adding lowercase support are both possible TO-DO's. 

