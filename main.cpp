

#include "BinarySearchTree.h"
#include "song.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace std;

// Constants for the menu choices
const char  LIST_CHOICE = 'L',
            //unsorted?

            //sort by title

            LIST_ID_CHOICE = 'P',//sort by title
            INDENTLIST_CHOICE = 'T', //display as tree
            SEARCH_CHOICE = 'S',
            //search by title

            //search by id

            DElETE_CHOICE = 'D',
            ADD_CHOICE = 'A',
            MENU_CHOICE = 'M',
            QUIT_CHOICE = 'Q';


void displayMenu();
bool validateID(string id_input);
void readData(string filename ,BinarySearchTree<Song>* tree);
bool performChoice(char choice, BinarySearchTree<Song>* tree);




int main()
{
    char choice;
    bool end_test = true;
    const string FILENAME = "songs.txt";
    
    
    BinarySearchTree<Song>* tree = new BinarySearchTree<Song>();
    
    
    readData( FILENAME , tree );
    
    displayMenu();
    
    do
    {
        cout << "\nEnter your choice ( M to show menu , Q to quit ):" ;
        cin >> choice;
        choice = toupper(choice);
        end_test = performChoice(choice, tree);
    }
    while(end_test);
    
    delete tree;
    
}
