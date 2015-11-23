#include <iostream>
#include <cstdlib>
#include <vector>
#include <list>
#include <string>
#include <string.h>
#include <cstring>
#include "tree.h"

using namespace std;

void Tree::print(vector<string> v)
{
	for(unsigned int i = 0; i < v.size(); i++)
		cout << v[i] <<  ", " << endl;
}

string Tree::checkChar(string line, int start, int& pos)
{
	int semi = 0;
	int orr = 0;
	int andd = 0;
	int lp = 0;
	int rp = 0;
	int ql = 0;
	int qr = 0;
	string x;
	string temp;

	semi = line.find_first_of(";",start);
	orr = line.find_first_of("||",start);
	andd = line.find_first_of("&&",start);
	lp= line.find_first_of("(",start);
	rp= line.find_first_of(")",start);
	ql = line.find_first_of("\"", start);
	qr = line.find_first_of("\"", ql+1);

	//Save the position of the first encountered connector.
	pos = smallest(semi,orr,andd, lp, rp);
	
	if(pos == -1)
		return "";

	if(ql != -1 and qr != -1)
	{
		if( pos > ql and pos < qr)
		{
			qr++;
			temp = checkChar(line,qr,pos);
			return temp;
		}
	}

	if(pos == semi)
		x = ";";
	else if(pos == orr)
	{
		x = "||";
		if(line[pos+1] == '|')
			pos++;
		else
		{		
			throw invalid_argument("Syntax Error near |.");
			return "";
		}
	}
	else if(pos == andd)
	{
		x = "&&";
		if(line[pos+1] == '&')
			pos++;
		else
		{		
			throw invalid_argument("Syntax Error near &.");
			return "";
		}
	}
	else if(pos == lp)
		x = "(";
	else if(pos == rp)
		x = ")";
	else
		x = "";

	return x;
}

