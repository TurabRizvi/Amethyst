#include "CommandProcessor.h"
#include <iostream>
#include <algorithm>

using namespace std;

CommandProcessor::CommandProcessor()
    : reminderManager("../data/reminders.txt") {
}

void CommandProcessor::process(const string& input) {
    if (input.empty()) return;

    string cmd = input;
    transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (cmd == "voice") {
        processVoice();
        return;
    }

    if (cmd.rfind("add note ", 0) == 0) {
        string note = input.substr(9);
        notesManager.addNote(note);
        cout << "Note added successfully!\n";
        cout.flush();
    }
    else if (cmd.rfind("show my notes", 0) == 0) {
        notesManager.showNotes();
        cout.flush();
    }

    else if (cmd.rfind("add reminder ", 0) == 0) {
        string remainder = input.substr(13);
        size_t sep = remainder.find(" | ");
        if (sep != string::npos) {
            string datetime = remainder.substr(0, sep);
            string message = remainder.substr(sep + 3);
            reminderManager.addReminder(datetime, message);
            cout << "Reminder added!\n";
            cout.flush();
        } else {
            cout << "Invalid format! Use: add reminder YYYY-MM-DD HH:MM | message\n";
            cout.flush();
        }
    }
    else if (cmd.rfind("check reminders", 0) == 0) {
        reminderManager.checkReminders();
        cout.flush();
    }

    else if (cmd == "open calculator") systemCommandManager.openCalculator();
    else if (cmd == "open browser") systemCommandManager.openBrowser();
    else if (cmd == "open notepad") systemCommandManager.openNotepad();
    else if (cmd == "open command prompt") systemCommandManager.openCommandPrompt();
    else if (cmd == "open powershell") systemCommandManager.openPowerShell();
    else if (cmd == "open file explorer") systemCommandManager.openFileExplorer();
    else if (cmd == "open control panel") systemCommandManager.openControlPanel();
    else if (cmd == "open task manager") systemCommandManager.openTaskManager();
    else if (cmd == "open paint") systemCommandManager.openPaint();
    else if (cmd == "open settings") systemCommandManager.openSettings();
    else if (cmd == "open vscode") systemCommandManager.openVSCode();
    else if (cmd == "open devcpp") systemCommandManager.openDevCpp();
    else if (cmd == "open cursor") systemCommandManager.openCursor();
    else if (cmd == "open word") systemCommandManager.openWord();
    else if (cmd == "open powerpoint") systemCommandManager.openPowerPoint();
    else if (cmd == "open excel") systemCommandManager.openExcel();
    else if (cmd == "open linkedin") systemCommandManager.openLinkedIn();
    else if (cmd == "open spotify") systemCommandManager.openSpotify();
    else if (cmd == "open chatgpt") systemCommandManager.openChatGPT();

    else {
        string reply = aiManager.askAI(input);
        cout << "AI: " << reply << endl;
        cout.flush();
    }
}

void CommandProcessor::processVoice() {
    cout << "Speak now...\n";
    cout.flush();

    string spokenText = voice.listen();
    if (spokenText.empty()) {
        cout << "AI: I didn't catch that.\n";
        cout.flush();
        return;
    }

    process(spokenText);
}
