#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showApp();
    void on_exit();
    void create_tray_icon();
    void on_pushButton_clicked();
    void syncThread();
private:
    void testIfAdmin();
    void closeEvent (QCloseEvent *event);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
