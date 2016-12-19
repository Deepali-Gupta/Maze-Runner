# Maze-Runner
A game that allows a maximum of 6 players to play simultaneously over a network, by either starting a new game of their own or by joining a game started by some other player. 

## Description and Rules
- Involves movement through a randomly-generated maze filled with coins, hints and treasures. 
- FPS (First-Person Shooter) game that equips the player with bullets that can be used to harm the opponents. 
- Has exactly 6 players, which are either real players (who joined the game started by a player) or artificially-simulated players, controlled by artificial intelligence. 
- Each player has only 3 lives, and a health bar that diminishes when the player is hit by a bullet. The game ends when any player acquires the central treasure. 
- The results of the game are displayed to all the participants at the end.

## System Requirements
Maze Runner runs on Linux based platforms only. It requires the following packages :
- GNU gcc compiler
- OpenGL library
- OpenSSL toolkit and certificate
- pthread library

## Features
### Interface
OpenGL, a multi-platform application programming interface (API) for rendering 2D and 3D graphics, is used for displaying both the initial start/join game menus and the game view.

### Networking
The initial communication between the starting player and the joining player happens on a secure TCP connection, established using OpenSSL to avoid loss of interested players. Once the participants are recognized by the starting player, it passes on all the information about player-ids, IP addresses, artificial players (AI) and maze to all the other players, and closes its TCP listener port. The game is completely peer-to-peer once it starts. The exchange of player coordinates, coin picks, bullet fires and treasure finds are done via UDP connection, wherein each player has a port dedicated to listening from a specific player. 

### Data storage
Each player has information about the maze layout, as well the positioning of all the coins and treasures in it. The starting position of a player is decided through its player-id. The artificial players (AIs) decide on a path to the central treasure from their starting point and store that path in a file, and follow its coordinates.

### Multithreading
In order to enable independent and uninterrupted working of the various listener and responder threads (for exchanging player movements and actions), multithreading is implemented. It is also used during the initial TCP connection phase, to maintain the states of connection to all other players adn respond back with appropriate information aout other players and maze information, once the player starts the game.

### Synchronisation
The position of every player is constantly exchanged with other players to maintain the same correct game-view for all the players. Same is done whenever the player acquires a coin or treasure or fires a bullet.

### Makefile
Makefile is used to build the application from its source code, organize the code, and its compilation and linking.

## Usage Instructions
* In the terminal, change the current directory to "Assignment3".
* To run the main program:
  * Type "make server" in the terminal.
* To only compile the program:
  * Type "make" in the terminal.
* "make server" spawns a player.
  * The program can be executed from various network locations, to spawn new players. 
  * This opens the game menu. The user can then navigate through the user-friendly menu as directed.
* "make clean" cleans all the temporary files and executables.
* Once the application is running, follow the on screen instructions.
* The starting player can set a difficulty level for the game. 
* All the players can change their usernames before joining a game.
* At each transfer of data between server and client, the corresponding message with the data flow direction and the data being transferred is displayed.
