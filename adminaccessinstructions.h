#ifndef ADMINACCESSINSTRUCTIONS_H
#define ADMINACCESSINSTRUCTIONS_H

#include <QDialog>

namespace Ui {
class adminAccessInstructions;
}

class adminAccessInstructions : public QDialog
{
    Q_OBJECT

public:
    explicit adminAccessInstructions(QWidget *parent = nullptr);
    ~adminAccessInstructions();

private:
    Ui::adminAccessInstructions *ui;
};

#endif // ADMINACCESSINSTRUCTIONS_H
