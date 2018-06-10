Damien Sudol
HW4 README
Version 1.0


***************************************************************************************************

				Client-Server Programming

This program consists of both a client and server. For proper execution please follow the 
instructions outlined in the document carefully. 

Gameplay: Gameplay will be conducted on the client-side. User will initially be prompted to enter
their name which they prefer to be displayed on the leaderboard, provided they have an adequate
score. Names must be no more than 100 characters. White space and special characters are 
accepted but counted towards length. 

Next, a random hidden number will be established for gameplay. User will be prompted to enter their
guess. Player can expect the random number to be between 0000-9999 and should enter a guess 
accordindgly, not less than, or more than the range. Only integer values will be accepted
or user will be re-prompted. Player must enter in 4 digits, for example 12 -> 0012 or 
1 -> 0001. This will help the player establish more informed guesses.

Server will assess the player's submitted guess and return a "Result of guess".
The reuslt of guess will be the difference of the submitted integer, at its respective index,
in relation to the hidden random numbers integer value, at its respective index, added together.

Example:
Hidden Number: 	7890
Player's guess: 1389
Index 0: 	7, 0	difference = 7
Index 1:	8, 3	difference = 5
Index 2:	9, 8	difference = 1
Index 3 	0, 9	difference = 9
Sum of differences:		     21

For the given example a Result of guess will be 21.

After a successful guess the player will be notified and a leader board will be displayed.
If the player's score places high enough on the leader board, top 3, their name will be 
displayed. If player has an equal score to an existing leader board ranking the existing 
ranking will retain its place.

Client process will terminate after a successful guess.

***************************************************************************************************
			IMPORTANT NOTES FOR FILE STRUCTURE

ServerSide.cpp and ClientSide.cpp must be in unique directories during compilation and execution.
The submitted directory will have two folders; ClientSide and ServerSide. User is expected
to enter the respective directory for compilation and execution. To run the program, at minimum two
terminal windows must be open, one for the server and one for the client. To enter the ServerSide
directory from the CWD of README.txt:
Command:
	cd ServerSide

To enter the ClientSide directory from the CWD of README.txt:
Command:
	cd ClientSide

Each directory will have the necessary cpp files for compilation and execution. Please note
ServerSide.cpp must be compiled and running before ClientSide.cpp can be ran.

**************************************************************************************************

			Compiling ServerSide.cpp

Follow the provided command line instructions for compiling. Instructions are 
intended to be entered from within the CWD containing ServerSide.cpp.

IMPORTANT NOTE: ServerSide.cpp MUST be in a different CWD than ClientSide.cpp.
Failing to do so will result in unwanted behavior. Given the current file structure of
the program folder it is expected user will enter the CWD containing ServerSide.cpp
before compiling. 

Compilation Command:
	g++ -std=c++11 ServerSide.cpp -lpthread

			Running ServerSide.cpp

After successfully compiling ServerSide.cpp use the provided command line input to ensure proper
execution. Enter command from the same directory as ServerSide.cpp.

Run Command:

	./a.out [desired port number]

Notes on run command: process takes in one command line argument, a "port number".
Please reference open port numbers and use best judgement. If a port binding error
occurs try the run command with a new port number.

			Exiting ServerSide.cpp

ServerSide.cpp is intended to run indefinately. To exit enter the following command:

Command:
	CTRL C

*****************************************************************************************************

			Compiling ClientSide.cpp

Follow the provided command line instructions for compiling. Instructions are intended
to be entered from within the CWD containing ClientSide.cpp.

IMPORTANT NOTE: ClientSide.cpp MUST be in a differenct CWD than ServerSide.cpp. Program
allows for concurrent excecution of the ClientSide.cpp process. To run ClientSide.cpp concurrently
open more terminals and follow the "run" instructions as laid out below, after succesful 
compilation. 

Compilation Command:
	g++ -std=c++11 ClientSide.cpp

			Running ClientSide.cpp

After successfully compiling ClientSide.cpp use the provided command line input to ensure proper
execution. Enter command from the same directory as ClientSide.cpp. User may run ClientSide.cpp
in multiple terminals, simultaneously. ServerSide.cpp MUST be actively running before
user may run ClientSide.cpp. 

Run Command:

	./a.out [ip address] [port number]

ClientSide.cpp requires two command line arguments:
	ip address- Must be ip address of Server where ServerSide.cpp is running
	port number- Must be port number established by ServerSide.cpp

For example: Program ServerSide.cpp running on CS1 and port number 27888

	./a.out 10.124.72.20 27888

			Exiting ClientSide.cpp

Process will exit with proper guess. To exit prematurely enter command:
	CTRL C


********************************************************************************************************




















