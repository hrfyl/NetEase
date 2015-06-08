#include "searchwidget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>

SearchWidget::SearchWidget(QWidget *parent) : QDialog(parent)
{
	QPushButton *okButton = new QPushButton("确定", this);
	QPushButton *cancelButton = new QPushButton("取消", this);
	QHBoxLayout *button = new QHBoxLayout;
	button->addWidget(okButton, 0, Qt::AlignLeft | Qt::AlignVCenter);
	button->addWidget(cancelButton, 0, Qt::AlignRight | Qt::AlignVCenter);
	button->setContentsMargins(5, 0, 5, 0);

	connect(okButton, SIGNAL(clicked()), this, SLOT(on_ok_clicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(on_cancel_clicked()));

	QLabel *searchLabel = new QLabel("输入搜索内容");
	searchLineEdit = new QLineEdit;
	QHBoxLayout *searchLayout = new QHBoxLayout;
	searchLayout->addWidget(searchLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
	searchLayout->addWidget(searchLineEdit, 0, Qt::AlignRight | Qt::AlignVCenter);
	searchLayout->setContentsMargins(5, 0, 5, 0);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(searchLayout);
	mainLayout->addSpacing(15);
	mainLayout->addLayout(button);
}

SearchWidget::~SearchWidget()
{

}

void SearchWidget::on_ok_clicked()
{
	QString s = searchLineEdit->text();
	if (s == "")
	{
		QMessageBox::warning(this, "输入有误", "请输入内容....", QMessageBox::Ok | QMessageBox::Cancel);
		return;
	}
	emit search(s);
	this->setVisible(false);
}

void SearchWidget::on_cancel_clicked()
{
	this->setVisible(false);
}

