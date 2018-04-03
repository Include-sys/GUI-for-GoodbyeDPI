#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    setWindowTitle("Hakkinda");
    setWindowIcon(QIcon(":/images/images/info-button.png"));
}

About::~About()
{
    delete ui;
}
