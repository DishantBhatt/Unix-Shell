# Unix-Shell
Command Line Interpreter in C

  This is a simple shell done for an Operating Systems Class. It prompts the user for input and ends when an appropriate signal or EOF is encountered. This program uses various C capabilities as well as Operating Systems concepts i.e forking, backgrounding, STDIN/STDOUT etc. There are several files to help this program run including the header file (p2.h) and a line analyzer (getword.c).
  
  NOTE: This shell is limited compared to a real unix shell, there are still some capabilities that need to be implemented. 
  
  NOTE: This is a work in progress, some errors that still need to be fixed include:
  
  - cd without any arguments is behaving incorrectly.
  - ls-F command is behaving incorrectly with certain inputs
  - Complicated pipe commands with redirection and backgrounding sometimes are incorrect
  
  getword.c
  
  - line analyzer
  - takes in each line, stores command or argument into a char array, and sends appropriate signals if special characters are encountered (<,>,|,etc)
  
  p2.h
  
  - Header file for p2.c
  
  p2.c
  
  - main program
  - executes commands 
  - handles redirection and piping (most of the time)
  - has several functions to help it including a parser function and a special function in case piping is necessary. Uses "vertical piping" to handle pipes.
