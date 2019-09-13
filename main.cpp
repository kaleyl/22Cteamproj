
#include "Song.h"
#include "BinarySearchTree.h"
#include "HashTable.h"
#include "StackADT.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace std;

// Constants for the menuc
const char  LIST_CHOICE = 'L',
            SEARCH_CHOICE = 'S',
            INSERT_CHOICE = 'I',
            DELETE_CHOICE = 'D',
            UNDO_DELETE_CHOICE = 'U',
            MENU_CHOICE = 'M',
            QUIT_CHOICE = 'Q';

//read data and get user input
void readData(string filename, BinarySearchTree<Song>*, BinarySearchTree<Song>*, HashTable<Song>*);
void writeData(string filename, HashTable<Song>*);
void performChoice(char, BinarySearchTree<Song>*, BinarySearchTree<Song>*, HashTable<Song>*, Stack<Song>&);

//reused functions
bool validateID(string);
void printMenu();
void displaySong(Song&);
void displaySongKey(Song&);
int compareId(const Song&, const Song&);
int compareTitle(const Song&, const Song&);

//list functions
void ListManager(BinarySearchTree<Song>*, BinarySearchTree<Song>*, HashTable<Song>*);
void ListUnsorted(HashTable<Song>* SongTable);
void ListById(BinarySearchTree<Song>* SongIdTreePtr);
void ListByTitle(BinarySearchTree<Song>* SongTitleTreePtr);
void ListAsIndentedTree(BinarySearchTree<Song>* SongIdTreePtr, BinarySearchTree<Song>* SongTitleTreePtr);

//search functions
void SearchManager(BinarySearchTree<Song>*, HashTable<Song>*);
void SearchById(HashTable<Song>* SongTable);
void SearchByTitle(BinarySearchTree<Song>* SongTitleTreePtr);

//insert and delete functions
void InsertManager(BinarySearchTree<Song>*, BinarySearchTree<Song>*, HashTable<Song>*);
void DeleteManager(BinarySearchTree<Song>*, BinarySearchTree<Song>*, HashTable<Song>*, Stack<Song>&);
void UndoDelete(Stack<Song> &,BinarySearchTree<Song>*, BinarySearchTree<Song>*, HashTable<Song>* );

int main()
{
    string choiceString;
    char choice;
    const string FILENAME = "songs.txt";
    
    // 2 bst and 1 hash table
    BinarySearchTree<Song>* SongIdTreePtr = new BinarySearchTree<Song>() ;
    BinarySearchTree<Song>* SongTitleTreePtr = new BinarySearchTree<Song>() ;
    //HashTable<Song>* SongTable = new HashTable<Song>(53);
    // Use below to test the rehashing function
     HashTable<Song>* SongTable = new HashTable<Song>(10);
    Stack<Song> deletedSongs;
    
    
    //readData from file
    readData( FILENAME, SongIdTreePtr, SongTitleTreePtr, SongTable );
    
    //display menu
    printMenu();
    
    //prompt user to enter their choice
    cout << "Enter your choice (L/S/I/D/U/M/Q): " ;
    cin >> choiceString;
    choice = toupper(choiceString[0]);
    
    
    //perform and validate choice until user choose to QUIT
    while (choice != QUIT_CHOICE)
    {
        performChoice(choice, SongIdTreePtr, SongTitleTreePtr, SongTable, deletedSongs);
        cout << "Enter your choice ('M' to show menu) (L/S/I/D/U/M/Q): " ;
        cin >> choiceString;
        choice = toupper(choiceString[0]);
    }
    
    
    //write new data to file
    char outputFileName[] = "songs_updated.txt";
    writeData(outputFileName,SongTable);
    cout << "All data has been written to songs_updated.txt!!!\n\n";
    SongTable->printStat();
    
    
    //release memory
    delete SongIdTreePtr;
    delete SongTitleTreePtr;
    delete SongTable;

    
}

/*****************************************************************************
 This function reads data about songs from a given file and inserts it
 into a Binary Search Tree.
 *****************************************************************************/
