#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Password Manager");

    ui->toUnlockButton->setVisible(false);

    connect(ui->unlockButton, SIGNAL(clicked()), this, SLOT(unlockVaultButtonClicked()));
    connect(ui->createButton, SIGNAL(clicked()), this, SLOT(createVaultButtonClicked()));

    connect(ui->toUnlockButton, SIGNAL(clicked()), this, SLOT(setUnlockPage()));
    connect(ui->toCreateButton, SIGNAL(clicked()), this, SLOT(setCreatePage()));

    connect(ui->showUnlockButton, SIGNAL(clicked()), this, SLOT(showUnlockButtonClicked()));
    connect(ui->showCreateButton, SIGNAL(clicked()), this, SLOT(showCreateButtonClicked()));

    if (checkDatabaseExist())
    {
        setUnlockPage();
    }
    else
    {
        setCreatePage();
    }

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::unlockVaultButtonClicked()
{
    m_masterPassword = Encryption::generateKeyHash(ui->inputPasswordTextBox->text().toStdString());
    
    //Open db
    sqlite3* db;
    sqlite3_open("./db.db", &db);

    //create statement to extract hashed password
    sqlite3_stmt* stmt;
    const char* check = "SELECT * FROM MASTERPASSWORD";
    std::string hashed;

    int rc = sqlite3_prepare_v2(db, check, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cout << "Error : " << sqlite3_errmsg(db) << '\n';
    }
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        hashed = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    }
    if (rc != SQLITE_DONE) {
        std::cout << "Error: " << sqlite3_errmsg(db) << '\n';
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);

    //Check hashed password with input
    if (m_masterPassword == hashed)
        QDialog::done(QDialog::Accepted);

    //notify user password does not match
}

void LoginDialog::createVaultButtonClicked()
{
    //Check password constraints

    //remove DB if there is one(coming from unlock page)
    QFile::remove("./db.db");
    
    //Create new DB and open
    sqlite3* db;
    sqlite3_open("./db.db", &db);

    //Create Password table
    char* messageError;
    std::string master = "CREATE TABLE MASTERPASSWORD(" \
        "ID INTEGER  PRIMARY   KEY, " \
        "HASHPASSWORD		   TEXT	NOT NULL); ";
    int rc = sqlite3_exec(db, master.c_str(), NULL, 0, &messageError);

    // Hash masterpassword and add to db
    std::string masterpassword = Encryption::generateKeyHash(ui->createPasswordTextBox->text().toStdString());
    std::string sql = "INSERT INTO MASTERPASSWORD (HASHPASSWORD) VALUES(?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, masterpassword.c_str(), -1, NULL);
    int in = sqlite3_step(stmt);
    
    if (in != SQLITE_DONE) {
        std::cout << "Error: " << sqlite3_errmsg(db) << '\n';
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
    QDialog::done(QDialog::Accepted);
}

void LoginDialog::setUnlockPage()
{
    ui->stackedWidget->setCurrentWidget(ui->unlockPage);
    ui->toUnlockButton->setVisible(true);
}

void LoginDialog::setCreatePage()
{
    ui->stackedWidget->setCurrentWidget(ui->createPage);
}

std::string LoginDialog::getHashedPassword() const
{
    return m_masterPassword;
}

bool LoginDialog::checkDatabaseExist()
{
    return QFileInfo::exists("./db.db");
}

void LoginDialog::showUnlockButtonClicked()
{
    if (!showUnlockPassword)
    {
        ui->showUnlockButton->setText("Hide");
        ui->inputPasswordTextBox->setEchoMode(QLineEdit::Normal);
        showUnlockPassword = true;
    }
    else
    {
        ui->showUnlockButton->setText("Show");
        ui->inputPasswordTextBox->setEchoMode(QLineEdit::Password);
        showUnlockPassword = false;
    }
}

void LoginDialog::showCreateButtonClicked()
{
    if (!showCreatePassword)
    {
        ui->showCreateButton->setText("Hide");
        ui->createPasswordTextBox->setEchoMode(QLineEdit::Normal);
        showCreatePassword = true;
    }
    else
    {
        ui->showCreateButton->setText("Show");
        ui->createPasswordTextBox->setEchoMode(QLineEdit::Password);
        showCreatePassword = false;
    }
}