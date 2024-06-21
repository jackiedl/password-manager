#include <QApplication>
#include "GUI.h"
#include "logindialog.h"
#include "PasswordManager.h"


int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	
	LoginDialog loginDialog;

	if (loginDialog.exec() == QDialog::Accepted)
	{
		std::string hashPassword = loginDialog.getHashedPassword();
		PasswordManager passwordManager("./db.db", hashPassword);

		GUI gui{ &passwordManager };

		gui.show();

		return app.exec();
	}
	else
	{
		return 0;
	}
}