#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "stdio.h"
#include "QDir"
#include "switchkeyboard.h"
#include <ApplicationServices/ApplicationServices.h>
#include "thread"
#include <sstream>
#include "utilities.h"
#include "QMessageBox"
#include <chrono>
#include <QThread>
#include <adminaccessinstructions.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <QSystemTrayIcon>
#include <QMenu>
#include <atomic>

// open accesability:   open "x-apple.systempreferences:com.apple.preference.security?Privacy_Accessibility"

std::string saveLocation;
std::vector<std::string> allActiveKeyboards;
std::string currentKeyboard;

std::vector<std::string> currentInstalledRussan;
std::vector<std::string> currentInstalledEnglish;
std::mutex eventTapMutex;
int currentHotKey_KeyCode;
bool needChangedKey = false;
QSystemTrayIcon *m_tray_icon;

static std::unordered_map<wchar_t,wchar_t> russianToEnglish = {
    {L'й',L'q'},
    {L'ц',L'w'},
    {L'у',L'e'},
    {L'к',L'r'},
    {L'е',L't'},
    {L'н',L'y'},
    {L'г',L'u'},
    {L'ш',L'i'},
    {L'щ',L'o'},
    {L'з',L'p'},
    {L'ф',L'a'},
    {L'ы',L's'},
    {L'в',L'd'},
    {L'а',L'f'},
    {L'п',L'g'},
    {L'р',L'h'},
    {L'о',L'j'},
    {L'л',L'k'},
    {L'д',L'l'},
    {L'ж',L';'},
    {L'э',L'\''},
    {L'я',L'z'},
    {L'ч',L'x'},
    {L'с',L'c'},
    {L'м',L'v'},
    {L'и',L'b'},
    {L'т',L'n'},
    {L'ь',L'm'},
    {L'б',L','},
    {L'ю',L'.'},
};

static std::unordered_map<wchar_t,wchar_t> englishToRussian = {
    {L'q',L'й'},
    {L'w',L'ц'},
    {L'e',L'у'},
    {L'r',L'к'},
    {L't',L'е'},
    {L'y',L'н'},
    {L'u',L'г'},
    {L'i',L'ш'},
    {L'o',L'щ'},
    {L'p',L'з'},
    {L'a',L'ф'},
    {L's',L'ы'},
    {L'd',L'в'},
    {L'f',L'а'},
    {L'g',L'п'},
    {L'h',L'р'},
    {L'j',L'о'},
    {L'k',L'л'},
    {L'l',L'д'},
    {L';',L'ж'},
    {L'\'',L'э'},
    {L'z',L'я'},
    {L'x',L'ч'},
    {L'c',L'с'},
    {L'v',L'м'},
    {L'b',L'и'},
    {L'n',L'т'},
    {L'm',L'ь'},
    {L',',L'б'},
    {L'.',L'ю'},
};

void findCurrentEngRus(){
    for(auto currentLan : allActiveKeyboards){
        if(currentLan.find("Russian") != std::string::npos)
            currentInstalledRussan.push_back(currentLan);
        if(currentLan.find("British") != std::string::npos)
            currentInstalledEnglish.push_back(currentLan);
        if(currentLan[0] == 'U' && currentLan[1] == 'S')
            currentInstalledEnglish.push_back(currentLan);
    }
}

bool isCurrentKeyboardEnglish(){
    for(auto russianKeyboard : currentInstalledRussan){
        if(russianKeyboard == currentKeyboard)
            return false;
    }
    for(auto englishKeyboard : currentInstalledEnglish){
        if(englishKeyboard == currentKeyboard)
            return true;
    }
}

