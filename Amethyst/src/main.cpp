#include "CommandProcessor.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    CommandProcessor processor;

    cout << "Amethyst is running...\n\n";

    string input;

    while (true) {
        cout << "You: ";
        getline(cin, input);

        if (input.empty()) continue;

        if (input == "exit" || input == "quit") {
            cout << "Exiting Amethyst...\n";
            break;
        }
        processor.process(input);
        processor.checkReminders();
    }

    return 0;
}
