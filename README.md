##### Jorge Flores & David Zhu 
# rshell
###Homework 2
####*CS 100*



##### Homework 1 
Our project consist of using a tokenizator in order to parse the input given. The input given on the terminal is then translated using different connectors we are able to see which commands are executed. We execute the commands using syscalls and we created a special built in command of exit. We were able to consider comments by not reading anything after the # character.
##### Homework 2 
For homework 2 we were suppose to create a test command,[]- square bracket acting as a test command, and precedence operator. We implemented it into our exisiting Rshell, and fixed the errors we encountered in our HW 1.
##### Test Command && []
It is set up as the test command in bash. This command returns 0 (TRUE) if the test succeeds and 1 (FALSE) if the test fails. This command is very useful for writing conditions that can be combined with && and || to write more complex bash command structures. The test command allows the user to run the command using the keyword test test -e /test/file/path. It also allows the user to use the symbolic version of the command [ -e /test/file/path ]. The test command also allows the user to run tests using the following ﬂags -e checks if the ﬁle/directory exists -f checks if the ﬁle/directory exists and is a regular ﬁle -d checks if the ﬁle/directory exists and is a directory If a user does not specify a ﬂag, then the -e functionality will be used by default. 
##### Design
We were able to parse the input given into certain strings that would allow us to execute the command and arguments. The strings were parsed into a vector which would hold the commands and argument. This would allow us to have the commands and arguments in order. We used a Connector class which would return the values of the connectors and set the value of the command. Through this connector class we were able to create a precedence type that would return true or false. This allowed us to check if there were a predence operator in the command or argument. Similarly, we created a test function which would return a boolean letting us know if the test argument is a directory or a regular file. The changes we could make to our program would be creating a better function to execute the commands. Instead of the function being a void we could make it a boolean return function in order to make it deal with the value instead of checking through if statements. 