void toggleKeyboardLanguageEngRus(){
    currentKeyboard = getCurrentKeyboardLayout();
    allActiveKeyboards.clear();
    allActiveKeyboards = getActiveKeyboardLayouts();
    currentInstalledRussan.clear();
    currentInstalledEnglish.clear();
    findCurrentEngRus();

    std::string currentUsedRussian;
    std::string currentUsedEnglish;

    for(auto russianKeyboard : currentInstalledRussan){
        if(russianKeyboard == currentKeyboard)
            currentUsedRussian = currentKeyboard;
    }
    for(auto englishKeyboard : currentInstalledEnglish){
        if(englishKeyboard == currentKeyboard)
            currentUsedEnglish = currentKeyboard;
    }
    if(!currentUsedRussian.empty())
        switchKeyboard(currentInstalledEnglish[0].c_str());
    if(!currentUsedEnglish.empty())
        switchKeyboard(currentInstalledRussan[0].c_str());
}

//https://stackoverflow.com/questions/3202629/where-can-i-find-a-list-of-mac-virtual-key-codes

//bool savedLastLanguageIsEng;
std::vector<std::pair<int,bool>> bufferLastPresses;
//std::atomic_bool  waitForSpacebar{false};
std::atomic_bool waitForKeyAfterSpace{false};

#include <codecvt>
#include <string>
std::wstring utf8_to_wstring (const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}
std::string wstring_to_utf8 (const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

std::atomic_bool lastKeyWasLeftClick {0};

void thread_hotkeyPressed(){
    toggleKeyboardLanguageEngRus();

    // replace highlighted text
    qDebug() << lastKeyWasLeftClick;
    if(lastKeyWasLeftClick){
        //std::string savedOriginalClipData = exec("LANG=en_US.UTF-8 pbpaste");
        std::wstring originalData = getClipBoardData();
        qDebug() << "highlighting code called";

        // 8 is c
        Click(8,0,1);

        bool englishKeyboard = isCurrentKeyboardEnglish();
        std::string clipboardData = exec("LANG=en_US.UTF-8 pbpaste");

        qDebug() << clipboardData.c_str();

        std::wstring wClipboardData = utf8_to_wstring(clipboardData);
        wchar_t * wstring = (wchar_t*)wClipboardData.c_str();
        for(int i =0;i<wClipboardData.length();i++){
            if(englishKeyboard){
                auto iter = englishToRussian.find(wstring[i]);
                if(iter != englishToRussian.end()){
                    wstring[i] = (*iter).second;
                }
                else{
                    qDebug() << "no convertion for charicter found";
                }
            }
            else {
                auto iter = russianToEnglish.find(wstring[i]);
                if(iter != russianToEnglish.end()){
                    wstring[i] = (*iter).second;
                }
                else{
                    qDebug() << "no convertion for charicter found";
                }
            }

        }

        qDebug() << QString::fromWCharArray(wstring);

        std::string str = wstring_to_utf8(wstring);

        std::string terminalCommand = "echo \"";
        terminalCommand += str;
        terminalCommand += "\"| tr -d \"\n\" | LANG=en_US.UTF-8 pbcopy";
        exec(terminalCommand.c_str());
        qDebug() << terminalCommand.c_str();

        // 9 is V
        Click(9,0,1);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));

//        terminalCommand = "echo \"";
//        terminalCommand += savedOriginalClipData;
//        terminalCommand += "\"| tr -d \"\n\" | LANG=en_US.UTF-8 pbcopy";
//        exec(terminalCommand.c_str());
        editClipBoardData(originalData);
    }
    if(bufferLastPresses.size()>0){
        for (int i =0;i<bufferLastPresses.size();i++) {
            Click(51,0,0);//deleate key
        }

        for (int i =0;i<bufferLastPresses.size();i++) {
            Click(bufferLastPresses[i].first,bufferLastPresses[i].second,0);
        }
    }

    // removed feature
//    bool englishKeyboard = !isCurrentKeyboardEnglish();
//    qDebug() << "is english keyboard: " << QString::number(englishKeyboard);
//    qDebug() << "is english keyboard saved: " << QString::number(savedLastLanguageIsEng);
//    if(savedLastLanguageIsEng != englishKeyboard){
//        qDebug() << "unedited langauge from external";
//    }else{
//        // BUG FALSE IDentifying of external app
//        // external program messed with language
//        toggleKeyboardLanguageEngRus();
//        qDebug() << "langauge edited from exterrnal app";
//    }
//    savedLastLanguageIsEng = !isCurrentKeyboardEnglish();
//    waitForSpacebar = true;
}

