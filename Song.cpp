//
//  Song.cpp
//

#include "Song.h"
#include <iostream>
#include <iomanip>
using namespace std;

/*This funciton overloads the cout<< operators to
 print out all the information of one song*/
ostream & operator <<(ostream & strm, const Song & obj)
{
    int min = obj.getDuration()/60;
    int sec = obj.getDuration()%60;
    
    strm << setw(8) << left << obj.getId()
    << setw(24) << left << obj.getTitle()
    << setw(23) << left << obj.getArtist()
    << setw(31) << left << obj.getAlbum()
    << setw(9) << left << obj.getGenre()
    << setw(1) << left << min << ":";
    
    if (sec < 10) cout << 0 << setw(7) << left << sec;
    else cout << setw(8) << left << sec;
    
    cout << setw(5) << left << obj.getYear();
    return strm;
}


/*This funciton overloads the > operators to
 to compare the id of songs*/
bool Song :: operator > (const Song & right)
{
    return id > right.id;
}

/*This funciton overloads the < operators to
 to compare the id of songs*/
bool Song :: operator < (const Song & right)
{
    
    return id < right.id;
}

/*This funciton overloads the == operators to
 to compare the id of songs*/
bool Song :: operator == (const Song & right)
{
    return id == right.id;
}

