#include "gui.h"
#include "ui_gui.h"

GUI::GUI(PasswordManager* pm, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GUI)
    ,m_passwordManager(pm)
{
    ui->setupUi(this);

    ui->randButton->setVisible(false);

    connect(ui->addEntryButton, SIGNAL(clicked()), this, SLOT(addEntryButtonClicked()));

    populateEntries();
}


GUI::~GUI()
{
    delete ui;
}

void GUI::addEntryButtonClicked()
{
    AddEntryDialog addDialog;
    if (addDialog.exec() == QDialog::Accepted)
    {
        PasswordEntry entry = addDialog.createEntry();
        entry.encryptPassword(m_passwordManager->getHashPassword());
        m_passwordManager->addEntry(entry);

        populateEntries();
    }
}

void GUI::populateEntries()
{
    //Clean up if layout exists
    if (ui->scrollArea->layout()) {
        QLayoutItem* child;
        while ((child = ui->scrollArea->layout()->takeAt(0)) != 0)
        {
            delete child->widget();
            delete child;
        }
    }

    //setup ScrollArea    
    QWidget* widget = new QWidget;
    QGridLayout* gridbox = new QGridLayout;
    QVBoxLayout* vbox = new QVBoxLayout;

    int x{ 0 };
    int y{ 0 };
    for (auto e : m_passwordManager->getEntries())
    {
        if (x == 2)
        {
            x = 0;
            y++;
        }
        EntryCard* entry = new EntryCard(e, this);
        gridbox->addWidget(entry, y, x++);
    }

    widget->setLayout(gridbox);

    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setWidget(widget);
    ui->scrollArea->setWindowTitle("Entries");

}

void GUI::recieveDeleteButtonClicked(const int i)
{
    m_passwordManager->removeEntry(i);
    populateEntries();
}

void GUI::recieveUpdateButtonClicked(const PasswordEntry& entry)
{
    PasswordEntry e = entry;
    e.encryptPassword(m_passwordManager->getHashPassword());
    m_passwordManager->updateEntry(e.getId(), e);
    populateEntries();
}