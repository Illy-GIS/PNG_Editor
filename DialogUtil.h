#pragma once
#include <qstring.h>
#include <qmessagebox.h>
class DialogUtil {
public:
	static void showError(QString message) {
		QMessageBox::critical(nullptr, "Error", message);
	}

	static void showWarning(QString message) {
		QMessageBox::warning(nullptr, "Warning", message);
	}

	static void showInfo(QString message) {
		QMessageBox::information(nullptr, "Information", message);
	}
};

