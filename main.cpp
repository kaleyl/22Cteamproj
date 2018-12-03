// main test driver for BST
// Created by Frank M. Carrano and Tim Henry.

#include "BinarySearchTree.h"
#include "HashTable.h"
#include "song.h"
#include "StackADT.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace std;

// Constants for the menu choices
const char  LIST_CHOICE = 'L',
            SEARCH_CHOICE = 'S',
            INSERT_CHOICE = 'I',
            DELETE_CHOICE = 'D',
            UNDO_DELETE_CHOICE = 'U',
            MENU_CHOICE = 'M',
            QUIT_CHOICE = 'Q',


void readData(string filename, BinarySearchTree<Song>*, BinarySearchTree<Song>*, HashTable<Song>*);
void performChoice (char);
bool validateID(string id_input);

void printMenu();
void ListManager(BinarySearchTree<Song>*, BinarySearchTree<Song>*, HashTable<Song>*);
void SearchManager(BinarySearchTree<Song>*, HashTable<Song>*);
void InsertManager(BinarySearchTree<Song>*, BinarySearchTree<Song>*, HashTable<Song>*);
void DeleteManager(BinarySearchTree<Song>*, BinarySearchTree<Song>*, HashTable<Song>*);
void UndoDelete(Stack<Song>);

//bool validateID(string id_input); // ?????

int main()
{
    char choice;
    bool end_test;
    const string FILENAME = "songs.txt";

    BinarySearchTree<Song>* SongIdTreePtr = new BinarySearchTree<Song>();
    BinarySearchTree<Song>* SongTitleTreePtr = new BinarySearchTree<Song>();

    HashTable<Song>* SongTable = new HashTable<Song>();

    readData( FILENAME , SongTreePtr );

    displayMainMenu();

    do
    {
        cout << "Enter your choice: " ;
        cin >> choice;
        choice = toupper(choice);
        preformChoice (choice);
    }
    while(end_test);

    delete SongTreePtr;

}

bool performChoice(char choice)
{
    switch (choice)
        {
            case LIST_CHOICE: ListManager(); break;
            case SEARCH_CHOICE: SearchManager(); break;
            case INSERT_CHOICE: InsertManager(tree); break;
            case DELETE_CHOICE: DeleteManager(tree); break;
            case UNDO_DELETE_CHOICE: UndoDelete(); break;
            case MENU_CHOICE: printMenu(); break;
            case QUIT_CHOICE: return false; break;
            default: cout << "Your input is incorrect!" << endl; break;
        }
    return true;
}

void printMenu()
{
    cout << "L - List all songs \n"
         << "S - Search a song \n"
         << "I - Insert a new song\n"
         << "D - Delete a song \n"
         << "U - Undo delete \n"
         << "Q - Write data to file and quit program \n\n";
}

void ListManager(BinarySearchTree<Song>*, BinarySearchTree<Song>*, HashTable<Song>*)
{
    const char UNSORTED_CHOICE = 'H',
               LIST_ID_CHOICE = 'P',//sort by title
               LIST_TITLE_CHOICE = 'I',
               INDENTLIST_CHOICE = 'T'; //display as tree

    cout << "U - unsorted list\n"
         << "P - sorted by ID \n"
         << "I - sorted by title \n"
         << "T - show tree \n"
         << "B - back to main menu \n\n";

    char choice;
    cout << "Enter your choice: " ;
    cin >> choice;
    choice = toupper(choice);

    switch (choice)
        {
            case UNSORTED_CHOICE: displayLMenu(); break; // change
            case LIST_ID_CHOICE: displaySMenu(); break; // change
            case LIST_TITLE_CHOICE: addData (tree); break; //change
            case INDENTLIST_CHOICE: deleteData (tree); break; // change
            case BACK_MENU_CHOICE: break;
            default:
                {
                    cout << "Your input is incorrect!" << endl;
                    displayLMenu();
                }
                break;
        }
}

