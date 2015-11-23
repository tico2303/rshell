#include <iostream>
#include <stdexcept>
#include "tree.h"

using namespace std;

int main()
{
	
	for(;;)
	{
		string userline;
		cout << "$";
		getline(cin, userline);
		try
		{
			Tree tree;
			tree.buildTree(userline);
			tree.execute();	

		}
		catch(invalid_argument e)
		{
			cout << e.what() << endl;
		}
		catch(...)
		{
			cout << "Catch all." << endl;
		}
	}

	return 0;
}
