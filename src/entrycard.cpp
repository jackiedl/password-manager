#include "entrycard.h"
#include "ui_entrycard.h"

EntryCard::EntryCard(const PasswordEntry& entry, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::EntryCard)
    , m_entry{entry}
{
    ui->setupUi(this);

    ui->updateButton->setVisible(false);

    connect(ui->editButton, SIGNAL(clicked()), this, SLOT(editButtonClicked()));
    connect(ui->deleteButton, SIGNAL(clicked()),this, SLOT(deleteButtonClicked()));
    connect(ui->updateButton, SIGNAL(clicked()), this, SLOT(updateButtonClicked()));
    connect(ui->showButton, SIGNAL(clicked()), this, SLOT(showButtonClicked()));

    connect(this, SIGNAL(emitDeleteButtonClicked(const int)), parent, SLOT(recieveDeleteButtonClicked(const int)));
    connect(this, SIGNAL(emitUpdateButtonClicked(const PasswordEntry&)), parent, SLOT(recieveUpdateButtonClicked(const PasswordEntry&)));

    ui->nameText->setText(QString::fromStdString(m_entry.getName()));
    ui->usernameText->setText(QString::fromStdString(m_entry.getUsername()));
    ui->passwordText->setText(QString::fromStdString(m_entry.getPassword()));
    ui->urlText->setText(QString::fromStdString(m_entry.getUrl()));

    ui->nameText->setEnabled(false);
    ui->usernameText->setEnabled(false);
    ui->passwordText->setEnabled(false);
    ui->urlText->setEnabled(false);
}

EntryCard::~EntryCard()
{
    delete ui;
}

void EntryCard::editButtonClicked()
{
    if (!editEntry) 
    {
        ui->nameText->setEnabled(true);
        ui->usernameText->setEnabled(true);
        ui->passwordText->setEnabled(true);
        ui->urlText->setEnabled(true);
        ui->editButton->setText("Cancel");
        editEntry = true;
        ui->updateButton->setVisible(true);
    }
    else
    {
        editEntry = false;
        ui->editButton->setText("Edit");
        ui->nameText->setEnabled(false);
        ui->usernameText->setEnabled(false);
        ui->passwordText->setEnabled(false);
        ui->urlText->setEnabled(false);
        ui->updateButton->setVisible(false);
    }
    
}

void EntryCard::deleteButtonClicked()
{
    emit emitDeleteButtonClicked(m_entry.getId());
}

void EntryCard::updateButtonClicked()
{
    //reset card state
    editEntry = false;
    ui->editButton->setText("Edit");
    ui->nameText->setEnabled(false);
    ui->usernameText->setEnabled(false);
    ui->passwordText->setEnabled(false);
    ui->urlText->setEnabled(false);
    //update entry to current value

    m_entry.setName(ui->nameText->text().toStdString());
    m_entry.setUsername(ui->usernameText->text().toStdString());
    m_entry.setPassword(ui->passwordText->text().toStdString());
    m_entry.setUrl(ui->urlText->text().toStdString());

    emit emitUpdateButtonClicked(m_entry);
}

void EntryCard::showButtonClicked()
{
    if (!showPassword)
    {
        ui->showButton->setText("Hide");
        ui->passwordText->setEchoMode(QLineEdit::Normal);
        showPassword = true;
    }
    else
    {
        ui->showButton->setText("Show");
        ui->passwordText->setEchoMode(QLineEdit::Password);
        showPassword = false;
    }
}