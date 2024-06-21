#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include "PasswordManager.h"
#include "addentrydialog.h"
#include "entrycard.h"

namespace Ui {
class GUI;
}

class GUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit GUI(PasswordManager* pm, QWidget *parent = nullptr);
    ~GUI();

    void populateEntries();

public slots:
    void recieveDeleteButtonClicked(const int i);
    void recieveUpdateButtonClicked(const PasswordEntry& entry);

private:
    Ui::GUI *ui;
    PasswordManager* m_passwordManager;

private slots:
    void addEntryButtonClicked();
};

#endif // GUI_H
