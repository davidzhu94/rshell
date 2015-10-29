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
    run = true;
    type = 0;
}

Connector::~Connector()
{   
}
bool Connector::runNext()
{
    if (type == 1)
    {
        if(!run)
        {
            return false;
        }
    }
    else if(type == 2)
    {
        if(run)
        {
            return false;
        }
    }
    return true;
}

void parseinator(vector<string> input, string& command, string& argument, int& position, Connector& connect);
void commandifier(string  command, string  argument, Connector& connect);

int main()
{
    
    string str;
    string command;
    string argument;
    Connector connect;
    char hostname[100];
    int position = 0;
    while(true)
    {
        cout<<getlogin();
        gethostname(hostname,sizeof hostname);
        cout<<"@"<<hostname<<"$ ";
        getline(cin, str);
        if (str == "exit")
            break;
        vector<string> instruction;
        typedef tokenizer<boost::char_separator<char> > Tok;
        char_separator<char> sep; // default constructed
        Tok tok(str, sep);
            for(Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
            instruction.push_back(*tok_iter);
       // for(int i = 0; i < instruction.size(); i++)
        //    cout << instruction[i] << endl;
        if(instruction[0] != "#")
        {    
            parseinator(instruction, command, argument, position, connect);
            commandifier(command, argument, connect);
            command = "";
            argument = "";
            for(; position < instruction.size();)
            {
                parseinator(instruction, command, argument, position, connect);
                if(connect.runNext())
                    commandifier(command, argument, connect);
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
    if(input[position] == "&")
    {
        connect.type = 1;
        position += 2;
    }
    if(input[position] == "|")
    {
        connect.type = 2;
        position += 2;
    }
    if(input[position] == ";")
    {
        connect.type = 0;
        position ++;
    }
    if(input[position] != "#")
    {
        command = input[position];
        position++;
    }
    for(; position < input.size(); position++)
    {
        if(input[position] == "#")
        {
              cout << "I AM IN HERE" << endl;
            position = input.size();
            connect.type = 1;
            connect.run = false;
            break;
        }
        if(input[position] == "&" || input[position] == "|" || input[position] == ";")
            break;
        if(input[position] == "-")
        {
            while(input[position] != ";" && input[position] != "|" && input[position] != "&" && position+1 < input.size())
            {
                argument += input[position];
                position++;
            }
            if(position+1 == input.size() || input[position] == ";" || input[position] == "|" || input[position] == "&");
            {
               // argument += input[position];
                position++;
                break;
            }
            break;
        }
        argument += input[position];
        if(position+1 != input.size() && command == "echo")
            argument += " ";
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
    char * args[2];
    char * args1[3];
    bool no_arg = true;
    bool failed = false;
    cout<<argument.size();
    if(argument.size() ==0)
	args = { char_command, NULL};
    else
    {
	no_arg = false;
	args1 = { char_command, char_argument, NULL} ;
    }
    pid_t c_pid, pid;
    int status;

    c_pid = fork();

    if( c_pid < 0)
    {
        perror("fork failed");
        exit(1);
    }

    else if (c_pid == 0)
    {

        printf("Child: executing ls\n");
 	if(no_arg)
	{                                                                                             
        	execvp( args[0], args);
        	if(execvp( args[0], args) == -1) 
                {                                                                                                     
                    perror("execve failed");
                    connect.run = false;
                    failed = true;
                }
	}
	else
	{
 		execv(args1[0],args1);
		if(execvp(args1[0],args1) == -1)
                {
                    perror("exeve failed");
                    connect.run = false;
                    failed = true;
                }
	}
    }
    else if (c_pid > 0)
    {
        if( (pid = wait(&status)) < 0)
        {
            perror("wait");
            exit(1);
        }

        printf("Parent: finished\n");

        
    }
    if(!failed)
        connect.run = true;
   // return 0;
}
