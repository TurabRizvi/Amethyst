#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <string>
#include "ReminderManager.h"
#include "AIManager.h"
#include "SystemCommandManager.h"
#include "NotesManager.h"
#include "VoiceInputManager.h"

class CommandProcessor {
private:
    ReminderManager reminderManager;
    AIManager aiManager;
    SystemCommandManager systemCommandManager;
    NotesManager notesManager;
    VoiceInputManager voice;

public:
    CommandProcessor();

    void process(const std::string& input);
    void processVoice();
    void checkReminders() { reminderManager.checkReminders(); }
};

#endif
