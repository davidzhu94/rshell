#include <iostream>
#include <string.h>

using namespace std;



int main()
{
    string command;
    
    while (command != "exit")
    {
        cout << "#rshell ->$ ";
        cin >> command;
    }
    return 0;
}
