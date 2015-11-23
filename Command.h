#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>


using namespace std;

class Command 
{
	public:
		virtual string get_command() = 0;
		virtual bool isConnector() = 0;
		virtual void execute() = 0;
		virtual void set_left(Command* n) = 0;
		virtual void set_right(Command* n) = 0;
		virtual Command* get_left() = 0;
		virtual Command* get_right() = 0;
		virtual bool get_result() = 0;
};

class Exec : public Command
{
	private: 
		string s;
		bool connector;
		bool result;
		Command* left;
		Command* right;

	public:
		Exec(string new_s, bool new_result, bool new_connector, Command* new_left, Command* new_right);

		virtual string get_command() {return s;}
		virtual bool get_result(){return result;}
		virtual bool isConnector() {return connector;}
		virtual void set_left(Command* n){right = n;}
		virtual void set_right(Command* n){left = n;}
		virtual Command* get_left(){return left;}
		virtual Command* get_right(){return right;}
		void executeCmd(string c);
		void parse(string userline, char* list[]);

		virtual void execute()
		{
			executeCmd(s);
		}


};

class Test : public Command
{
	private: 

		string s;
		bool connector;
		bool result;
		Command* left;
		Command* right;

	public:

		Test(string new_s, bool new_connector, bool new_result, Command* new_left, Command* new_right); 

		virtual string get_command() {return s;}
		virtual bool get_result(){return result;}
		virtual bool isConnector() {return connector;}
		virtual void set_left(Command* n){right = n;}
		virtual void set_right(Command* n){left = n;}
		virtual Command* get_left(){return left;};
		virtual Command* get_right(){return right;}

		void executeTest(string cmd);
		void parse(string userline, char* list[]);
		vector<string> splitOnSpaces(string s);

		virtual void execute()
		{
			executeTest(s);
		}


};
class And : public Command
{
	private:
		string s;
		bool connector;
		bool result;
		Command* left;
		Command* right;

	public:
		And(string new_s,  bool new_connector, bool new_result, Command* new_left, Command* new_right); 
		virtual bool get_result(){return result;}
		virtual void set_left(Command* n){right = n;}
		virtual void set_right(Command* n){left = n;}
		virtual Command* get_left(){return left;}
		virtual Command* get_right(){return right;}
		virtual string get_command() {return s;}
		virtual bool isConnector() {return connector;}

		virtual void execute()
		{
			bool one = false;
			bool two = false;
			
			if(left == NULL or right == NULL)
			{
				throw invalid_argument("Error: Expected Addtional Arugments.");
				return;
			}
			get_left()->execute();
			one = get_left()->get_result();

			if(one)
			{	
				get_right()->execute();
				two = get_right()->get_result();

				if(two)
					result = true;
				else
					result = false;
			}
			else
				result = false;
		}


};

class Or : public Command
{
	private:
		string s;
		bool connector;
		bool result;
		Command* left;
		Command* right;

	public:
		Or(string new_s,  bool new_connector, bool new_result,Command* new_left, Command* new_right); 

		virtual void set_left(Command* n){right = n;}
		virtual void set_right(Command* n){left = n;}
		virtual Command* get_left(){return left;}
		virtual Command* get_right(){return right;}
		virtual string get_command() {return s;}
		virtual bool get_result(){return result;}
		virtual bool isConnector() {return connector;}

		virtual void execute()
		{
			if(left == NULL or right == NULL)
			{
				throw invalid_argument("Error: Expected Addtional Arugments.");
				return;
			}
			bool one = false;
			bool two = false;
			
			get_left()->execute();
			one = get_left()->get_result();

			if(one)
				result = true;
			else
			{
				get_right()->execute();
				two = get_right()->get_result();
				if(two)
					result = true;
				else
					result = false;
			}
		}

};

class Semicolon : public Command
{
	public:
		Semicolon(string new_s,  bool new_connector, bool new_result,Command* new_left, Command* new_right); 
		virtual void set_left(Command* n){right = n;}
		virtual void set_right(Command* n){left = n;}
		virtual Command* get_left(){return left;}
		virtual Command* get_right(){return right;}
		virtual bool get_result(){return result;}
		virtual string get_command() {return s;}
		virtual bool isConnector() {return connector;}

		virtual void execute()
		{
			if(right != NULL)
			{
				get_right()->execute();
				result = get_right()->get_result();
			}

			if(left != NULL)
			{
				get_left()->execute();
				result = get_left()->get_result();
			}
		}

	private:
		string s;
		bool connector;
		bool result;
		Command* left;
		Command* right;

};

class Parenthesis : public Command
{
	public:
	
		Parenthesis(string new_s,  bool new_connector, bool new_result,Command* new_left, Command* new_right); 
		virtual void set_left(Command* n){right = n;}
		virtual void set_right(Command* n){left = n;}
		virtual Command* get_left(){return left;}
		virtual Command* get_right(){return right;}
		virtual string get_command() {return s;}
		virtual bool get_result(){return result;}
		virtual bool isConnector() {return connector;}

		virtual void execute()
		{
			throw invalid_argument("Error: Invalid Tree.");
			return;
		}

	private:
		string s;
		bool connector;
		bool result;
		Command* left;
		Command* right;
};


#endif