void updateSavedKeyboard(){
    allActiveKeyboards = getActiveKeyboardLayouts();
    findCurrentEngRus();
    //savedLastLanguageIsEng = isCurrentKeyboardEnglish();
    toggleKeyboardLanguageEngRus();
    usleep(100000);
    toggleKeyboardLanguageEngRus();
}

bool firstTime = true;
int eventNumTimesActive = 0;
bool shoudlNotReceveEvents = false;
bool shouldSkip = false;
// 49 space
CGEventRef myCGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon)
{
    static bool nextTime;
    // detect if the keyboard language has been changed not by this app and dont change language but do retype what is in buffer
    // and reset this detection if a button is pressed that should clear the buffer
    if(firstTime && shoudlNotReceveEvents == false){
        updateSavedKeyboard();
        firstTime = false;
    }

    CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    CGEventFlags eventFlags = CGEventGetFlags(event);

    if(eventFlags > 506871168){// virtual keys have very high flags
        if(keycode == 115)
            m_tray_icon->showMessage("Success",
                                     "KeyBoardSwitcher successfully started",
                                     QSystemTrayIcon::NoIcon, 51000);
        return event;
    }

    if(nextTime && type != kCGEventLeftMouseDown){
        lastKeyWasLeftClick = false;
    }
    nextTime = false;

    if(needChangedKey){
        currentHotKey_KeyCode = keycode;
        needChangedKey = false;
    }

    if(keycode == currentHotKey_KeyCode){// how can you distingwish between keypresses 59){//

        CGEventSetType(event,kCGEventNull);
        CGEventSetFlags(event,NULL);
        CGEventSetIntegerValueField(event, kCGKeyboardEventKeycode, 0xFFFFFFFF);

        eventNumTimesActive++;
        if(eventNumTimesActive == 2)
            eventNumTimesActive = 0;
        else return event;

        if(shouldSkip){
            shouldSkip = false;
            return event;
        }
        std::thread callHotkeyCallback(thread_hotkeyPressed);
        callHotkeyCallback.detach();

        nextTime = true;
        return event;
    }
    else if(eventNumTimesActive == 1){
        shouldSkip = true;
    }

    if(type == kCGEventLeftMouseDown){
        lastKeyWasLeftClick = true;
        qDebug() << 1;
    }else{
        lastKeyWasLeftClick = false;
        qDebug() << 0;
    }

    static short everSecondTime;
    //backspace 51
    if(keycode == 51){
        everSecondTime++;
        if(everSecondTime == 2){
            everSecondTime = 0;
            if(bufferLastPresses.size()>0){
                bufferLastPresses.pop_back();
            }
        }
    }

    // left arrow 123 right arrow 124      tab 48             delete 117                                               36 enter key
    if(keycode == 123 || keycode == 124 || keycode == 48 || keycode == 117 || type == kCGEventLeftMouseDown || keycode == 36){


        bufferLastPresses.clear();

        // could just set a flag
//        currentKeyboard = getCurrentKeyboardLayout();
//        savedLastLanguageIsEng = isCurrentKeyboardEnglish();

//        waitForSpacebar = false;
        waitForKeyAfterSpace = false;
        return event;
    }

    if(type != kCGEventKeyDown)
        return event;

    if(keycode < 0x5C){
        if(keycode > 0x32)
            if(!isInvalidTypeingKey(keycode))
                return event;

        qDebug() << eventFlags;

        //if caps lock or shift
        bool shiftActive = false;
           //shift                           //capslock
        if(131330 == eventFlags || eventFlags == 65792)//0x00020000  kCGEventFlagMaskShift
            shiftActive = true;
        else if(eventFlags != 256)
            return event;
        qDebug() << shiftActive;

        if(waitForKeyAfterSpace && keycode != 49){
            waitForKeyAfterSpace = false;
            bufferLastPresses.clear();
        }

        bufferLastPresses.emplace_back(keycode,shiftActive);

        //space bar = 49
        if(keycode == 49){
            waitForKeyAfterSpace = true;
        }
    }
    return event;
}
// add to plist
//<key>LSUIElement</key>
//<true/>

