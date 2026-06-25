#pragma once
#include <string>

class ReminderManager {
private:
    std::string remindersFile;

    bool isDue(const std::string& datetime) const;

public:
    ReminderManager(const std::string& filename = "reminders.txt");

    void addReminder(const std::string& datetime, const std::string& message);
    void checkReminders();
};