void readData(string filename, BinarySearchTree<Song>* SongIdTreePtr, BinarySearchTree<Song>* SongTitleTreePtr, HashTable<Song>* SongTable)
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
    string id, title, artist, album, genre, duration, year;
    int durationInt, yearInt;
    
    while(getline(inputFile, id, ';'))
    {
        getline(inputFile, title, ';');
        getline(inputFile, artist, ';');
        getline(inputFile, album, ';');
        getline(inputFile, genre, ';');
        getline(inputFile, duration, ';');
        getline(inputFile, year);
        
        durationInt = stoi(duration);
        yearInt = stoi(year);
        
        localSong.setId(id);
        localSong.setTitle(title);
        localSong.setArtist(artist);
        localSong.setAlbum(album);
        localSong.setGenre(genre);
        localSong.setDuration(durationInt);
        localSong.setYear(yearInt);
        
        // insert the Song object into the Binary Search Tree
        SongIdTreePtr->insert(localSong, compareId);
        SongTitleTreePtr->insert(localSong, compareTitle);
        
        //insert into Hash
        SongTable->insertHash(id, localSong);
        
        
        
    }
    inputFile.close();
}


/*****************************************************************************
 This function write the new tree into a file using the writeToFile function
 in HashTable
 *****************************************************************************/
void writeData(string filename, HashTable<Song>* SongTable) {
    SongTable->writeToFile(filename);
}

/**********************************************************************
 This function get the choice user choosen as parameter then perform  *
 the correspond BST operation it will return a bool value             *
 **********************************************************************/
void performChoice(char choice, BinarySearchTree<Song>* SongIdTreePtr, BinarySearchTree<Song>* SongTitleTreePtr, HashTable<Song>* SongTable, Stack<Song>& deleted)
{
    switch (choice)
    {
        case LIST_CHOICE:
            ListManager(SongIdTreePtr , SongTitleTreePtr, SongTable);
            break;
        case SEARCH_CHOICE:
            SearchManager(SongTitleTreePtr, SongTable);
            break;
        case INSERT_CHOICE:
            InsertManager(SongIdTreePtr , SongTitleTreePtr, SongTable);
            break;
        case DELETE_CHOICE:
            DeleteManager(SongIdTreePtr , SongTitleTreePtr, SongTable, deleted);
            break;
        case UNDO_DELETE_CHOICE:
            UndoDelete(deleted, SongIdTreePtr, SongTitleTreePtr, SongTable);
            break;
        case MENU_CHOICE:
            printMenu();
            break;
        default:
            cout << "Invalid option.\n" << endl;
            break;
    }
}

/************************************
 This function will print out       *
 the menu of all the BST operations *
 ************************************/
void printMenu()
{
    cout << "********ONLINE_MUSIC*********\n"
    << "*   L - List all songs      *\n"
    << "*   S - Search a song       *\n"
    << "*   I - Insert a new song   *\n"
    << "*   D - Delete a song       *\n"
    << "*   U - Undo delete         *\n"
    << "*   Q - Quit program        *\n"
    << "*****************************\n";
}

/*****************************************************************************
 This function will shows the SubMenu for the LIST operations and
 perform the list operations(unsorted, ID sort, Title sort, tree list)
 upon user's choice
 *****************************************************************************/
void ListManager(BinarySearchTree<Song>* SongIdTreePtr, BinarySearchTree<Song>* SongTitleTreePtr, HashTable<Song>* SongTable)
{
    cout << "========================================\n"
    << "List\n"
    << "========================================\n";
    
    cout << "U - Unsorted\n"
    << "P - Sorted by ID\n"
    << "S - Sorted by title\n"
    << "T - Print ID and title tree as an indented list\n";
    
    cout << "========================================\n\n";
    
    string choiceString;
    char choice;
    cout << "Enter a method ('Q' to stop listing) (U/P/S/T/Q) : " ;
    cin >> choiceString;
    choice = toupper(choiceString[0]);
    
    while (choice != 'Q')
    {
        
        switch (choice)
        {
            case 'U':
                ListUnsorted(SongTable);
                break;
            case 'P':
                ListById(SongIdTreePtr);
                break;
            case 'S':
                ListByTitle(SongTitleTreePtr);
                break;
            case 'T':
                ListAsIndentedTree(SongIdTreePtr, SongTitleTreePtr);
                break;
            default: cout << "Invalid option!\n";
        }
        
        cout << "\nEnter a method ('Q' to stop listing) (U/P/S/T/Q) : " ;
        cin >> choiceString;
        choice = toupper(choiceString[0]);
        
    }
    
    cout << "==========END LIST SECTION==========\n\n";
}


/*****************************************************************************
 This function print the unsorted data using the HashTable
 *****************************************************************************/
