# Galactic War
Galactic War is a C program that simulates a war between planets in a galaxy. The program uses a personal implementation of a circular doubly linked list to store the planets in the galaxy, and each planet has its own doubly linked list of shields. The program allows the user to add new planets to the galaxy at a specified index, delete existing planets from the galaxy, and attack other planets with a specified number of units. The program also provides a command to print the current state of the galaxy, including the name, number of shields, and number of destroyed planets for each planet. The purpose of this project was to get familiar with advanced data structures and implement them with no memory leaks. <br>

# Usage
Compile the program with "make" and run the resulting executable (./main). The program will prompt you to enter commands to manipulate the galaxy. Firstly, enter the number of commands you are going to run. <br>

The following commands are supported:

- ADD <planet_name> <planet_index> <number_of_shields>: adds a planet with the specified name and number of shields at the specified index in the galaxy. <br>
- BLH <index>: deletes the planet at the specified index from the galaxy. <br>
- UPG <planet_index> <shield_index> <upgrade_value>: increases the value of the shield at the specified index for the planet at the specified index by the specified amount. <br>
- ATK <attacking_planet_index> <attacked_planet_index> <attack_units>: attacks the planet at the second specified index with the specified number of units from the planet at the first specified index. If the attack is successful (i.e. the target planet's shields are reduced to zero), the attacking planet will gain one destroyed planet. <br>
- PRT: prints the current state of the galaxy, including the name, number of shields, and number of destroyed planets for each planet. <br>

# Examples
Here are some examples of how to use the program:

> 9 <br>
> ADD Earth 0 3 <br>
The planet Earth has joined the galaxy. <br>
> ADD Mars 1 2 <br>
The planet Mars has joined the galaxy. <br>
> ATK 0 1 2 <br>
> PRT <br>
0: Earth (3 shields, 0 destroyed planets) <br>
1: Mars (0 shields, 1 destroyed planets) <br>
> ADD Earth 0 3 <br>
The planet Earth has joined the galaxy. <br>
> ADD Mars 1 2 <br>
The planet Mars has joined the galaxy. <br>
> ADD Venus 2 3 <br>
The planet Venus has joined the galaxy. <br>
> BLH 1 <br>
The planet Mars has been eaten by the vortex. <br>
> PRT <br>
0: Earth (3 shields, 0 destroyed planets) <br>
1: Venus (3 shields, 0 destroyed planets) <br>