//https://doc.qt.io/qt-5/appicon.html#setting-the-application-icon-on-macos
//https://www.qtcentre.org/threads/28947-How-to-display-Systemtray-message
//https://apple.stackexchange.com/questions/178313/change-accessibility-setting-on-mac-using-terminal

//sqlite3 "/Library/Application Support/com.apple.TCC/TCC.db" 'SELECT allowed FROM access WHERE client LIKE "%KeyBoardSwitcher"'

// global vertioning affeting plist

void EventTapSetupThread::run() {
    // infinate loop
    CFMachPortRef      eventTap;
    CGEventMask        eventMask;
    CFRunLoopSourceRef runLoopSource;
    eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventKeyUp) | (1 << kCGEventFlagsChanged) | (1 << kCGEventLeftMouseDown));
    eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault,eventMask, myCGEventCallback, NULL);
    if (!eventTap) {
        emit errorOccured("Restarting app");
        return;
    }
    runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource,kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    CFRunLoopRun();
}

void eventTapMessage::onErrorOccured(QString info){
    //std::exit(99);
    //QCoreApplication::exit(666);
}

#include <QApplication>
#include <QProcess>

void restartApp(){
    //std::this_thread::sleep_for(std::chrono::milliseconds(400));
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    std::exit(-1);
}

long getLastEditTime(const char* filename){
    struct stat result;
    stat(filename, &result);
    return result.st_mtime;
}

bool cheakIfAcessebility = false;
bool shouldRestartIfEditTimeChanged = false;
long lastEditTime = NULL;

void MainWindow::testIfAdmin(){
    CFMachPortRef eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault,((1 << kCGEventKeyDown) | (1 << kCGEventKeyUp)), myCGEventCallback, NULL);
    if (!eventTap) {

        shoudlNotReceveEvents = true;

        system("open \"x-apple.systempreferences:com.apple.preference.security?Privacy_Accessibility\"");

        std::string testAppPermitions  = exec("sqlite3 \"/Library/Application Support/com.apple.TCC/TCC.db\" \'SELECT allowed FROM access WHERE client LIKE \"%KeyBoardSwitcher\"\'");
        if(testAppPermitions.find('1') != std::string::npos ){
                lastEditTime = getLastEditTime("/Library/Application Support/com.apple.TCC/TCC.db");
        }

        adminAccessInstructions newPage;
        newPage.setWindowTitle("Error");
        newPage.setModal(true);
        newPage.exec();

        if(lastEditTime != NULL){
            long editTimeNow = getLastEditTime("/Library/Application Support/com.apple.TCC/TCC.db");
            if(editTimeNow != lastEditTime)
                restartApp();
            shouldRestartIfEditTimeChanged = true;
            m_tray_icon->showMessage("Warning - KeyboardSwitcher",
                                     "Please give this app the permissions it needs, it cannot function without them",
                                     QSystemTrayIcon::NoIcon, 151000);
        }else{
            std::string appPermitions  = exec("sqlite3 \"/Library/Application Support/com.apple.TCC/TCC.db\" \'SELECT allowed FROM access WHERE client LIKE \"%KeyBoardSwitcher\"\'");
            if(appPermitions.find('1') != std::string::npos){
                restartApp();
            }else {
                m_tray_icon->showMessage("Warning - KeyboardSwitcher",
                                         "Please give this app the permissions it needs, it cannot function without them",
                                         QSystemTrayIcon::NoIcon, 151000);
                //NoIcon, Information, Warning, Critical
                cheakIfAcessebility = true;
            }
        }
    }
}

