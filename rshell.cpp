#include <iostream>
#include <string>
#include <boost/tokenizer.hpp>
#include <vector>
using namespace std;
using namespace boost;
/*
int main(){
   string s;
   getline(cin, s);
   tokenizer<escaped_list_separator<char> >  tok(s);
   for(tokenizer<escaped_list_separator<char> > ::iterator beg=tok.begin(); beg!=tok.end();++beg){
       cout << *beg << "\n";
   }
}
*/
int main()
{
    string str;
    getline(cin, str);
    vector<string> command;
    typedef tokenizer<boost::char_separator<char> > Tok;
    char_separator<char> sep; // default constructed
    Tok tok(str, sep);
    for(Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end(); ++tok_iter)
        command.push_back(*tok_iter);
    for(int i = 0; i < command.size(); i++)
        cout << command[i] << endl;
    return 0;
}


string parseinator(vector<string> input,  int& position)
{
    string output;
    for(; position < input.size(); position++)
    {
        if(input[position] == "-;")
        {
            position++;
            break;
        }
        if(input[position] == "|" || input[position] == "&")
        {
            position += 2;
            break;
        }
        output += input[position];
        output += " ";
    }
    return output;
} 
