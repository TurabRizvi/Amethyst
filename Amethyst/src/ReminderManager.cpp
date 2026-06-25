#include "ReminderManager.h"
#include <fstream>   
#include <iostream> 
#include <sstream> 
#include <vector> 
#include <ctime> 
#include <windows.h> 
struct Reminder {
    std::string datetime;
    std::string message;
};

ReminderManager::ReminderManager(const std::string& filename)
    : remindersFile(filename) {}

void ReminderManager::addReminder(const std::string& datetime, const std::string& message) {
    std::ofstream fout(remindersFile, std::ios::app);
    if (fout.is_open()) {
        fout << datetime << " | " << message << std::endl;
        fout.close();
        std::cout << "Reminder added successfully!\n";
    } else {
        std::cerr << "Failed to open reminders file!\n";
    }
}

bool ReminderManager::isDue(const std::string& datetime) const {
    int year, month, day, hour, minute;
    char sep1, sep2, sep3;

    std::istringstream ss(datetime);
    ss >> year >> sep1 >> month >> sep2 >> day >> hour >> sep3 >> minute;

    if (ss.fail() || sep1 != '-' || sep2 != '-' || sep3 != ':' ) {
        std::cerr << "Invalid datetime format: " << datetime << std::endl;
        return false;
    }

    std::tm tm_reminder = {};
    tm_reminder.tm_year = year - 1900;
    tm_reminder.tm_mon  = month - 1;
    tm_reminder.tm_mday = day;
    tm_reminder.tm_hour = hour;
    tm_reminder.tm_min  = minute;
    tm_reminder.tm_sec  = 0;

    std::time_t reminder_time = std::mktime(&tm_reminder);
    if (reminder_time == -1) return false;

    std::time_t now = std::time(nullptr);
    return difftime(now, reminder_time) >= 0;
}

void ReminderManager::checkReminders() {
    std::ifstream fin(remindersFile);
    if (!fin.is_open()) {
        std::cerr << "Failed to open reminders file!\n";
        return;
    }

    std::vector<Reminder> remaining;
    std::string line;
    while (std::getline(fin, line)) {
        size_t sep = line.find(" | ");
        if (sep == std::string::npos) continue;

        Reminder r;
        r.datetime = line.substr(0, sep);
        r.message = line.substr(sep + 3);

        if (isDue(r.datetime)) {
            MessageBoxA(NULL, r.message.c_str(), "Amethyst Reminder", MB_OK | MB_ICONINFORMATION);
        } else {
            remaining.push_back(r);
        }
    }
    fin.close();

    std::ofstream fout(remindersFile, std::ios::trunc);
    for (const auto& r : remaining) {
        fout << r.datetime << " | " << r.message << std::endl;
    }
}