int Tree::smallest(int a, int b, int c, int d, int e)
{
	//Put the three values in a list.
	list<int> v;
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	v.push_back(d);
	v.push_back(e);
	
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

string Tree::parseOn(vector<string> &v, string line, int pos, string x)
{
	string s;
	int newpos;
	int i = 0;

	if(pos == -1)
	{
		//If its all white space, there no command to execute at all.
		if(line.find_first_not_of(" ") == string::npos)
		{
			line ="";
			return line;
		}
		else
		{
			i = line.find_first_not_of(" ");
			line.erase(0,i);
			//Otherwise, push_back the command onto the queue.
			v.push_back(line);
			//Then update the line to empty because this is the last
			//command if there is no connectors left.
			line = "";
			return line;
		}
	}
	if(pos !=0)
	{
		newpos = line.find_first_of(x);
		s = line.substr(0,newpos);
		if(s == "")
		{
			throw invalid_argument("Error(2): Expected Addition Argument.");
				return "";
		}
		if(s.find_first_not_of(" ") != string::npos)
		{
			i = s.find_first_not_of(" ");
			s.erase(0,i);
			v.push_back(s);
		}
	}
	v.push_back(x);
	//Updates the string after parsing.
	line.erase(0, pos+1);

	return line;
}

vector<string> Tree::parse_userline(string line)
{
	int pos = 0;
	string x;
	string copy;
	vector<string> vs;
	string hash = line;

	hashCheck(line, 0);

	copy = line;
	copy = removeAll(copy, ' ');
	if(copy.length()  < 1)
		return vs;

	if(line.length() > 0 and checkLine(line) == false)
	{
		throw invalid_argument("Error: Expected Additional Arguments.");
		return vs;
	}

	hash = removeAll(hash, '#');	
	if(hash.length() < 1)
		return vs;


	//While there is still commands left.
	while(line.length() > 0)
	{
		//If there any connectors, save their spots in the string.
		x = checkChar(line, 0, pos);
		//Check if there was no connector encountered.
		line = parseOn(vs, line, pos, x);
	}

	return vs;
}

void Tree::printTreeSideways(Command* n)
{
	//For outputting the spaces.
	int i = 0;
	static int space = 0;
	//If there is no tree.
	if(n == NULL)
	{
		throw invalid_argument("Error printing tree: tree is empty.");
		return;
	}

	//Increase space by 1 every time this function is called.
	space++;
	//If there is a right, go right.
	if(n->get_right() != NULL)
	{
		printTreeSideways(n->get_right());
	}
	//Outputs the spaces to the screen.
	while(i != space)
	{
		//If its the root, dont put space before it.
		if(i != 0)
		{
			cout << "          ";
			cout << "|";
		}
		i++;
	}
	//Print the string and the count.
	cout << n->get_command() << "(" << n->isConnector() << ")" << endl;
	//If there is a left child, go left.
	if(n->get_left() != NULL)
	{
		printTreeSideways(n->get_left());
	}
	space--;

}

Command* Tree::commandType(string x)
{
	if(x == "&&")
	{
		Command* A = new And("And", true, false, NULL, NULL);
		return A;
	}
	else if( x == "||")
	{
		Command* A = new Or("Or", true, false,  NULL, NULL);
		return A;
	}
	else if(x == ";")
	{
		Command* A = new Semicolon(";", true, false, NULL, NULL);
		return A;
	}
	else if(x == "(")
	{
		Command* A = new Parenthesis("(", true, false, NULL, NULL);
		return A;
	}
	else if(x == ")")
	{
		Command* A = new Parenthesis(")", true, false, NULL, NULL);
		return A;
	}
	else if( x.find("[") == 0)
	{
		Command* A = new Test(x, false, false, NULL, NULL);
		return A;
	}
	else if(x.find("test") == 0)
	{
		Command* A = new Test(x, false, false, NULL, NULL);
		return A;
	}
	else
	{
		x = removeAll(x,'\"');
		Command* A = new Exec(x, false, false, NULL, NULL);
		return A;
	}
}

bool Tree::checkLine(string userline)
{
	int quote = 0;
	int lb = 0;
	int rb = 0;
	string copy = userline;

	for(unsigned int i = 0; i < userline.length(); i++)
	{
		if(userline[i] == '\"')
			quote++;
		if(userline[i] == '[')
			lb++;
		if(userline[i] == ']')
			rb++;
	}
	
	if(quote%2 != 0)
	{
		throw invalid_argument("Error: Expected addition quote.");
		return false;
	}
	if((lb + rb)%2 != 0)
	{
		throw invalid_argument("Error: Expected additional brace.");
		return false;
	}

	char cs[] = ";()&|\" ";
	char* cp = cs;
	copy = removeAllChars(copy, cp);	
	if(copy.length()  < 1)
		return false;

	return true;
}

void Tree::hashCheck(string& userline, int start)
{
	int leftQuote = userline.find_first_of("\"",start);
	int rightQuote = userline.find_first_of("\"", leftQuote+1);
	int hashtag = userline.find_first_of("#",start);
	
	if(hashtag == -1) 
		return;
	else if(leftQuote == -1)
		userline = userline.substr(0, hashtag);
	else if(hashtag < leftQuote)
		userline = userline.substr(0, hashtag);
	else
		hashCheck(userline,rightQuote+1); 
}

void Tree::prints(queue<Command*> s)
{
	queue<Command*> temp(s);
	cout << "Printint Queue." << endl;
	if(temp.empty())
		cout << "Queue Empty." << endl;

	while(!temp.empty())
	{
		cout << temp.front()->get_command()<< endl;
		temp.pop();
	}
}

string Tree::removeAll(string s, char x)
{
	if(s.find_first_of(x,0) != string::npos)
		s.erase( remove(s.begin(), s.end(), x), s.end() );

	return s;
}

string Tree::removeAllChars(string s, char * charlist)
{
	for(unsigned int i = 0; i < strlen(charlist); i++)
		s.erase( remove(s.begin(), s.end(), charlist[i]), s.end() );
	return s;
}
   

