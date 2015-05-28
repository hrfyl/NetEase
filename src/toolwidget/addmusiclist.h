#ifndef ADDMUSICLIST_H
#define ADDMUSICLIST_H

#include <QDialog>

class QPushButton;
class QLineEdit;

class AddMusicList : public QDialog
{
	Q_OBJECT
public:
	explicit AddMusicList(QWidget *parent = 0);
	~AddMusicList();

public slots:


signals:
	void okButtonClicked(QString listName, QStringList files);

private slots:
	void okButtonClicked();
	void checkButtonClicked();

private:
	QLineEdit *m_pathLineEdit;
	QLineEdit *m_nameLineEdit;
	QPushButton *m_okButton;
	QPushButton *m_cancelButton;
	QPushButton *m_checkPathButton;

	QStringList m_files;
};

#endif // ADDMUSICLIST_H
