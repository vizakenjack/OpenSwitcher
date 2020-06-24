#include "mainwindow.h"
#include <QApplication>
#include <thread>

#include <QProcess>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    w.setWindowTitle("Keyboard language changer");

    int returncode = a.exec();
    if(returncode == 666)// no longer working
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
    return returncode;
}