void ListUnsorted(HashTable<Song>* SongTable)
{
    cout << "----------------------------------------\n"
    << "Unsorted\n"
    << "----------------------------------------\n";
    cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
    cout << "ID      Title                   Artist                 Album                          Genre    Duration  Year\n";
    cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
    SongTable->printHash();
    cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
}

/*****************************************************************************
 This function print the song list based on ID using the ID_BST
 *****************************************************************************/

void ListById(BinarySearchTree<Song>* SongIdTreePtr) {
    cout << "----------------------------------------\n"
    << "Sorted by ID\n"
    << "----------------------------------------\n";
    cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
    cout << "ID      Title                   Artist                 Album                          Genre    Duration  Year\n";
    cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
    SongIdTreePtr->inOrder(displaySong);
    cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
}

/*****************************************************************************
 This function print the song list based on Title using the TITLE_BST
 *****************************************************************************/

void ListByTitle(BinarySearchTree<Song>* SongTitleTreePtr) {
    cout << "----------------------------------------\n"
    << "Sorted by title\n"
    << "----------------------------------------\n";
    cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
    cout << "ID      Title                   Artist                 Album                          Genre    Duration  Year\n";
    cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
    SongTitleTreePtr->inOrder(displaySong);
    cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
}


/*****************************************************************************
 This function print the ID_BST and TITLE_BST as an indented list
 *****************************************************************************/
void ListAsIndentedTree(BinarySearchTree<Song>* SongIdTreePtr, BinarySearchTree<Song>* SongTitleTreePtr) {
    cout << "----------------------------------------\n"
    << "Print ID tree as an indented list \n"
    << "----------------------------------------\n";
    SongIdTreePtr->RightRootLeft(displaySongKey,0);
    
    cout << "\n----------------------------------------\n"
    << "Print title tree as an indented list \n"
    << "----------------------------------------\n";
    SongTitleTreePtr->RightRootLeft(displaySongKey,0);
}

/****************************************************************
 This function will allow the user to choose the search function *
 they want to perform, search using ID or title                  *
 ****************************************************************/

void SearchManager(BinarySearchTree<Song>* SongTitleTreePtr, HashTable<Song>* SongTable)
{
    cout << "========================================\n"
         << "Search\n"
         << "========================================\n";
    
    cout << "P - Search by ID \n"
         << "S - Search by title \n";
    
    cout << "========================================\n\n";
    
    string choiceString;
    char choice;
    cout << "Enter a method ('Q' to stop searching) (P/S/Q): " ;
    cin >> choiceString;
    choice = toupper(choiceString[0]);
    
    while (choice != 'Q')
    {
        
        switch (choice)
        {
            case 'P':
                SearchById(SongTable);
                break;
            case 'S':
                SearchByTitle(SongTitleTreePtr);
                break;
            default: cout << "Invalid option!\n";
        }
        
        cout << "\nEnter a method ('Q' to stop searching) (P/S/Q): " ;
        cin >> choiceString;
        choice = toupper(choiceString[0]);
    }
    
    cout << "==========END SEARCH SECTION==========\n\n";
}


/*****************************************************************************
 This function search the target based on ID using the HashTable
 *****************************************************************************/
void SearchById(HashTable<Song>* SongTable)
{
    
    cout << "----------------------------------------\n"
    << "Search by ID\n"
    << "----------------------------------------\n";
    
    string target;
    cout << "Enter the ID: ";
    cin >> target;
    
    if (validateID(target))
    { // if id is valid
        
        Song returnedSong;
        
        if (SongTable->searchHash(target, returnedSong))
        { // if song is found
            cout << "Found:\n";
            cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
            cout << "ID      Title                   Artist                 Album                          Genre    Duration  Year\n";
            cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
            cout << returnedSong << endl;
            cout << "======= ======================= ====================== ============================== ======== ========= =====\n";
        }
        else
        { // if song is not in the table
            cout << "Not found!\n";
        }
    }
    else
    {
        // if id is not valid
        cout << "Invalid ID!\n";
    }
}

/*****************************************************************************
 This function search the target based on Title using the BST
 *****************************************************************************/
void SearchByTitle(BinarySearchTree<Song>* SongTitleTreePtr)
{
    cout << "----------------------------------------\n"
    << "Search by Title\n"
    << "----------------------------------------\n";
    
    string targetTitle;
    cout << "Enter the title: ";
    cin.ignore();
    getline(cin, targetTitle);
    
    Song targetSong;
    targetSong.setTitle(targetTitle);

    /***************/
    bool found = SongTitleTreePtr->getEntry(targetSong, compareTitle, displaySong) ;
    
    if (!found) cout << "***NOT FOUND!!!***\n";

}

