#include <iostream>
#include <string.h>
#include <boost/tokenizer.hpp>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
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
};

Connector::Connector()
{ 
    // the semicolon connector allows the next command to be execute always
    run = true; 
    type = 0; // semicolon connector set to 0 
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
void parseinator(vector<string> input, string& command, string& argument, int& position, Connector& connect);
void commandifier(string command, string argument, Connector& connect);
int main()
{
    
    string str;
    string command;
    string argument;
    Connector connect;
    char hostname[100];
    int position = 0;
    while (true)
    {
        cout << getlogin(); // returns string containing name of user logged in
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
        Tok tok(str, sep);
        for (Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
        {
	    instruction.push_back(*tok_iter);
	}
       // for(int i = 0; i < instruction.size(); i++)
        //    cout << instruction[i] << endl;
        if (instruction[0] != "#") // checks for user input except for comments (#)
        {    
            parseinator(instruction, command, argument, position, connect); 
            commandifier(command, argument, connect);
            command = "";
            argument = "";
            for (; position < instruction.size();)
            {
                parseinator(instruction, command, argument, position, connect);
                if (connect.runNext())
		{
                    commandifier(command, argument, connect);
                }
		command = "";
                argument = "";
            }
            position = 0;
        }
    }
    return 0;
}
void parseinator(vector<string> input, string& command, string& argument, int& position, Connector& connect)
{
   // if(input[position] == "#")
   // {
   //     connect.type = 1;
   //     connect.run = false;
   // }
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
    if (input[position] != "#")
    {
        command = input[position];
        position++;
    }
    for (; position < input.size(); position++)
    {
        if (input[position] == "#")
        {
            cout << "I AM IN HERE" << endl;
            position = input.size();
            connect.type = 1;
            connect.run = false;
            break;
        }
        if (input[position] == "&" || input[position] == "|" || input[position] == ";")
	{
            break;
	}
        if (input[position] == "-")
        {
            while (input[position] != ";" && input[position] != "|" && input[position] != "&" && position+1 < input.size())
            {
                argument += input[position];
                position++;
            }
            if (position+1 == input.size() || input[position] == ";" || input[position] == "|" || input[position] == "&");
            {
               // argument += input[position];
                position++;
                break;
            }
            break;
        }
        argument += input[position];
        if (position+1 != input.size() && command == "echo")
  	{
            argument += " ";
	}
    }
}
void commandifier(string command, string argument, Connector& connect)
{
    cout << "i be command >>" << command << "<< i be argument >>" << argument << "<<" << endl;
    const char * const_command = command.c_str(); 
    char char_command[command.size()];
    const char * const_argument = argument.c_str();
    char char_argument[argument.size()];
    strcpy (char_command, const_command);
    strcpy (char_argument, const_argument);    
    char * args[2]; // char pointer array that holds command, and NULL
    char * args1[3]; // char pointer array that holds command, argument, and NULL
    bool no_arg = true;
    bool failed = false;
    cout << argument.size();

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

        cout << "Child: executing ls" << endl;
 	if (no_arg) // no argument
	{                                                                                             
        	execvp( args[0], args); // execvp the char pointer array that holds the command only
        	if (execvp(args[0], args) == -1) // if it returns -1 it failed
                {                                                                                                     
                    perror("execvp failed");
                    connect.run = false;
                    failed = true;
                }
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
	}
    }
    else if (c_pid > 0) // parent process
    {
        if ((pid = wait(&status)) < 0) 
        {
            perror("wait");
            exit(1);
        }
        cout << "Parent: finished" << endl;   
    }
    if (!failed)
    {
        connect.run = true;
    }
}
