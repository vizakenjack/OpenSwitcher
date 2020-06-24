#include <sys/stat.h>
#include "switchkeyboard.h"
#include <sstream>
#include "utilities.h"
#include <ApplicationServices/ApplicationServices.h>
#include <wchar.h>
#include <QDebug>

#include <string>
#include <locale>
#include <codecvt>
#include <cassert> 

#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>


std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

inline void Press(int key, bool shiftDown,bool commandDown) {
    // Create an HID hardware event source
    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);

    // Create a new keyboard key press event
    CGEventRef evt = CGEventCreateKeyboardEvent(src, (CGKeyCode) key, true);

    //CGEventSetFlags(evt, kCGEventFlagMaskNonCoalesced | CGEventGetFlags(evt));

    if(commandDown)
        CGEventSetFlags(evt, (kCGEventFlagMaskCommand  | CGEventGetFlags(evt)));
    if(shiftDown)
        CGEventSetFlags(evt, (kCGEventFlagMaskShift  | CGEventGetFlags(evt)));
    else CGEventSetFlags(evt, (CGEventGetFlags(evt)) & ~kCGEventFlagMaskShift);
    // Post keyboard event and release
    CGEventPost (kCGHIDEventTap, evt);
    CFRelease (evt); CFRelease (src);

    usleep(100);
}

inline void Release(int key, bool shiftDown,bool commandDown) {
    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
    CGEventRef evt = CGEventCreateKeyboardEvent(src, (CGKeyCode) key, false);

    if(commandDown)
        CGEventSetFlags(evt, (kCGEventFlagMaskCommand  | CGEventGetFlags(evt)));
    if(shiftDown)
        CGEventSetFlags(evt, (kCGEventFlagMaskShift  | CGEventGetFlags(evt)));
    else CGEventSetFlags(evt, (CGEventGetFlags(evt)) & ~kCGEventFlagMaskShift);
    // Post keyboard event and release
    CGEventPost (kCGHIDEventTap, evt);
    CFRelease (evt); CFRelease (src);

    usleep(100);
}

void Click(int key, bool shiftDown,bool commandDown){
    Press(key,shiftDown,commandDown);
    Release(key,shiftDown,commandDown);
}

bool doseFileExist(const char* fileName) {
    struct stat buffer;
    return (stat (fileName, &buffer) == 0);
}

void switchKeyboard(const char* nameToSwitch){
    const char * argv[3];
    argv[1] = "-se";
    argv[2] = nameToSwitch;
    xkbswitch(3, argv);
}

void saveSettings(int hotkey,std::string location){
    FILE* saveFile = fopen(location.c_str(),"w");
    fwrite(&hotkey,sizeof(int),1,saveFile);
    fclose(saveFile);
}

int loadSettings(std::string location){
    int returnVal;
    FILE* loadFile = fopen(location.c_str(),"r");
    fread(&returnVal,sizeof(int),1,loadFile);
    fclose(loadFile);
    return returnVal;
}

bool isInvalidTypeingKey(int keycode){
    switch(keycode){
        case(0x33):
            return false;
        case(0x35):
            return false;
        case(0x37):
            return false;
        case(0x38):
            return false;
        case(0x39):
            return false;
        case(0x3A):
            return false;
        case(0x3B):
            return false;
        case(0x3C):
            return false;
        case(0x3D):
            return false;
        case(0x3E):
            return false;
        case(0x3F):
            return false;
        case(0x40):
            return false;
        case(0x48):
            return false;
        case(0x49):
            return false;
        case(0x4A):
            return false;
        case(0x4F):
            return false;
        case(0x50):
            return false;
        case(0x5A):
            return false;
        default:
            return true;
    }
}

