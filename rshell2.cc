#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>
#include <string.h>
#include <queue>
#include <list>
#include <algorithm>
#include <typeinfo>

using namespace std;

void parse(string userline, char* list[]);
int arg_num(string userline);
queue<string> semiparse(string userline);
int smallest(int a,int b,int c);
void printq(queue<string> q);
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
		cout << "$" ;

		//q = semiparse(userline);
		getline(cin, userline);
		q = parse_userline(userline);

		while(!q.empty())
		{
			if(logic(q,previous))
			{
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

				int size = arg_num(userline);
				char* args = new char[500];

				char quit[] = "quit";
				
				//parses on space
				parse(command,args);

				if(strcmp(quit,args[0]) == 0)
				{
					cout << "QUITTING" << endl;
					exit(0);
				}

				pid_t pid = fork();

				if (pid < 0)
					exit(1);
				else if(pid == 0)
				{
					if(execvp(args[0],args) == -1)
					{
						cerr << "Invalid Command." << endl;
						exit(0);
					}
				}
				else
				{
					int wait_return = 0;
					int status = 0;
					int childret = 0;
					cout << endl;

					//Use to see if previous command success or fail. Returns a certain number if it fails.
					wait_return = wait(&status);
					childret = WEXITSTATUS(status);

					if(childret != 0)
						previous = false;
				}
			}
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

queue<string> parse_userline(string line)
{
	queue<string> q;
	string s;

	while(line.length() > 0)
	{
		int semi = 0;
		int orr = 0;
		int andd = 0;
		int pos = 0;

		semi = line.find_first_of(";",0);
		orr = line.find_first_of("||",0);
		andd = line.find_first_of("&&",0);

		pos = smallest(semi,orr,andd);

		if(pos == -1)
		{
			if(line.find_first_not_of(" ") == string::npos)
				return q;
			q.push(line);
			line = "";
		}
		else if(pos == semi)
		{
			string linecpy(line);
			if(!strtok((char*)linecpy.c_str(), ";"))
			{
				cout << "Error. not enough arguments." << endl;
				return q;
			}
			s = strtok((char*)line.c_str(), ";");
			q.push(s);
			q.push(";");
			line.erase(0, pos+1);
		}
		else if(pos == orr)
		{
			string linecpy(line);
			if(!strtok((char*)linecpy.c_str(), "|"))
			{
				cout << "Error. not enough arguments." << endl;
				return q;
			}
			s = strtok((char*)line.c_str(), "|");
			q.push(s);
			q.push("||");
			line.erase(0, pos+2);
		}
		else	
		{
			string linecpy(line);
			if(!strtok((char*)linecpy.c_str(), "&"))
			{
				cout << "Error. not enough arguments." << endl;
				return q;
			}
			s = strtok((char*)line.c_str(), "&");
			q.push(s);
			q.push("&&");
			line.erase(0, pos+2);
		}
	}

	return q;
}

bool logic(queue<string>& q, bool previous)
{
	string command;
	if(!q.empty())
		command = q.front();
	else
	{
		cout << "Error, not enough arguments." << endl;
		exit(0);
	}

	if(command == "&&")
	{
		q.pop();
		if(previous == true)
			return true; 
		else
			return false;
	}

	else if(command == "||")
	{
		q.pop();
		if(previous == true) 
		 	return false;
		else
		 	return true;
	}
	else if(command == ";")
	{
		q.pop();
		return true;
	}
	else
		return true;
}

int smallest(int a,int b,int c)
{
	list<int> v;
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	v.remove(-1);

	if(v.empty())
		return -1;
	else
		v.sort();

	return v.front();
}

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
		i++;
		list[i] = strtok(NULL, " ");
		j = list[i];
	}

	list[i+1] = NULL;
}

int arg_num(string userline)
{
	char *c = new char[userline.length()+1];
	char *j;

	strcpy(c, userline.c_str());
	j = strtok(c, " ");

	int i = 0;
	while(j)
	{
		j = strtok(NULL, " ");
		i++;
	}

	return i;
}

queue<string> semiparse(string userline)
{
	queue<string> sl;
	char* cp;
	
	getline(cin, userline);
	cp = strtok((char*)userline.c_str(), ";");
	
	while(cp)
	{
		sl.push(cp);
		cp = strtok(NULL, ";");
	}

	return sl;
}

void printq(queue<string> q)
{
	string x;
	queue<string> temp = q;

	cout << "Printing Queue: " << endl;
	while(!temp.empty())
	{
		x = temp.front();
		cout << x << endl;
		temp.pop();
	}
}


