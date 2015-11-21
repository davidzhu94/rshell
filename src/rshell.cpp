#include <iostream>
#include <string.h>
#include <boost/tokenizer.hpp>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <algorithm>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

using namespace std;
using namespace boost;

class Connector
{
    public:
        Connector();
        ~Connector();
        bool run;
        int type;
        bool runNext();
        bool precedence;
	
};

Connector::Connector()
{ 
    // the semicolon connector allows the next command to be execute always
    run = true; 
    type = 0; // semicolon connector set to 0
    precedence = false;
}

Connector::~Connector()
{   
}
bool Connector::runNext()
{
    if (type == 1) // && connector 
    {
        if (!run) // if the first command doesn't succeed  it wont run the next command
        {
            return false;
        }
    }
    else if (type == 2) // || connector
    {
        if (run) // if the first command succeed it wont run the next command
        {
            return false;
        }
    }
    return true;
}
void parseinator(vector<string> input, string& command, string& argument, int& position, Connector& connect, string& flag);
void commandifier(string command, string argument, Connector& connect, string flag);
void test_function(string command, string flag, string argument, Connector& connect);
int main()
{
    string flag;
    string str;
    string command;
    string argument;
    Connector connect;
    char hostname[100];
    int position = 0;
    while (true)
    {
        printf("%s",getlogin()); // returns string containing name of user logged in
        gethostname(hostname, sizeof hostname); // returns the standard host name for the current machine
        cout << "@" << hostname << "$ ";
        getline(cin, str); 
        if (str == "exit") // special command of exit 
        {
            break;
        }
        vector<string> instruction; // vector of strings 
        typedef tokenizer<boost::char_separator<char> > Tok;
        char_separator<char> sep; // default constructed
        Tok tok(str, sep);;
        for (Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter) // parses the input into a command and argument
        {
	    instruction.push_back(*tok_iter);
		
	}
	if (instruction[0] != "#") // checks for user input except for comments (#)
        {    
            parseinator(instruction, command, argument, position, connect, flag); // parses the input into a command and argument  
            commandifier(command, argument, connect, flag); // runs a command with a given argument 
            command = "";
            argument = "";
            int vector_size = instruction.size();
            for (; position < vector_size;) // run until the end of the instruction
            {
                parseinator(instruction, command, argument, position, connect, flag);
                if (connect.runNext() && command != "") // checks connector to see if the next command should be ran
	        {
                    commandifier(command, argument, connect, flag);
                }
	        else
	        {
	            while(connect.precedence && position < vector_size)
		        {
		            if(instruction[position] == ")")
		            {
		                connect.precedence = false;
		            }
		            position++;
		        }
	            connect.run = true;
	            command = "";
	        }
	        command = "";
                argument = "";
            }
            position = 0;
        }
    }
    return 0;
}
void parseinator(vector<string> input, string& command, string& argument, int& position, Connector& connect, string& flag)
{
    if (input[position] == "&") // check if string is equal to & connector 
    {
        connect.type = 1; // set & connector to 1 
        position += 2; // +=2 in order to take && connect
    }
    if (input[position] == "|")
    {
        connect.type = 2; // set | connector to 2
        position += 2; // +=2 in order to take || connector
    }
    if (input[position] == ";")
    {
        connect.type = 0; // set ; connector to 0 
        position ++;
    }
    if (input[position] == "(")
    {
        connect.precedence = true;
        position++;
    }
    if (input[position] != "#")
    {
        command = input[position];
        position++;
    }
    if (command == "test")
    {
        if(input[position] == "-")
        {
            position++;
	    if (input[position] == "f")
            {
		flag = "-e";
		position++;
	    }
	    else if (input[position] == "d")
	    {
		flag = "-d";
		position++;	
	    }
	    else // DEFAULT E
	    {
		flag = "-e";
		position++;
            }
        }
    }
  
    int input_size = input.size();
    for (; position < input_size; position++)
    {
        if (input[position] == ")")
        {
            connect.precedence = false;
            position++;
            break;
        }
        if(command == "echo" && input[position] == "\"")
        {
            position++;
            while(input[position] != "\"" && position <= input_size)
            {

                argument += input[position];
                if(!isalnum(input[position].at(0)) && !isalnum(input[position+1].at(0)))
                {

                }
                else
		{
                    argument += " ";
                }
		position++;
            }
            position++;
            break;
        }
        if (input[position] == "#")
        {
            position = input.size();
            break;
        }
        if (input[position] == "&" || input[position] == "|" || input[position] == ";")
	    {
            break;
    	}
	
        argument += input[position];
        int input_size1 = input.size();
        if (position+1 != input_size1 && command == "echo")
	{				
	    //adds spaces when echoing
            argument += " ";
	}
    }
}
void commandifier(string command, string argument, Connector& connect, string flag)
{
    const char * const_command = command.c_str(); 
    char * char_command = new char[command.size()];
    const char * const_argument = argument.c_str();
    char * char_argument = new char[argument.size()];
    strcpy (char_command, const_command);
    strcpy (char_argument, const_argument);    
    char * args[2]; // char pointer array that holds command, and NULL
    char * args1[3]; // char pointer array that holds command, argument, and NULL
    bool no_arg = true;
    bool failed = false;
    
    if(command == "test" || command == "[")
    {	
	test_function(command,flag,argument,connect);
	return;	
    }
    // exit command     
    if(command == "exit" || argument == "exit")
    {
	exit(1);
    }

    if (argument.size() == 0) // no arguments 
    {
        args[0] = char_command; // args will contain the command
        args[1] = NULL;
    }
    else
    {
	no_arg = false; // sets bool no_arg to false 
	args1[0] = char_command; // args1 contains command and argument
        args1[1] = char_argument;
        args1[2] = NULL; 
    }
    pid_t c_pid, pid; // data type to reporesent process ID's
    int status;
    c_pid = fork(); // create our fork  

    if (c_pid < 0) // indicates the fork has fail if its less than 0 
    {
        perror("fork failed");
        exit(1);
    }
    else if (c_pid == 0) // in child process 
    {
 	if (no_arg) // no argument
	{ 		                                                                                            
        	execvp( args[0], args); // execvp the char pointer array that holds the command only
        	if (execvp(args[0], args) == -1) // if it returns -1 it failed
                {                                                                                                     
                    perror("execvp failed");
                    connect.run = false;
                    failed = true;
                }
		exit(3);
	}
	else // with command and argument
	{
		execvp(args1[0], args1); // execvp the char pointer array that holds the command, and argument
		if (execvp(args1[0], args1) == -1) // if it returns -1 it failed
                {
                    perror("execvp failed");
                    connect.run = false; 
                    failed = true;
                }
		exit(3);
	}
    }
    else if (c_pid > 0) // parent process
    {
        if((pid = wait(&status)) < 0 )
        {
            perror("wait");
	    exit(3);
        }
        if (WIFEXITED(status))  //Evaluates to a non-zero value if status was returned for a child process that terminated normally. 

        {
            if (WEXITSTATUS(status) != 0)
            {
                connect.run = false;
            }
	    else
	    {
		connect.run = true;	
            }
        }
       else 
        {
            connect.run = true;
        }
     }
}
void test_function(string command, string flag, string argument, Connector &connect)
{
   const char * arg = argument.c_str(); 
   struct stat sb; // struct for the stat function 
   if(flag == "-f")
   {
 	switch (sb.st_mode & S_IFMT){
	case S_IFREG:   connect.run = true;  // checks if the input is a regular file
	}
   }
   if(flag == "-d")
   {
	switch(sb.st_mode & S_IFMT){ 
	case S_IFDIR: connect.run = true; // check if the input is a directory  
	}	
	
   }
   if(stat(arg,&sb)==0) // checks if file/directory exists
   {
        connect.run = true;
   }
   else
   {
   	connect.run = false;    
   }	
}  



