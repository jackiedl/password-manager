#ifndef ENTRYCARD_H
#define ENTRYCARD_H

#include <QWidget>
#include "PasswordEntry.h"

namespace Ui {
class EntryCard;
}

class EntryCard : public QWidget
{
    Q_OBJECT

public:
    explicit EntryCard(const PasswordEntry& entry, QWidget *parent = nullptr);
    ~EntryCard();

    void populateInfo(const PasswordEntry& entry);

signals:
    void emitDeleteButtonClicked(const int i);
    void emitUpdateButtonClicked(const PasswordEntry& entry);

private:
    Ui::EntryCard *ui;

    PasswordEntry m_entry;

    bool showPassword{ false };
    bool editEntry{ false };

private slots:
    void editButtonClicked();
    void deleteButtonClicked();
    void updateButtonClicked();
    void showButtonClicked();
};

#endif // ENTRYCARD_H
