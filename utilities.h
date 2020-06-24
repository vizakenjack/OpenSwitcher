#ifndef UTILITIES_H
#define UTILITIES_H

#endif // UTILITIES_H
#include <string>
#include <unordered_map>

bool doseFileExist(const char* fileName);
void switchKeyboard(const char* nameToSwitch);
void saveSettings(int hotkey,std::string location);
int loadSettings(std::string location);
std::string getKeyTextFromCode (int keycode);
bool isInvalidTypeingKey(int keycode);

inline void Press(int key, bool shiftDown = 0,bool commandDown = 0);
inline void Release(int key, bool shiftDown,bool commandDown);
void Click(int key, bool shiftDown,bool commandDown);

//std::unordered_map<wchar_t,wchar_t> russianToEnglish;

std::string exec(const char* cmd);

#include <QThread>

class EventTapSetupThread : public QThread {
    Q_OBJECT

    void run();
    // Define signal:
    signals:
    void errorOccured(QString info);
};

class eventTapMessage : public QObject {
    Q_OBJECT
public slots:
    void onErrorOccured(QString info);
};

