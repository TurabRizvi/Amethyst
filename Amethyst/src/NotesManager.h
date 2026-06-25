#pragma once
#include <string>

class NotesManager {
private:
    std::string notesFile;

public:
    NotesManager(const std::string& filename = "notes.txt"); 

    void addNote(const std::string& note);   
    void showNotes() const;                 
};
