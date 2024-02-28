##Author
Name: Arya Papala
Student ID: 101189141

Name: Nimish Bhatia
Student ID: 101272154


## Final Project 
This assignment has us writing a code following after the game phasmophobia where it has one ghost and four hunters trying to find the ghost before its too late, using the concepts we have already learned throughout the course.
 

#Purpose of Program

For the final project the goal is to write a program where it can simulate a ghost leaving evidence of different types for different ghosts and 4 hunters where the user can enter the names, where the hunters have to try and find what type of ghost it may be before they reach FEAR_MAX/BOREDOM_MAX they get to scared and reach their fear or get too bored before the ghost gets bored.

##List of files

- ghost.c: Contains all the functions for initalizing, adding, printing, threading etc.
- hunter.c: Contains all the functions for initalizing, adding, printing, threading etc.
- evidence.c: Contains all the functions for initalizing, adding, printing, threading etc.
- room.c: Contains all the functions for intializing, adding, printing, threading etc.
- house.c: Includes all rooms. Within the house it contains a room and a ghost.
- logger.c: Contains all logging functions for ghost and hunter.
- util.c: Contains some utility functions to use.
- defs.h: All function headers and structures.
- main.c: Includes main function where the code will execute.
- Makefile: Contains the compilation.
- README.txt: Containing the intstructions to run code

## Compiling and Executing

1. Navigate through the terminal to the folder which the file is being saved in
2. Use command 'make'
3. Use command './ghostHunter' to execute and run program
4. Using the command 'valgrind --leak-check=full ./ghostHunter' to check for summary of memory and leaks
5. To clean and remove all .o files use the command 'make clean'

## Usage Instructions

1. The program will ask the user for the four names of the four hunters

2. After the program will run creating the ghost of a random type and the ghost will move to different rooms in the house leaving different types of evidences based of the type of ghost

3. The program creates the four hunters with the names the user gives it. The hunters will go from room to room within the house gathering the evidences that the ghost leaves behind. 

4. Hunters can review evidence and come to a conlusion if the evidence they gathered is sufficent then the program closes and concludes that they have found out what type of ghost

5. The ghost can get bored if the hunter is not in the same room as the hunter and leave. Or if three hunters get to scared they will reach FEAR_MAX and the program ends.

