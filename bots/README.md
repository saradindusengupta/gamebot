# tictactoe_AI
A tictactoe game using AI
This game has been developed using C++ (GNU GCC version 4.9 and higher)
While some functions such as "to_string()" is only available in C++11, so compiling the code would yeild a error result.

Thus to get the compilation done correctly one need to consider the following things

*If you are using Ubuntu 14 or higher then g++ is already the latest one installed 
 Otherwise upgrade to latest one using the following commands in the shell

$ sudo apt-get update
$ sudo apt-get install g++

Now to compile the code in shell run the following commands as

$ g++ -std=c++11 ttt.c -o ttt

          or
          
$ g++ -std=c++0x ttt.c -o ttt

Now run the compiled programm using 
$ ./ttt

AND ENJOY

IF you are lucky and use CodeBlocks , then just head to the Settings -> Compiler and then select GNU GCC and in the bottom set the flag to acceept C++11 as C++11 or C++0x, whatever is available. 

That is it.