/*****************************************************************************
 This function allows the user to insert a new song into the two Binary      *
 Search Trees and the hash table.                                            *
 *****************************************************************************/
void InsertManager(BinarySearchTree<Song>* SongIdTreePtr, BinarySearchTree<Song>* SongTitleTreePtr, HashTable<Song>* SongTable)
{
    Song localSong;
    string id, title, artist, album, genre;
    int duration, year;
    //bool inserted = false;
    
    cout << "Enter the ID of the song you want to add: ";
    cin.ignore();
    getline (cin, id);
    
    if (validateID(id)) {
    
        if ( ! SongTable->searchHash(id, localSong)) {
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
            cin >> year;
            
            localSong.setId(id);
            localSong.setTitle(title);
            localSong.setArtist(artist);
            localSong.setAlbum(album);
            localSong.setGenre(genre);
            localSong.setDuration(duration);
            localSong.setYear(year);
            
            SongIdTreePtr->insert(localSong,compareId);
            SongTitleTreePtr->insert(localSong,compareTitle);
            SongTable->insertHash(id,localSong);
            
            cout << "The Song has been inserted!!\n";
        }

        else
        {
            cout << "The song cannot be inserted." << endl;
        }
        
    }
    
    else cout << "Invalid ID!!!\n";
    
    cout << "==========END INSERT SECTION==========\n\n";
    
}
    
/*****************************************************************************
This function allows the user to delete a song from the two Binary          *
Search Trees and the hash table using the id of the song.                   *
*****************************************************************************/
void DeleteManager (BinarySearchTree<Song>* IdTree, BinarySearchTree<Song>* TitleTree, HashTable<Song>* songHash, Stack<Song>& deleted )
{
        string id;
    
        Song Entry, returnedItem;
        
        cout << "Enter the ID of the song you want to delete: ";
        cin.ignore();
        cin >> id;
    
        
        Entry.setId (id);
        
        bool removed = IdTree->remove(Entry);

        if ( removed ){
            removed = TitleTree->remove(Entry);
            removed = songHash->deleteHash(Entry.getId(), Entry);
            cout << "The song is deleted successfully!" << endl << endl;
            
            deleted.push(Entry);
            
        }
        else{
            cout << "The song is not found." << endl ;
        }
}
    

    
/*****************************************************************************
This function allows the user to delete a song from the two Binary          *
Search Trees and the hash table using the id of the song.                   *
*****************************************************************************/
    
void UndoDelete(Stack<Song>& deleted ,BinarySearchTree<Song>* IdTree, BinarySearchTree<Song>* TitleTree, HashTable<Song>* SongTable)
{
    
    if (deleted.isEmpty()) {
        cout << "No Song has been deleted yet! \n\n";
        return;
    }
    
    Song temp;
    deleted.pop(temp);
    
    // insert the Song object into the Binary Search Tree
    IdTree->insert(temp, compareId);
    TitleTree->insert(temp, compareTitle);
    
    //insert into Hash
    SongTable->insertHash(temp.getId(), temp);

    cout << temp.getId() << " " << temp.getTitle() << " is being put back into the database!\n\n";
    
}
    
    
/*****************************************************************************
 This function will check if the ID given by the user is only number         *
 not alphabet.                                                               *
 *****************************************************************************/
bool validateID(string id_input)
{
    int i = 0;
    Song tempSong;
    
    if (id_input.length()!=4)
        return false;
    
    while( i < id_input.length())
    {
        if(isalpha(id_input[i]))
        {
            // cout << "INVALID INPUT! PLEASE ENTER NUMBER ONLY! PLEASE ENTER AGAIN!\n" << endl;
            return false;
        }
            i++;
    }
    
    return true;
}


//This funciton will print the information of the song
void displaySong(Song& aSong)
{
    cout << aSong << endl;
}

//This funciton will print the ID and Title of the song
void displaySongKey(Song& aSong) {
    cout << aSong.getId() << " " << aSong.getTitle() << endl;
}


/*This funciton will compares the ID of two songs
 and returns an integer value :
 1 : when left > right
 -1: when left < right
 0 : when left = right
 */
int compareId(const Song& left, const Song& right )
{
    if ( left.getId() > right.getId())
        return 1;
    else if ( left.getId() < right.getId() )
        return -1;
    else
        return 0;
    
}

