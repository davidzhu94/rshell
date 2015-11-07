#Variables 
Compile = g++
Flags = -Wall -Werror -ansi -pedantic 

all : rshell
	
rshell: 
	$(Compile) $(Flags)  


clean:
	rm -rf *~ bin
