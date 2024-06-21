#ifndef ADDENTRYDIALOG_H
#define ADDENTRYDIALOG_H

#include <QDialog>
#include "PasswordEntry.h"

namespace Ui {
class AddEntryDialog;
}

class AddEntryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEntryDialog(QWidget *parent = nullptr);
    ~AddEntryDialog();

    PasswordEntry createEntry();

private:
    Ui::AddEntryDialog *ui;
    bool showPassword{ false };

private slots:
    void addButtonClicked();
    void cancelButtonClicked();
    void showButtonClicked();
};

#endif // ADDENTRYDIALOG_H
