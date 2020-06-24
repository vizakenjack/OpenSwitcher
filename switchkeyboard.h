#ifndef SWITCHKEYBOARD_H
#define SWITCHKEYBOARD_H

#endif // SWITCHKEYBOARD_H
#import "string"
#import "vector"

void ignoreOtherApps();
const char* getCurrentAppFolder();
int xkbswitch(int argc, const char * argv[]);
const char* getCurrentKeyboardLayout();
std::vector<std::string> getActiveKeyboardLayouts();
const char* getAppSupportFolder();

std::wstring getClipBoardData();
void editClipBoardData(std::wstring new_clipboard);
