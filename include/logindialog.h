#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QFileInfo>
#include <sqlite3.h> 
#include <iostream>
#include "Encryption.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    std::string getHashedPassword() const;

private:
    Ui::LoginDialog* ui;
    std::string m_masterPassword{};
    bool showUnlockPassword{ false };
    bool showCreatePassword{ false };

private slots:
    void unlockVaultButtonClicked();
    void createVaultButtonClicked();

    void setUnlockPage();
    void setCreatePage();

    void showUnlockButtonClicked();
    void showCreateButtonClicked();

private:
    bool checkDatabaseExist();

};

#endif // LOGINDIALOG_H
