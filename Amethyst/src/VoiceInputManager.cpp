#include "VoiceInputManager.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm> 

using namespace std;

string VoiceInputManager::listen() {
    string cmd = "python \"D:\\Amethyst\\src\\voice_to_text.py\"";
    string result;
#ifdef _WIN32
    FILE* pipe = _popen(cmd.c_str(), "r");
#else
    FILE* pipe = popen(cmd.c_str(), "r");
#endif
    if (!pipe) {
        cerr << "Error running Python voice script.\n";
        return "";
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        result += buffer;
    }
#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif
    result.erase(
        std::remove(result.begin(), result.end(), '\n'),
        result.end()
    );

    return result;
}