std::string getKeyTextFromCode (int keycode){
    switch (keycode) {
    case(0x00):
        return "A";
    case(0x01):
        return "S";
    case(0x02):
        return "D";
    case(0x03):
        return "F";
    case(0x04):
        return "H";
    case(0x05):
        return "G";
    case(0x06):
        return "Z";
    case(0x07):
        return "X";
    case(0x08):
        return "C";
    case(0x09):
        return "V";
    case(0x0B):
        return "B";
    case(0x0C):
        return "Q";
    case(0x0D):
        return "W";
    case(0x0E):
        return "E";
    case(0x0F):
        return "R";
    case(0x10):
        return "Y";
    case(0x11):
        return "T";
    case(0x12):
        return "1";
    case(0x13):
        return "2";
    case(0x14):
        return "3";
    case(0x15):
        return "4";
    case(0x16):
        return "6";
    case(0x17):
        return "5";
    case(0x18):
        return "Equal";
    case(0x19):
        return "9";
    case(0x1A):
        return "7";
    case(0x1B):
        return "Minus";
    case(0x1C):
        return "8";
    case(0x1D):
        return "0";
    case(0x1E):
        return "RightBracket";
    case(0x1F):
        return "O";
    case(0x20):
        return "U";
    case(0x21):
        return "LeftBracket";
    case(0x22):
        return "I";
    case(0x23):
        return "P";
    case(0x25):
        return "L";
    case(0x26):
        return "J";
    case(0x27):
        return "Quote";
    case(0x28):
        return "K";
    case(0x29):
        return "Semicolon";
    case(0x2A):
        return "Backslash";
    case(0x2B):
        return "Comma";
    case(0x2C):
        return "Slash";
    case(0x2D):
        return "N";
    case(0x2E):
        return "M";
    case(0x2F):
        return "Period";
    case(0x32):
        return "Grave";
    case(0x41):
        return "KeypadDecimal";
    case(0x43):
        return "KeypadMultiply";
    case(0x45):
        return "KeypadPlus";
    case(0x47):
        return "KeypadClear";
    case(0x4B):
        return "KeypadDivide";
    case(0x4C):
        return "KeypadEnter";
    case(0x4E):
        return "KeypadMinus";
    case(0x51):
        return "KeypadEquals";
    case(0x52):
        return "Keypad0";
    case(0x53):
        return "Keypad1";
    case(0x54):
        return "Keypad2";
    case(0x55):
        return "Keypad3";
    case(0x56):
        return "Keypad4";
    case(0x57):
        return "Keypad5";
    case(0x58):
        return "Keypad6";
    case(0x59):
        return "Keypad7";
    case(0x5B):
        return "Keypad8";
    case(0x5C):
        return "Keypad9";
    case(0x24):
        return "Return";
    case(0x30):
        return "Tab";
    case(0x31):
        return "Space";
    case(0x33):
        return "Delete";
    case(0x35):
        return "Escape";
    case(0x37):
        return "Command";
    case(0x38):
        return "Shift";
    case(0x39):
        return "CapsLock";
    case(0x3A):
        return "Alt";
    case(0x3B):
        return "Control";
    case(0x3C):
        return "RightShift";
    case(0x3D):
        return "RightOption";
    case(0x3E):
        return "RightControl";
    case(0x3F):
        return "Function";
    case(0x40):
        return "F17";
    case(0x48):
        return "VolumeUp";
    case(0x49):
        return "VolumeDown";
    case(0x4A):
        return "Mute";
    case(0x4F):
        return "F18";
    case(0x50):
        return "F19";
    case(0x5A):
        return "F20";
    case(0x60):
        return "F5";
    case(0x61):
        return "F6";
    case(0x62):
        return "F7";
    case(0x63):
        return "F3";
    case(0x64):
        return "F8";
    case(0x65):
        return "F9";
    case(0x67):
        return "F11";
    case(0x69):
        return "F13";
    case(0x6A):
        return "F16";
    case(0x6B):
        return "F14";
    case(0x6D):
        return "F10";
    case(0x6F):
        return "F12";
    case(0x71):
        return "F15";
    case(0x72):
        return "Help";
    case(0x73):
        return "Home";
    case(0x74):
        return "PageUp";
    case(0x75):
        return "ForwardDelete";
    case(0x76):
        return "F4";
    case(0x77):
        return "End";
    case(0x78):
        return "F2";
    case(0x79):
        return "PageDown";
    case(0x7A):
        return "F1";
    case(0x7B):
        return "LeftArrow";
    case(0x7C):
        return "RightArrow";
    case(0x7D):
        return "DownArrow";
    case(0x7E):
        return "UpArrow";
    default:
        return std::to_string(keycode);
    }
}



