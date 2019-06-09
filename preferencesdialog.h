#pragma once

#include <QDialog>

namespace Ui {
	class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
	Q_OBJECT
public:
	explicit PreferencesDialog(QWidget* parent = nullptr);

	~PreferencesDialog();
private:
	Ui::PreferencesDialog* ui;
};
