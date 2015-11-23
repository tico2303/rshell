#include "Command.h"

using namespace std;

void printv(vector<string> v);

Exec::Exec(string new_s, bool new_connector, bool new_result,Command* new_left, Command* new_right) : s(new_s), connector(new_connector), result(new_result),left(new_left), right(new_right){}

//forks process and runs command
void Exec::executeCmd(string s)
{
	const int size = 500;
	char* args[size];
	char quit[] = "exit";
	int childret = 0;
	int status = 0;
	result = true;

	parse(s,args);

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
			perror("Invalid Command.");
			exit(-7);
		}
	}
	else
	{
		//This makes the parent process wait for the child process to finish.
		if(wait(&status) == -1)
			perror("Error occured with wait sys call");
		//Use to see if previous command was a success or a failure. Returns a certain number if it fails.
		childret = WEXITSTATUS(status);
		if(childret != 0)
			result = false;
	}
}

// takes the userline string and parses on spaces saving results
// to a char* list
void Exec::parse(string userline, char* list[])
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

Test::Test(string new_s, bool new_result, bool new_connector,Command* new_left, Command* new_right) : s(new_s),  connector(new_connector),result(new_result), left(new_left), right(new_right){}

// logic for the test execute
void Test::executeTest(string cmd)
{
	struct stat sb;
	vector<string> argv = splitOnSpaces(cmd);
	int size = argv.size()-1;
	int dirIndex = 0;
	int x = 0;

	if(argv[0] != "test" and argv[0] != "[") 
	{
		throw invalid_argument("Invalid use of test command");
		return;
	}
	else if(argv[0] == "[" and argv[size] != "]")
	{
		throw invalid_argument("Invalid use of test command.");
		return;
	}

	if(argv[0] == "[")
		dirIndex = size - 1;
	else 
		dirIndex = size;

	
	if(argv[1][0] == '-')
	{
		if(argv[1][1] != 'd' and  argv[1][1] != 'f' and argv[1][1] != 'e')
		{
			throw invalid_argument("Error: Invalid Test Option.");
			return;
		}
	}
	if(stat((char*)argv[dirIndex].c_str(), &sb) == -1)
		perror("stat");
	else if(argv[1] == "-d")
	{
		bool isdir =  S_ISDIR(sb.st_mode); 
		if(isdir == 1)
			result = true;
		if(isdir == 0)
			result = false;
	}
	else if(argv[1] == "-f")
	{
		bool isreg = S_ISREG(sb.st_mode);
		if(isreg == 1)
			result = true;
		if(isreg == 0)
			result = false;
	}
	else
	{
		x =  stat((char*)argv[dirIndex].c_str(), &sb);
		if(x == 0)
			result = true;
		if(x == 1)
			result = false;
	}
}
void printv(vector<string> v)
{
	for(unsigned int i = 0; i<v.size(); i++)
	{
		cout << v[i] <<endl;
	}
}
//splits string on spaces
vector<string> Test::splitOnSpaces(string s)
{
	vector<string> v;
	istringstream iss(s);

	do
	{
		string sub;
		iss >> sub;
		if(sub.find_first_not_of(" ") !=string::npos)
			v.push_back(sub);

	}while(iss);

	return v;
}
//--Constructors for classes--
And::And(string new_s, bool new_connector, bool new_result, Command* new_left, Command* new_right) : s(new_s),  connector(new_connector), result(new_result),left(new_left), right(new_right){}

Or::Or(string new_s, bool new_connector, bool new_result, Command* new_left, Command* new_right) : s(new_s),  connector(new_connector), result(new_result),left(new_left), right(new_right){}


Semicolon::Semicolon(string new_s, bool new_connector, bool new_result, Command* new_left, Command* new_right) : s(new_s), connector(new_connector), result(new_result), left(new_left), right(new_right){}

Parenthesis::Parenthesis(string new_s, bool new_connector, bool new_result, Command* new_left, Command* new_right) : s(new_s), connector(new_connector), result(new_result), left(new_left), right(new_right){}


