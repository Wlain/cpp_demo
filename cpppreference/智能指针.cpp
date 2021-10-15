//
// Created by william on 2021/1/8.
//


#include <string>
#include <memory>

class Song
{
public:
    Song(const char* name):m_name(name)
    {

    }
    ~Song() = default;
private:
    std::string m_name;
};

void useRawPointer()
{
    // Using a raw pointer -- not recommended.
    Song* song = new Song("xiaowang");
    delete song;
}


void useSmartPointer()
{
    // Declare a smart pointer on stack and pass it the raw pointer.
    std::unique_ptr<Song> song(new Song("lisi"));
    song.reset(); // Free the memory before we exit function block.
}