void MainWindow::syncThread(){

    int savedHotKey = currentHotKey_KeyCode;
    while(1){
        if(cheakIfAcessebility){
            std::string appPermitions  = exec("sqlite3 \"/Library/Application Support/com.apple.TCC/TCC.db\" \'SELECT allowed FROM access WHERE client LIKE \"%KeyBoardSwitcher\"\'");
            if(appPermitions.find('1') != std::string::npos){
                restartApp();
            }
        }
        if(shouldRestartIfEditTimeChanged){
            long editTimeNow = getLastEditTime("/Library/Application Support/com.apple.TCC/TCC.db");
            if(editTimeNow != lastEditTime)
                restartApp();
        }

        if(savedHotKey != currentHotKey_KeyCode){
            std::string hotkeyText = getKeyTextFromCode(currentHotKey_KeyCode);
            ui->pushButton->setText(hotkeyText.c_str());
            savedHotKey = currentHotKey_KeyCode;
            ui->pushButton->setEnabled(true);
            saveSettings(currentHotKey_KeyCode,saveLocation);
        }
        ui->label_2->setText(std::to_string(bufferLastPresses.size()).c_str());
        usleep(100000);
    }
}

void MainWindow::showApp(){
    if(cheakIfAcessebility || shouldRestartIfEditTimeChanged){
        restartApp();
    }else {
        this->show();
        ignoreOtherApps();
        this->activateWindow();
        this->raise();
        this->setFocus();
    }
}

void MainWindow::on_exit(){
    std::exit(0);
}

void MainWindow::create_tray_icon()
{
   m_tray_icon = new QSystemTrayIcon(QIcon(":/icon.png"), this);

   QAction *quit_action = new QAction( "Exit", m_tray_icon );
   connect( quit_action, SIGNAL(triggered()), this, SLOT(on_exit()) );

   QAction *another_action = new QAction( "Show window", m_tray_icon );
   connect( another_action, SIGNAL(triggered()), this, SLOT(showApp()) );

   QMenu *tray_icon_menu = new QMenu;
   tray_icon_menu->addAction( another_action );
   tray_icon_menu->addAction( quit_action );

   m_tray_icon->setContextMenu( tray_icon_menu );

   m_tray_icon->show();
 }

void MainWindow::closeEvent (QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    create_tray_icon();

    testIfAdmin();

    std::string appFolderPath = getAppSupportFolder(); //getCurrentAppFolder();

    if(!doseFileExist((appFolderPath+"/keyboardSwitcherSave/").c_str())){
        std::stringstream newStream;
        newStream << "cd \"";
        newStream << appFolderPath.c_str();
        newStream << "\"";
        newStream <<  " && mkdir keyboardSwitcherSave";
        qDebug() << newStream.str().c_str();
        system(newStream.str().c_str());
    }

    appFolderPath+="/keyboardSwitcherSave/";

    saveLocation = appFolderPath+"saveData.bin";

    currentKeyboard = getCurrentKeyboardLayout();
    allActiveKeyboards = getActiveKeyboardLayouts();

    for(auto str : allActiveKeyboards){
        qDebug() << str.c_str();
    }

    if(!doseFileExist(saveLocation.c_str())){
        int hotkey = 58;//alt
        saveSettings(hotkey,saveLocation);
    }

    qDebug() << saveLocation.c_str();

    currentHotKey_KeyCode = loadSettings(saveLocation);
    std::string hotkeyText = getKeyTextFromCode(currentHotKey_KeyCode);
    ui->pushButton->setText(hotkeyText.c_str());
    qDebug() << currentHotKey_KeyCode << hotkeyText.c_str();

    eventTapMessage* newEventMessage = new eventTapMessage;
    EventTapSetupThread* setupThead = new EventTapSetupThread;
    connect(setupThead, SIGNAL(errorOccured(QString)),newEventMessage,SLOT(onErrorOccured(QString)));
    //connect(setupThead, SIGNAL(finished()),newEventMessage, SLOT(deleteLater()));
    setupThead->start();

    std::thread t(&MainWindow::syncThread,this);
    t.detach();

    Click(115,0,0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    std::lock_guard<std::mutex> lock(eventTapMutex);
    needChangedKey = true;
    ui->pushButton->setEnabled(false);
}
