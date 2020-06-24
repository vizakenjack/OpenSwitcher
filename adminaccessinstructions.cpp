#include "adminaccessinstructions.h"
#include "ui_adminaccessinstructions.h"

adminAccessInstructions::adminAccessInstructions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adminAccessInstructions)
{
    ui->setupUi(this);
}

adminAccessInstructions::~adminAccessInstructions()
{
    delete ui;
}
