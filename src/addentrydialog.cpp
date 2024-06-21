#include "addentrydialog.h"
#include "ui_addentrydialog.h"

AddEntryDialog::AddEntryDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddEntryDialog)
{
    ui->setupUi(this);

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
    connect(ui->showButton, SIGNAL(clicked()), this, SLOT(showButtonClicked()));
}

AddEntryDialog::~AddEntryDialog()
{
    delete ui;
}

PasswordEntry AddEntryDialog::createEntry()
{
    std::string name = ui->nameTextBox->text().toStdString();
    std::string username = ui->usernameTextBox->text().toStdString();
    std::string password = ui->passwordTextBox->text().toStdString();
    std::string url = ui->urlTextBox->text().toStdString();

    return PasswordEntry(name, username, password, url);
}

void AddEntryDialog::addButtonClicked()
{
    //check if all items are inputted correctly

    QDialog::done(QDialog::Accepted);
}

void AddEntryDialog::cancelButtonClicked()
{
    QDialog::done(QDialog::Rejected);
}

void AddEntryDialog::showButtonClicked()
{
    if (!showPassword)
    {
        ui->showButton->setText("Hide");
        ui->passwordTextBox->setEchoMode(QLineEdit::Normal);
        showPassword = true;
    }
    else
    {
        ui->showButton->setText("Show");
        ui->passwordTextBox->setEchoMode(QLineEdit::Password);
        showPassword = false;
    }
}

