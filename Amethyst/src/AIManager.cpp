#include "AIManager.h"
#include <cstdio>
#include <iostream>

using namespace std;

string AIManager::askAI(const string& userInput) {

    string safeInput = userInput;
    size_t pos = 0;
    while ((pos = safeInput.find('"', pos)) != string::npos) {
        safeInput.replace(pos, 1, "\\\"");
        pos += 2;
    }

    string command = "python ..\\ai\\ai_bridge.py \"" + safeInput + "\"";

#ifdef _WIN32
    FILE* pipe = _popen(command.c_str(), "r");
#else
    FILE* pipe = popen(command.c_str(), "r");
#endif

    if (!pipe) {
        return "AI Error: Could not start AI process.";
    }

    char buffer[256];
    string result;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

#ifdef _WIN32
    int exitCode = _pclose(pipe);
#else
    int exitCode = pclose(pipe);
#endif

    if (exitCode != 0) {
        return "AI Error: AI process failed.";
    }

    result += "<END_OF_RESPONSE>\n";

    return result;
}
