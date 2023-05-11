#include "AVL.h"
using namespace std;

int main() 
{
    string name;
    string ufid;
    string space = " ";
    AVLTree T;

    // read in first line and create variable for the number of commands (lineCount)
    string line;
    getline(cin, line);
    int lineCount = stoi(line);

    // for each command, execute it on the AVLTree T
    for (int i = 0; i < lineCount; ++i)
    {
        // read current line, stop when a space reached, set the read string as the "command" to be called in the AVL Tree
        getline(cin, line);          
        string command = line.substr(0, line.find(space));

        //============================ INSERT NAME ID ============================= //
        if (command == "insert")
        {
            // bool variables for checking if the node can be inserted or not
            bool validName = true;
            bool validId = true;

            // erase line until just after first double quote, set name equal to the substring from line(0) until the second double quote
            line.erase(0, line.find(space) + 2);
            name = line.substr(0,line.find("\""));


            // for each character in name, check if its a letter or space
            for (char c : name)
            {
                if(!isalpha(c) && !isspace(c))
                {
                    // if a character in "name" is not a letter, invalid name -> cannot insert
                    validName = false;
                }
            }
            
            // get ufid number from continuing input parsing
            line.erase(0, name.length() + 2);
            ufid = line.substr(0, line.find(space));


            // check number of digits in ufid
            if(ufid.length() != 8)
            {
                // if ufid doesn't contain exactly 8 digits, invalid ufid -> cannot insert
                validId = false;
            }
            
            // for each char in ufid, check if its a digit
            for (char c : ufid)
            {
                if(!isdigit(c))
                {
                    // if a character in "ufid" is not a digit, invalid ufid -> cannot insert
                    validId = false;
                }
            }

            if (validName && validId)
            {
                // if input for name and ufid are valid, insert node into AVLTree "T"
                T.insert(name,ufid);
            }
            else
            {
                // else, display "unsuccessful"
                cout << T.unsuccess << endl;
            }

        }

        //============================ REMOVE ID ============================= //
        else if (command == "remove")
        {
            // erase line until the first digit of the ufid, set ufid to be the string until the next space is reached
            line.erase(0, line.find(space) + 1);
            ufid = line.substr(0, line.find(space));

            // call to remove function in AVLTree class...
            T.remove(ufid);
        }

        //============================ REMOVEINORDER N ============================= //
        else if (command == "removeInorder")
        {
            // erase line until the digit of n, set n to be the string until the next space is reached
            line.erase(0, line.find(space) + 1);
            string n = line.substr(0, line.find(space));

            // call to removeInorder function in AVLTree class
            T.removeInorder(stoi(n));

        }

        //============================ SEARCH COMMANDS ============================= //
        else if (command == "search")
        {
            // erase line until the first character after the space, set readline to be the string until the next space is reached
            line.erase(0, line.find(space) + 1);
            string readLine = line.substr(0, line.find(space));
            //============================ SEARCH ID ============================= //
            if (isdigit(readLine[0]))
            {
                // if first character of readLine is a digit, then we are searching for a ufid
                ufid = readLine;

                // call to searchId function in AVLTree class
                T.searchId(ufid);
            }
            //============================ SEARCH NAME ============================= //
            else
            {   
                line.erase(0,1);
                // else, we are searching for a name (disregard "readLine"), set name to be the string until the next double quote is reached 
                string name = line.substr(0,line.find("\""));

                // call to searchName function in AVLTree class...
                T.searchName(name);
            }
        }

        //============================ PRINT COMMANDS ============================= //

        else if (command == "printInorder")
        {
            // call to printInorder function
            T.printInorder();
        }

        else if (command == "printPreorder")
        {
            // call to printPreorder function
            T.printPreorder();
        }

        else if (command == "printPostorder")
        {
            // call to printPostorder function
            T.printPostOrder();
        }

        else if (command == "printLevelCount")
        {
            // call to printLevelCount function
            T.printLevelCount();
            cout << endl;
        }

        else
        {
            // print "unsuccessful" for misspelled or invalid commands
            cout << T.unsuccess << endl;
        }

    }

    return 0;
}