/*This funciton will compares the TITLE of two songs
and returns an integer value :
1 : when left > right
-1: when left < right
0 : when left = right
*/
int compareTitle(const Song& left, const Song& right )
{
    if ( left.getTitle() > right.getTitle())
        return 1;
    else if (  left.getTitle() < right.getTitle() )
        return -1;
    else
        return 0;
    
}

////////////////
//TEST RUN #1 //
////////////////
/*
 ********ONLINE_MUSIC*********
 *   L - List all songs      *
 *   S - Search a song       *
 *   I - Insert a new song   *
 *   D - Delete a song       *
 *   U - Undo delete         *
 *   Q - Quit program        *
 *****************************
 Enter your choice (L/S/I/D/U/M/Q): L
 ========================================
 List
 ========================================
 U - Unsorted
 P - Sorted by ID
 S - Sorted by title
 T - Print ID and title tree as an indented list
 ========================================
 
 Enter a method ('Q' to stop listing) (U/P/S/T/Q) : U
 ----------------------------------------
 Unsorted
 ----------------------------------------
 ======= ======================= ====================== ============================== ======== ========= =====
 ID      Title                   Artist                 Album                          Genre    Duration  Year
 ======= ======================= ====================== ============================== ======== ========= =====
 3021    Shake It Off            Taylor Swift           1989                           Pop      3:39      2014
 2127    Happy                   Pharrel Williams       Happy                          Soul     3:57      2013
 1708    Payphone                Maroon 5               Overexposed                    Pop      4:39      2012
 1234    Attention               Charlie Puth           Voicenotes                     Funk     3:41      2017
 1024    Hello                   Adele                  25                             Soul     4:56      2015
 8711    Stitches                Shawn Mendes           Handwritten                    Pop      3:30      2015
 9083    Gangnam Style           PSY                    Psy6 (Six Rules)               K-pop    4:12      2012
 7604    Starships               Nicki Minaj            Pink Friday: Roman Reloaded    Hip-Hop  4:22      2012
 6810    Kanzen Kankaku Dreamer  ONE OK ROCK            Niche Syndrome                 Rock     4:12      2010
 5327    Dark Horse              Katy Perry             Prism                          Pop      3:45      2013
 9201    God Is a Woman          Ariana Grande          Sweetener                      Pop      3:17      2018
 1040    Shape of You            Ed Sheeran             Shape of You                   Pop      3:53      2017
 2047    No Tears Left to Cry    Ariana Grande          Sweetener                      Disco    3:26      2018
 1999    Wherever You Are        5s of Summer           She Looks So Perfect           Pop      3:08      2014
 7724    The Hills               The Weeknd             Beauty Behind the Madness      R&B      4:08      2015
 3114    Sorry                   Justin Bieber          Purpose                        R&B      3:20      2015
 7301    24K Magic               Bruno Mars             24K Magic                      R&B      3:46      2016
 5502    We Don't Talk Anymore   Charlie Puth           Nine Track Mind                Pop      3:37      2016
 3013    TT                      Twice                  Twicecoaster:Lane 1            K-pop    3:38      2016
 4819    I Really Like You       Carly Rae Jepsen       Emotion                        Pop      3:24      2015
 6515    Uptown Funk             Mark Ronson            Uptown Funk                    Funk     4:30      2014
 8423    Bad Blood               Taylor Swift           1989                           Pop      3:31      2015
 3124    Wherever You Are        ONE OK ROCK            Niche Syndrome                 Rock     4:57      2010
 2012    Blank Space             Taylor Swift           1989                           Pop      3:51      2014
 8002    Closer                  The Chainsmokers       Collage                        Dance    4:22      2016
 ======= ======================= ====================== ============================== ======== ========= =====
 
 Enter a method ('Q' to stop listing) (U/P/S/T/Q) : P
 ----------------------------------------
 Sorted by ID
 ----------------------------------------
 ======= ======================= ====================== ============================== ======== ========= =====
 ID      Title                   Artist                 Album                          Genre    Duration  Year
 ======= ======================= ====================== ============================== ======== ========= =====
 1024    Hello                   Adele                  25                             Soul     4:56      2015
 1040    Shape of You            Ed Sheeran             Shape of You                   Pop      3:53      2017
 1234    Attention               Charlie Puth           Voicenotes                     Funk     3:41      2017
 1708    Payphone                Maroon 5               Overexposed                    Pop      4:39      2012
 1999    Wherever You Are        5s of Summer           She Looks So Perfect           Pop      3:08      2014
 2012    Blank Space             Taylor Swift           1989                           Pop      3:51      2014
 2047    No Tears Left to Cry    Ariana Grande          Sweetener                      Disco    3:26      2018
 2127    Happy                   Pharrel Williams       Happy                          Soul     3:57      2013
 3013    TT                      Twice                  Twicecoaster:Lane 1            K-pop    3:38      2016
 3021    Shake It Off            Taylor Swift           1989                           Pop      3:39      2014
 3114    Sorry                   Justin Bieber          Purpose                        R&B      3:20      2015
 3124    Wherever You Are        ONE OK ROCK            Niche Syndrome                 Rock     4:57      2010
 4819    I Really Like You       Carly Rae Jepsen       Emotion                        Pop      3:24      2015
 5327    Dark Horse              Katy Perry             Prism                          Pop      3:45      2013
 5502    We Don't Talk Anymore   Charlie Puth           Nine Track Mind                Pop      3:37      2016
 6515    Uptown Funk             Mark Ronson            Uptown Funk                    Funk     4:30      2014
 6810    Kanzen Kankaku Dreamer  ONE OK ROCK            Niche Syndrome                 Rock     4:12      2010
 7301    24K Magic               Bruno Mars             24K Magic                      R&B      3:46      2016
 7604    Starships               Nicki Minaj            Pink Friday: Roman Reloaded    Hip-Hop  4:22      2012
 7724    The Hills               The Weeknd             Beauty Behind the Madness      R&B      4:08      2015
 8002    Closer                  The Chainsmokers       Collage                        Dance    4:22      2016
 8423    Bad Blood               Taylor Swift           1989                           Pop      3:31      2015
 8711    Stitches                Shawn Mendes           Handwritten                    Pop      3:30      2015
 9083    Gangnam Style           PSY                    Psy6 (Six Rules)               K-pop    4:12      2012
 9201    God Is a Woman          Ariana Grande          Sweetener                      Pop      3:17      2018
 ======= ======================= ====================== ============================== ======== ========= =====
 
 Enter a method ('Q' to stop listing) (U/P/S/T/Q) : S
 ----------------------------------------
 Sorted by title
 ----------------------------------------
 ======= ======================= ====================== ============================== ======== ========= =====
 ID      Title                   Artist                 Album                          Genre    Duration  Year
 ======= ======================= ====================== ============================== ======== ========= =====
 7301    24K Magic               Bruno Mars             24K Magic                      R&B      3:46      2016
 1234    Attention               Charlie Puth           Voicenotes                     Funk     3:41      2017
 8423    Bad Blood               Taylor Swift           1989                           Pop      3:31      2015
 2012    Blank Space             Taylor Swift           1989                           Pop      3:51      2014
 8002    Closer                  The Chainsmokers       Collage                        Dance    4:22      2016
 5327    Dark Horse              Katy Perry             Prism                          Pop      3:45      2013
 9083    Gangnam Style           PSY                    Psy6 (Six Rules)               K-pop    4:12      2012
 9201    God Is a Woman          Ariana Grande          Sweetener                      Pop      3:17      2018
 2127    Happy                   Pharrel Williams       Happy                          Soul     3:57      2013
 1024    Hello                   Adele                  25                             Soul     4:56      2015
 4819    I Really Like You       Carly Rae Jepsen       Emotion                        Pop      3:24      2015
 6810    Kanzen Kankaku Dreamer  ONE OK ROCK            Niche Syndrome                 Rock     4:12      2010
 2047    No Tears Left to Cry    Ariana Grande          Sweetener                      Disco    3:26      2018
 1708    Payphone                Maroon 5               Overexposed                    Pop      4:39      2012
 3021    Shake It Off            Taylor Swift           1989                           Pop      3:39      2014
 1040    Shape of You            Ed Sheeran             Shape of You                   Pop      3:53      2017
 3114    Sorry                   Justin Bieber          Purpose                        R&B      3:20      2015
 7604    Starships               Nicki Minaj            Pink Friday: Roman Reloaded    Hip-Hop  4:22      2012
 8711    Stitches                Shawn Mendes           Handwritten                    Pop      3:30      2015
 3013    TT                      Twice                  Twicecoaster:Lane 1            K-pop    3:38      2016
 7724    The Hills               The Weeknd             Beauty Behind the Madness      R&B      4:08      2015
 6515    Uptown Funk             Mark Ronson            Uptown Funk                    Funk     4:30      2014
 5502    We Don't Talk Anymore   Charlie Puth           Nine Track Mind                Pop      3:37      2016
 1999    Wherever You Are        5s of Summer           She Looks So Perfect           Pop      3:08      2014
 3124    Wherever You Are        ONE OK ROCK            Niche Syndrome                 Rock     4:57      2010
 ======= ======================= ====================== ============================== ======== ========= =====
 
 Enter a method ('Q' to stop listing) (U/P/S/T/Q) : T
 ----------------------------------------
 Print ID tree as an indented list
 ----------------------------------------
 
 (Level 3)  9201 God Is a Woman
 
 (Level 2)  9083 Gangnam Style
 
 (Level 3)  8711 Stitches
 
 (Level 4)  8423 Bad Blood
 
 (Level 1)  8002 Closer
 
 (Level 4)  7724 The Hills
 
 (Level 3)  7604 Starships
 
 (Level 2)  7301 24K Magic
 
 (Level 4)  6810 Kanzen Kankaku Dreamer
 
 (Level 5)  6515 Uptown Funk
 
 (Level 3)  5502 We Don't Talk Anymore
 
 (Level 4)  5327 Dark Horse
 
 (Level 0)  4819 I Really Like You
 
 (Level 4)  3124 Wherever You Are
 
 (Level 3)  3114 Sorry
 
 (Level 4)  3021 Shake It Off
 
 (Level 2)  3013 TT
 
 (Level 3)  2127 Happy
 
 (Level 1)  2047 No Tears Left to Cry
 
 (Level 4)  2012 Blank Space
 
 (Level 3)  1999 Wherever You Are
 
 (Level 2)  1708 Payphone
 
 (Level 3)  1234 Attention
 
 (Level 5)  1040 Shape of You
 
 (Level 4)  1024 Hello
 
 ----------------------------------------
 Print title tree as an indented list
 ----------------------------------------
 
 (Level 6)  3124 Wherever You Are
 
 (Level 5)  1999 Wherever You Are
 
 (Level 4)  5502 We Don't Talk Anymore
 
 (Level 6)  6515 Uptown Funk
 
 (Level 7)  7724 The Hills
 
 (Level 5)  3013 TT
 
 (Level 6)  8711 Stitches
 
 (Level 3)  7604 Starships
 
 (Level 4)  3114 Sorry
 
 (Level 5)  1040 Shape of You
 
 (Level 6)  3021 Shake It Off
 
 (Level 2)  1708 Payphone
 
 (Level 1)  2047 No Tears Left to Cry
 
 (Level 2)  6810 Kanzen Kankaku Dreamer
 
 (Level 0)  4819 I Really Like You
 
 (Level 3)  1024 Hello
 
 (Level 5)  2127 Happy
 
 (Level 4)  9201 God Is a Woman
 
 (Level 2)  9083 Gangnam Style
 
 (Level 3)  5327 Dark Horse
 
 (Level 1)  8002 Closer
 
 (Level 4)  2012 Blank Space
 
 (Level 5)  8423 Bad Blood
 
 (Level 3)  1234 Attention
 
 (Level 2)  7301 24K Magic
 
 Enter a method ('Q' to stop listing) (U/P/S/T/Q) : Q
 ==========END LIST SECTION==========
 
 Enter your choice ('M' to show menu) (L/S/I/D/U/M/Q): S
 ========================================
 Search
 ========================================
 P - Search by ID
 S - Search by title
 ========================================
 
 Enter a method ('Q' to stop searching) (P/S/Q): P
 ----------------------------------------
 Search by ID
 ----------------------------------------
 Enter the ID: abcd
 Invalid ID!
 
 Enter a method ('Q' to stop searching) (P/S/Q): P
 ----------------------------------------
 Search by ID
 ----------------------------------------
 Enter the ID: 9999
 9999 Not found!
 
 Enter a method ('Q' to stop searching) (P/S/Q): P
 ----------------------------------------
 Search by ID
 ----------------------------------------
 Enter the ID: 1234
 1234 Found:
 ======= ======================= ====================== ============================== ======== ========= =====
 ID      Title                   Artist                 Album                          Genre    Duration  Year
 ======= ======================= ====================== ============================== ======== ========= =====
 1234    Attention               Charlie Puth           Voicenotes                     Funk     3:41      2017
 ======= ======================= ====================== ============================== ======== ========= =====
 
 Enter a method ('Q' to stop searching) (P/S/Q): S
 ----------------------------------------
 Search by Title
 ----------------------------------------
 Enter the title: Not Exist
 ***NOT FOUND!!!***
 
 Enter a method ('Q' to stop searching) (P/S/Q): S
 ----------------------------------------
 Search by Title
 ----------------------------------------
 Enter the title: Attention
 ======= ======================= ====================== ============================== ======== ========= =====
 ID      Title                   Artist                 Album                          Genre    Duration  Year
 ======= ======================= ====================== ============================== ======== ========= =====
 1234    Attention               Charlie Puth           Voicenotes                     Funk     3:41      2017
 ======= ======================= ====================== ============================== ======== ========= =====
 
 Enter a method ('Q' to stop searching) (P/S/Q): S
 ----------------------------------------
 Search by Title
 ----------------------------------------
 Enter the title: Wherever You Are
 ======= ======================= ====================== ============================== ======== ========= =====
 ID      Title                   Artist                 Album                          Genre    Duration  Year
 ======= ======================= ====================== ============================== ======== ========= =====
 1999    Wherever You Are        5s of Summer           She Looks So Perfect           Pop      3:08      2014
 3124    Wherever You Are        ONE OK ROCK            Niche Syndrome                 Rock     4:57      2010
 ======= ======================= ====================== ============================== ======== ========= =====
 
 Enter a method ('Q' to stop searching) (P/S/Q): Q
 ==========END SEARCH SECTION==========
 
 Enter your choice ('M' to show menu) (L/S/I/D/U/M/Q): I
 Enter the ID of the song you want to add: 1234
 1234 The song cannot be inserted.
 ==========END INSERT SECTION==========
 
 Enter your choice ('M' to show menu) (L/S/I/D/U/M/Q): I
 Enter the ID of the song you want to add: abcd
 Invalid ID!!!
 ==========END INSERT SECTION==========
 
 Enter your choice ('M' to show menu) (L/S/I/D/U/M/Q): I
 Enter the ID of the song you want to add: 9999
 9999 Enter the title of the song you want to add: temp
 Enter the artist of the song you want to add: temp
 Enter the album of the song you want to add: temp
 Enter the genre of the song you want to add: temp
 Enter the duration of the song you want to add: 0
 Enter the year of the song you want to add: 2000
 The Song has been inserted!!
 ==========END INSERT SECTION==========
 
 Enter your choice ('M' to show menu) (L/S/I/D/U/M/Q): D
 Enter the ID of the song you want to delete: 4819
 4819 The song is deleted successfully!
 
 Enter your choice ('M' to show menu) (L/S/I/D/U/M/Q): D
 Enter the ID of the song you want to delete: 1234
 1234 The song is deleted successfully!
 
 Enter your choice ('M' to show menu) (L/S/I/D/U/M/Q): U

 1234 Attention is being put back into the database!
 
 Enter your choice ('M' to show menu) (L/S/I/D/U/M/Q): M
 ********ONLINE_MUSIC*********
 *   L - List all songs      *
 *   S - Search a song       *
 *   I - Insert a new song   *
 *   D - Delete a song       *
 *   U - Undo delete         *
 *   Q - Quit program        *
 *****************************
 Enter your choice ('M' to show menu) (L/S/I/D/U/M/Q): Q
 All data has been written to songs_updated.txt!!!
 
 ========================================
 Statistic of Hash
 ========================================
 Load Factor:         41.5094%
 Number of Collision: 5
 ========================================
 
 Program ended with exit code: 0
 */

////////////////
//TEST RUN #2 //
////////////////
//if size of hashtable is changed to 10
/*
 >>>>>>>>>>>>>>>rehashing...from size 10 to 23<<<<<<<<<<<<<<<
 
 ********ONLINE_MUSIC*********
 *   L - List all songs      *
 *   S - Search a song       *
 *   I - Insert a new song   *
 *   D - Delete a song       *
 *   U - Undo delete         *
 *   Q - Quit program        *
 *****************************
 Enter your choice (L/S/I/D/U/M/Q): Q
 All data has been written to songs_updated.txt!!!
 
 ========================================
 Statistic of Hash
 ========================================
 Load Factor:         65.2174%
 Number of Collision: 10
 ========================================
 
 Program ended with exit code: 0
 */
