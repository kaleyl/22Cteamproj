/**~*~*
 Song header file
 *~**/


#ifndef Song_h
#define Song_h

#include <string>

using namespace std;

class Song
{
    private:
        string id;
        int year;
        int duration;    // in seconds
        string title;
        string artist;
        string album;
        string genre;
    
    public:
        
        // Constructor
        Song() {
            id = "";
            title = "";
            artist = "";
            album = "";
            genre = "";
            duration = -1;
            year = -1;
        }
         
    
    // Setters and getters
    void setId(string i) {id = i;}
    void setTitle(string t) {title = t;}
    void setArtist(string art) {artist = art;}
    void setAlbum(string alb) {album = alb;}
    void setGenre(string g) {genre = g;}
    void setDuration(int d) {duration = d;}
    void setYear(int y) {year = y;}
    
    string getId() const {return id;}
    string getTitle() const {return title;}
    string getArtist() const {return artist;}
    string getAlbum() const {return album;}
    string getGenre() const {return genre;}
    int getDuration() const {return duration;}
    int getYear() const {return year;}
    
    // Overloaded operator functions
    bool operator > (const Song &);     // Overloaded >
    bool operator < (const Song &);     // Overloaded <
    bool operator == (const Song &);    // Overloaded ==
    
    // Friends
    friend ostream &operator << (ostream &, const Song &);
};

#endif /* songs_h */
