#ifndef TREE_H_
#define TREE_H_

#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <ctype.h>
#include <string.h>
#include <cctype>
#include <list>
#include <queue>
#include <vector>
#include "Command.h"

using namespace std;

//Tree Class.
class Tree
{

//The Root Pointer of the BST.
private:
	Command* root;
	Command* tmpRoot;
	queue<Command*> cs;

private:
	
	void prints(queue<Command*> s);

	//Attempts to insert a node with the given string.
	static Command* insert(Command* new_command, Command* n)
	{
		//check if the node pointer is null,
		//if it is, create a new node at this location.
		//initializing with constructor.
		if(n == NULL)
			n = new_command;
		//Check if we have a connector, if its a connector, then its a new root.
		else if(new_command->isConnector())
		{
			Command* tmp = new_command;
			tmp->set_right(n);
			n = tmp;
		}
		//Otherwise its an command.
		else
			n->set_left(new_command);

		return n;
	}

	//This function prints the tree sideways to the screen.
	//Uses in order transversal to print.
	void printTreeSideways(Command* n);

	
public:

	//Default Constructor.
	Tree(){root = NULL;tmpRoot = NULL;}
	//Inserts a node into the tree.
	void insert(Command* command){root = insert(command,root);}

	//Prints the tree to the screen.
	void printTreeSideways(){printTreeSideways(root);}

	//Utility parsing functions.
	vector<string> parse_userline(string line);
	string parseOn( vector<string> &v, string line, int pos, string x);
	bool checkLine(string userline);
	void hashCheck(string& userline, int start);
	void print(vector<string> v);
	string checkChar(string line, int start, int& pos);
	int smallest(int a, int b, int c, int d, int e);
	string removeAll(string s, char x);
	string removeAllChars(string s, char * charlist);

	Command* commandType(string s);

	// creates a vector of executable commands
	void buildCommandqueue(vector<string> sv)
	{
		for(unsigned int i = 0; i < sv.size(); i++)
		{
			Command* currentCommand = commandType(sv[i]); 
			cs.push(currentCommand);
		}
	}
	// initates and inorder traversal of the execute function to all classes
	// which are nodes or leaves within the tree
	void execute()
	{
		if(root != NULL)
			root->execute();
	}
	// combines two tree's in the case in which precedence dictates	
	static Command* merge_trees(Command* root, Command* tmpRoot)
	{
		if(root == NULL)
			root = tmpRoot;
		else if(tmpRoot != NULL)
			root->set_left(tmpRoot);
		return root;
	}
	// takes a command queue and inserts the class instances into a tree
	// as per insert algorithm
	void buildTree(string userline)
	{
		vector<string> sv;
		sv = parse_userline(userline);
		buildCommandqueue(sv);
		
		while(!cs.empty())
		{
			Command* new_command = cs.front();
			cs.pop();

			if(new_command->get_command() == ")")
			{
				root = merge_trees(root, tmpRoot);
				tmpRoot = NULL;
			}
			else if(new_command->get_command() != "(" and tmpRoot == NULL)
				root = insert(new_command, root);
			else if(new_command->get_command() != "(" and tmpRoot != NULL)
				tmpRoot = insert(new_command,tmpRoot);
			//Current command is a "(".
			else
			{
				if(!cs.empty())
				{
					new_command = cs.front();
					cs.pop();
					while(new_command->get_command() == "(")
					{
						new_command = cs.front();
						cs.pop();
					}
				}
				else
				{
					throw invalid_argument("Error: Invalid use of parenthesis.");
					return;
				}
				if(root == NULL)
					root = insert(new_command,root);
				else
					tmpRoot = insert(new_command, tmpRoot);
			}
		}
	}
};



#endif