void SearchManager(BinarySearchTree<Song>*, HashTable<Song>*)
{
    const char SEARCH_ID_CHOICE = 'P', // search by ID
               SEARCH_TITLE_CHOICE = 'S';//search by title

    cout << "P - searched by ID \n
         << "S - searched by title \n"
         << "B - back to main menu \n\n";

    char choice;
    cout << "Enter your choice: " ;
    cin >> choice;
    choice = toupper(choice);
    

    switch (choice)
        {
            case SEARCH_ID_CHOICE: displayLMenu(); break; // change
            case SEARCH_TITLE_CHOICE: displaySMenu(); break; // change
            case BACK_MENU_CHOICE: break;
            default:
                {
                    cout << "Your input is incorrect!" << endl;
                    SearchManager();
                }
                break;
        }
}

/*****************************************************************************
 This function reads data about songs from a given file and inserts it
 into a Binary Search Tree.
 *****************************************************************************/
void readData(string filename, BinarySearchTree<Song>* SongTreePtr)
{
    ifstream inputFile;
    inputFile.open(filename);

    if (inputFile.fail())
    {
        cout << "\nThe file failed to open! Please try again...\n";
        exit(EXIT_FAILURE);
    }

    // use local variables to store data from file and use them
    // to build a Song object
    Song localSong;
    string id, title, artist, album, genre;
    int duration, year;

    while(getline(inputFile, id, ';'))
    {
        getline(inputFile, title, ';');
        getline(inputFile, artist, ';');
        getline(inputFile, album, ';');
        getline(inputFile, genre, ';');
        getline(inputFile, duration, ';');
        getline(inputFile, year);

        localSong.setId(id);
        localSong.setTitle(title);
        localSong.setArtist(artist);
        localSong.setAlbum(album);
        localSong.setGenre(genre);
        localSong.setDuration(duration);
        localSong.setYear(year);

        // insert the Song object into the Binary Search Tree
        SongTreePtr->insert(localSong);
    }
    inputFile.close();
}

void InsertManager(BinarySearchTree<Song>*, BinarySearchTree<Song>*, HashTable<Song>*);
{
    Song localSong;
    string id, title, artist, album, genre;
    int duration, year;
    bool inserted=false;

    cout << "Enter the ID of the song you want to add: ";
    getline (cin, id); // check?
    validateID(id);      

    cout << "Enter the title of the song you want to add: ";
    getline (cin, title);

    cout << "Enter the artist of the song you want to add: ";
    getline (cin, artist);

    cout << "Enter the album of the song you want to add: ";
    getline (cin, album);

    cout << "Enter the genre of the song you want to add: ";
    getline (cin, genre);

    cout << "Enter the duration of the song you want to add: ";
    cin >> duration; // check?

    cout << "Enter the year of the song you want to add: ";
    cin >> year; // check?

    localSong.setId(id);
    localSong.setTitle(title);
    localSong.setArtist(artist);
    localSong.setAlbum(album);
    localSong.setGenre(genre);
    localSong.setDuration(duration);
    localSong.setYear(year);

    // insert the Song object into the Binary Search Tree
    inserted=SongTreePtr->insert(localSong);

    if (inserted)
        cout << "The song is inserted successfully!" << endl << endl;
    else
        cout << "The song cannot be inserted." << endl << endl;
}

void deleteData (BinarySearchTree<Song>* SongTreePtr)
{
    bool deleted=false;
    string id;
    BinarySearchTree<Song>* searchIdPtr;
    Song Entry, returnedItem;

    cout << "Enter the ID of the song you want to delete: ";
    cin >> id;

    Entry.setId (id);

    searchIdPtr = SongTreePtr->getEntry(Entry,returnedItem); //
    deleted = searchIdPtr->remove(returnedItem);

    if (deleted)
        cout << "The song is deleted successfully!" << endl << endl;
    else
        cout << "The song is not found." << endl << endl;
}

/************************************
 This function will check if the ID *
 given by the user is only number   *
 not alphabet                       *
 ************************************/
bool validateID(string id_input)
{
    int i = 0;
    while( i < id_input.length())
    {
        if(isalpha(id_input[i]))
        {
            cout << "INVALID INPUT! PLEASE ENTER NUMBER ONLY! PLEASE ENTER AGAIN!\n" << endl;
            return false;
        }
        i++;
    }
    return true;
}

