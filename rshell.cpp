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

void parseinator(vector<string> input, string& command, string& argument, int& position);
void commandifier(string command, string argument);

class Connector
{
    public:
    Connector();
    ~Connector();
    bool run;
    virtual void runNext(bool succeed)= 0;
};

Connector::Connector()
{
    run = false;
}

Connector::~Connector()
{
}

class And: public Connector
{
    void runNext(bool succeed);
};

void And::runNext(bool succeed)
{
    if(succeed)
        run = true;
    else
        run = false;
}

class Or: public Connector
{
    void runNext(bool succeed);
};

void Or::runNext(bool succeed)
{
    if(succeed)
        run = false;
    else
        run = true;
}

class Do: public Connector
{
    void runNext(bool succeed);
};

void Do::runNext(bool succeed)
{
    run = true;
}
int main()
{
    string str;
    string command;
    string argument;
    char hostname[100];
    int position = 0;
    cout<<getlogin();
    gethostname(hostname,sizeof hostname);
    cout<<"@"<<hostname<<"$ ";
    getline(cin, str);
    vector<string> instruction;
    typedef tokenizer<boost::char_separator<char> > Tok;
    char_separator<char> sep; // default constructed
    Tok tok(str, sep);
    for(Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
        instruction.push_back(*tok_iter);
    for(int i = 0; i < instruction.size(); i++)
        cout << instruction[i] << endl;
    for(; position < instruction.size();)
    {
        parseinator(instruction, command, argument, position);
        commandifier(command, argument);
        command = "";
        argument = "";
    }
}

void parseinator(vector<string> input, string& command, string& argument, int& position)
{
    command = input[position];
    position++;
    for(; position < input.size(); position++)
    {
        if(input[position] == ";")
        {
	
            position++;
            break;
        }
        if(input[position] == "|" || input[position] == "&")
        {
            position += 2;
            break;
        }
        if(input[position] == "#")
        {
            position = input.size();
            break;
        }
        argument += input[position];
        argument += " ";
    }
}

void commandifier(string command, string argument)
{
    const char * const_command = command.c_str();
    char char_command[command.size()];
    const char * const_argument = argument.c_str();
    char char_argument[argument.size()];
    strcpy (char_command, const_command);
    strcpy (char_argument, const_argument);
    char * args[3] = { char_command, char_argument, NULL} ;
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
        execvp( args[0], args);
        if(execvp( args[0], args) == -1)
        {
            
        }                                                                                                      
        perror("execve failed");

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
   // return 0;
}
