#include "NotesManager.h"
#include <fstream>
#include <iostream>
#include <cstdlib> 

NotesManager::NotesManager(const std::string& filename)
    : notesFile(filename) {}


void NotesManager::addNote(const std::string& note) {
    std::ofstream fout(notesFile, std::ios::app); 
    if (fout.is_open()) {
        fout << note << std::endl;
        fout.close();


        std::string cmd = "notepad " + notesFile;
        system(cmd.c_str());
    } else {
        std::cerr << "Failed to open notes file!\n";
    }
}

void NotesManager::showNotes() const {
    std::string cmd = "notepad " + notesFile;
    system(cmd.c_str());
}
