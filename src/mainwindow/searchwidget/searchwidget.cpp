#include "searchwidget.h"
#include "../../toolwidgets/funcbutton.h"
#include "../../configure/configure.h"
#include "../../toolwidgets/titlewidget.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>

SearchWidget::SearchWidget(QWidget *parent) :
	BasedWidget(parent)
{
	setFixedSize(searchWidgetSize);
	setObjectName("SearchWidget");

	initUi();
	initConnect();
}

void SearchWidget::searchClicked()
{
	QString _search = m_searLineEdit->text();
	if (!m_musicSearch->isChecked() || (_search == ""))
		return;

	emit search(_search);

	hide();
}

void SearchWidget::initUi()
{
	m_title = new TitleWidget(false, this);
	m_title->setFixedSize(searchTitleSize);
	m_title->move(0, 0);

	FuncButton *searchButton = new FuncButton(":/buttons/search_btn", this);
	connect(searchButton, SIGNAL(clicked()), this, SLOT(searchClicked()));

	searchButton->setCapturEnterEvent(false);
	m_searLineEdit = new QLineEdit(this);
	m_searLineEdit->setFixedWidth(searchWidgetSize.width() * 0.85);
	QHBoxLayout *searLayout = new QHBoxLayout;
	searLayout->addWidget(m_searLineEdit, 0, Qt::AlignLeft | Qt::AlignVCenter);
	searLayout->addWidget(searchButton, 0, Qt::AlignVCenter);
	searLayout->addStretch();

	m_musicSearch = new QCheckBox("单曲搜索", this);
	m_aritlsSearch = new QCheckBox("歌手搜索", this);
	m_albumSearch = new QCheckBox("专辑搜索", this);
	m_lyricSearch = new QCheckBox("歌词搜索", this);
	m_listsSearch = new QCheckBox("歌单搜索", this);
	m_ownerSearch = new QCheckBox("主播电台", this);

	m_musicSearch->setChecked(true);

	QHBoxLayout *searCheck1 = new QHBoxLayout;
	searCheck1->addWidget(m_musicSearch, 0, Qt::AlignLeft | Qt::AlignVCenter);
	searCheck1->addWidget(m_aritlsSearch, 0, Qt::AlignVCenter);
	searCheck1->addWidget(m_albumSearch, 0, Qt::AlignRight | Qt::AlignVCenter);
	searCheck1->setContentsMargins(0, 5, 5, 0);

	QHBoxLayout *searCheck2 = new QHBoxLayout;
	searCheck2->addWidget(m_listsSearch, 0, Qt::AlignLeft | Qt::AlignVCenter);
	searCheck2->addWidget(m_lyricSearch, 0, Qt::AlignVCenter);
	searCheck2->addWidget(m_ownerSearch, 0, Qt::AlignRight | Qt::AlignVCenter);
	searCheck2->setContentsMargins(0, 5, 5, 0);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->addSpacing(searchTitleSize.height() + 10);
	mainLayout->addLayout(searLayout);
	mainLayout->addLayout(searCheck1);
	mainLayout->addLayout(searCheck2);

	QLabel *label = new QLabel("<font color='red' size=5>当前只实现单曲查找...</font>", this);
	mainLayout->addWidget(label, 0, Qt::AlignRight | Qt::AlignVCenter);

	mainLayout->setContentsMargins(10, 0, 10, 10);
}

void SearchWidget::initConnect()
{
	connect(m_title, SIGNAL(closeClicked()), this, SLOT(hide()));
}














