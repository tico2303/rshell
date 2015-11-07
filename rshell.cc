#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>
#include <string.h>
#include <queue>
#include <list>

using namespace std;

//Size for the char* args passed to execvp. 
//Pedantic does not allow dynamic array size so 
//this const size is used.
const int size = 500;

void parse(string userline, char* list[]);
int smallest(int a,int b,int c);
queue<string> parse_userline(string line);
bool logic(queue<string>& q, bool previous);

int main()
{
	string userline;
	string command;
	queue<string> q;
	
	start:

	while(1)
	{
		bool previous = true;
		int status = 0;
		int childret = 0;
		char* args[size];
		char quit[] = "exit";

		cout << "$";
		getline(cin, userline);

		//Takes in the line the user entered and parses on all connectors,
		//and forming returning a queue.
		q = parse_userline(userline);

		//While there is still commands to execute.
		while(!q.empty())
		{
			//Check if the next command should be executed according to the
			//logic of the connectors and the bool reult of the last command 
			//executed.
			if(logic(q,previous))
			{
				//Get the next command to be executed.
				if(!q.empty())
				{
					command = q.front();
					q.pop();
				}
				else
				{
					cout << "Expected additional arugment." << endl;
					goto start;
				}

				//Create the args array for execvp.
				//Parse the current command string on spaces to seperate commmand
				//and options and place them into the agrs array for execvp.
				parse(command,args);

				//Check if the command is exit.
				if(strcmp(quit,args[0]) == 0)
					exit(0);

				//Fork a new process.
				pid_t pid = fork();

				//Check if the process forked successfully.
				if (pid < 0)
					exit(1);
				//Check if this is the child process.
				//This is where execvp will be called and the command will be executed.
				else if(pid == 0)
				{
					//Check if the command failed to execute.
					if(execvp(args[0],args) == -1)
					{
						cerr << "Invalid Command." << endl;
						exit(1);
					}
				}
				else
				{
					//This makes the parent process wait for the child process to finish.
					wait(&status);
					//Use to see if previous command was a success or a failure. Returns a certain number if it fails.
					childret = WEXITSTATUS(status);
					if(childret != 0)
						previous = false;
				}
			}
			//If the next commmand should not be ran, pop it off the queue.
			//If the Queue is empty, restart the prompt.
			else
			{
				if(!q.empty())
					q.pop();
				else
				{
					cout << "Error, Not enough arguments." << endl;
					goto start;
				}
			}
		}
	}
}

//Takes in the user entered line, parses on connectors,
//and returns a queue of strings.
queue<string> parse_userline(string line)
{
	queue<string> q;
	string s;
	int hpos = 0;
	int semi = 0;
	int orr = 0;
	int andd = 0;
	int pos = 0;

	//First, see if there is a hashtag signaling everything after
	//is a comment and not part of the command.
	hpos = line.find_first_of("#", 0);
	//if one is found, remove the comment from the command that
	//will be executed.
	if(hpos != -1)
		line = line.substr(0,hpos);
	//While there is still commands left.
	while(line.length() > 0)
	{
		//If there any connectors, save their spots in the string.
		semi = line.find_first_of(";",0);
		orr = line.find_first_of("||",0);
		andd = line.find_first_of("&&",0);

		//Save the position of the first encountered connector.
		pos = smallest(semi,orr,andd);

		//Check if there was no connector encountered.
		if(pos == -1)
		{
			//If its all white space, there no command to execute at all.
			if(line.find_first_not_of(" ") == string::npos)
				return q;
			//Otherwise, push the command onto the queue.
			q.push(line);
			//Then update the line to empty because this is the last
			//command if there is no connectors left.
			line = "";
		}
		//Check if a semicolon was the first connector entered.
		else if(pos == semi)
		{
			string linecpy(line);
			//Checks to see if the line is composed only of semicolons.
			if(!strtok((char*)linecpy.c_str(), ";"))
			{
				cout << "Error, not enough arguments." << endl;
				return q;
			}
			//Uses strtok to parse on the ; character and pushes the strings onto the queue.
			//Preserves order of the commands and connector.
			s = strtok((char*)line.c_str(), ";");
			q.push(s);
			q.push(";");
			//Updates the string after parsing.
			line.erase(0, pos+1);
		}
		else if(pos == orr)
		{
			string linecpy(line);
			//Checks to see if the line is composed only of the character |.
			if(!strtok((char*)linecpy.c_str(), "|"))
			{
				cout << "Error, not enough arguments." << endl;
				return q;
			}
			//Uses strtok to parse on the | character and pushes the strings onto the queue.
			//Preserves order of the commands and connector.
			s = strtok((char*)line.c_str(), "|");
			q.push(s);
			q.push("||");
			//Updates the string after parsing.
			line.erase(0, pos+2);
		}
		else	
		{
			string linecpy(line);
			//Checks to see if the line is composed only of ampersand.
			if(!strtok((char*)linecpy.c_str(), "&"))
			{
				cout << "Error, not enough arguments." << endl;
				return q;
			}
			//Uses strtok to parse on the & character and pushes the strings onto the queue.
			//Preserves order of the commands and connector.
			s = strtok((char*)line.c_str(), "&");
			q.push(s);
			q.push("&&");
			//Updates the string after parsing.
			line.erase(0, pos+2);
		}
	}

	return q;
}

//Checks if the next command should be executed based on the connector encountered,
//and if the previous command was a success or failure.
bool logic(queue<string>& q, bool previous)
{
	string command;
	//If the queue isnt empty, get the next command.
	if(!q.empty())
		command = q.front();
	else
	{
		cout << "Error, not enough arguments." << endl;
		exit(0);
	}

	//Check if its the ampersand connector.
	if(command == "&&")
	{
		//Take the connector off the queue.
		q.pop();
		//Based on the success or failure of the previous command,
		//and the logic of the connectors, returns a bool value regarding
		//if the next command should be executed.
		if(previous == true)
			return true; 
		else
			return false;
	}
	//Check if its the || connector.
	else if(command == "||")
	{
		//Take the connector off the queue.
		q.pop();
		//Based on the success or failure of the previous command,
		//and the logic of the connectors, returns a bool value regarding
		//if the next command should be executed.
		if(previous == true) 
		 	return false;
		else
		 	return true;
	}
	//Check if its the ; connector.
	else if(command == ";")
	{
		//Take the connector off the queue.
		q.pop();
		//Based on the success or failure of the previous command,
		//and the logic of the connectors, returns a bool value regarding
		//if the next command should be executed.
		return true;
	}
	else
		return true;
}

//Returns the smallest of three values.
int smallest(int a,int b,int c)
{
	//Put the three values in a list.
	list<int> v;
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	//Remove any negative ones.
	v.remove(-1);

	//check if its empty, if so return -1.
	if(v.empty())
		return -1;
	else
	//sort the list.
		v.sort();

	//the smallest value in the list is now the front, return it.
	return v.front();
}

//Parses on white space, used to seperate commands and options
//to pass into the args array for execvp.
void parse(string userline, char* list[])
{
	int i = 0;
	char *c = new char[userline.length()+1];
	char* j;

	strcpy(c, userline.c_str());
	list[i] = strtok(c, " ");
	j = list[i];
	
	while(j)
	{
		//i keeps track of the index of the list array.
		i++;
		//pass the characters up to a white space into the list at 
		//index i.
		list[i] = strtok(NULL, " ");
		j = list[i];
	}
	//The last element of the array passed to execvp must be NULL.
	list[i+1] = NULL;
}