#include "EditCustomizeChannel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>

EditCustomizeChannel::EditCustomizeChannel(std::shared_ptr<std::map<QString, CusChannelData>> mapDataPtr, std::shared_ptr<std::set<QString>> nameSet):
    m_reCusName(""), m_MaxNum(25), m_cusDataMapPtr(mapDataPtr), m_nameSet(nameSet),
	m_currName(mapDataPtr->begin()->first), m_curr(mapDataPtr->begin()->second), m_previewWinVec(),
	m_hasMerged(false), m_mousedown(false), m_paint(false)
{
	InitUi();
	setMinimumSize(600, 800);
}

EditCustomizeChannel::~EditCustomizeChannel()
{
}

void EditCustomizeChannel::SetCustomizeBtnFn(std::function<void(const QString&name, const QString&rename, bool isDelete, bool isDelComs)> f)
{
	m_cusChannelBtnFn = f;
}

void EditCustomizeChannel::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton && !m_hasMerged)
	{
		for (int i = 0; i < m_previewWinVec.size(); ++i)
		{
			m_previewWinVec[i]->setStyleSheet(QString::fromUtf8("border:1px solid black"));
		}
		m_startpoint = event->pos();
		m_mousedown = true;
	}
}

void EditCustomizeChannel::mouseMoveEvent(QMouseEvent * event)
{
	if (event->buttons() == Qt::LeftButton && !m_hasMerged)
	{
		if (m_mousedown)
		{
			m_endpoint = event->pos();
			m_paint = true;
			update();
		}
	}
}

void EditCustomizeChannel::mouseReleaseEvent(QMouseEvent * event)
{
	if (m_mousedown && !m_hasMerged)
	{
		m_endpoint = event->pos();
		m_mousedown = false;
		m_paint = false;
		QRect rect(m_startpoint, m_endpoint);
		int r = 0, c = 0, rtmp = 0;
		bool find = false;
		m_curr.m_mergeId.clear();
		for (int i = 0; i < m_curr.m_cusChannel.second; ++i)
		{
			rtmp = 0;
			for (int j = 0; j < m_curr.m_cusChannel.first; ++j)
			{
				int idx = i * m_curr.m_cusChannel.first + j;
				if (rect.intersects(QRect(m_previewWinVec[idx]->pos(), m_previewWinVec[idx]->pos() + QPoint(m_previewWinVec[idx]->width(), m_previewWinVec[idx]->height()))))
				{
					find = true;
					++rtmp;
					m_curr.m_mergeId.insert(idx);
					m_previewWinVec[idx]->setStyleSheet(QString::fromUtf8("border:1px solid red"));
				}
			}
			if (find)
			{
				++c;
				r = rtmp;
				find = false;
			}
		}
		m_curr.m_selRowCol = { r , c };
		update();
	}
}

void EditCustomizeChannel::paintEvent(QPaintEvent * event)
{
	if (m_paint)
	{
		QPainter painter(this);
		painter.setPen(Qt::red);
		QRect rect(m_startpoint, m_endpoint);
		painter.drawRect(rect);
	}
}

void EditCustomizeChannel::InitUi()
{
	setWindowTitle("Edit Customize Channel");
	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	QHBoxLayout* nameLayout = new QHBoxLayout(this);
	QLabel* nameL = new QLabel("name", this);
	m_nameCb = new QComboBox(this);
	
	nameLayout->addWidget(nameL);
	nameLayout->addWidget(m_nameCb);

	QHBoxLayout* renameLayout = new QHBoxLayout(this);
	QLabel* renameL = new QLabel("rename", this);
	QLineEdit* renameE = new QLineEdit(this);
	renameLayout->addWidget(renameL);
	renameLayout->addWidget(renameE);

	QHBoxLayout* sizeLayout = new QHBoxLayout(this);
	QLabel* sizeL = new QLabel("size", this);
	QLineEdit* xSizeE = new QLineEdit(QString::number(m_curr.m_cusChannel.first), this);
	QLabel* multiplyL = new QLabel("*", this);
	QLineEdit* ySizeE = new QLineEdit(QString::number(m_curr.m_cusChannel.second), this);
	sizeLayout->addWidget(sizeL);
	sizeLayout->addWidget(xSizeE);
	sizeLayout->addWidget(multiplyL);
	sizeLayout->addWidget(ySizeE);

	QHBoxLayout* mergeLayout = new QHBoxLayout(this);
	QLabel* diviL = new QLabel("division", this);
	QPushButton* mergeButton = new QPushButton("merge", this);
	QPushButton* cancelMergeButton = new QPushButton("cancel merge", this);
	mergeLayout->addWidget(diviL);
	mergeLayout->addWidget(mergeButton);
	mergeLayout->addWidget(cancelMergeButton);

	m_winLayout = new QGridLayout(this);
	WinLayout();
	RefreshCombo();
	QHBoxLayout* btnLayout = new QHBoxLayout(this);
	QPushButton* editBtn = new QPushButton("edit", this);
	QPushButton* cancelBtn = new QPushButton("cancel", this);
	QPushButton* deleteBtn = new QPushButton("delete", this);
	btnLayout->addWidget(editBtn);
	btnLayout->addWidget(cancelBtn);
	btnLayout->addWidget(deleteBtn);

	mainLayout->addLayout(nameLayout);
	mainLayout->addLayout(renameLayout);
	mainLayout->addLayout(sizeLayout);
	mainLayout->addLayout(mergeLayout);
	mainLayout->addLayout(m_winLayout);
	mainLayout->addLayout(btnLayout);
	setLayout(mainLayout);

	connect(mergeButton, SIGNAL(clicked()), this, SLOT(MergeClickedSlot()));
	connect(cancelMergeButton, SIGNAL(clicked()), this, SLOT(CancelMergeClickedSlot()));
	connect(xSizeE, SIGNAL(textChanged(const QString&)), this, SLOT(ColumChangedSlot(const QString&)));
	connect(ySizeE, SIGNAL(textChanged(const QString&)), this, SLOT(RowChangedSlot(const QString&)));
	connect(renameE, SIGNAL(textChanged(const QString&)), this, SLOT(RenameChangedSlot(const QString&)));
	connect(editBtn, SIGNAL(clicked()), this, SLOT(EditClickedSlot()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(CancelClickedSlot()));
	connect(deleteBtn, SIGNAL(clicked()), this, SLOT(DeleteClickedSlot()));
	connect(m_nameCb, SIGNAL(currentIndexChanged(int)), this, SLOT(nameChangedSlot(int)));
}

void EditCustomizeChannel::WinLayout()
{
	m_previewWinVec.clear();
	m_previewWinVec.resize(m_curr.m_cusChannel.first*m_curr.m_cusChannel.second - m_curr.m_mergeId.size() + 1);
	bool first = false;
	int cnt = 0;
	for (int i = 0; i < m_curr.m_cusChannel.second; ++i)
	{
		for (int j = 0; j < m_curr.m_cusChannel.first; ++j)
		{
			int idx = i * m_curr.m_cusChannel.first + j;
			if (m_curr.m_mergeId.find(idx) != m_curr.m_mergeId.end())
			{
				if (!first)
				{
					first = true;
					m_previewWinVec[cnt] = new QFrame(this);
					m_previewWinVec[cnt]->setStyleSheet(QString::fromUtf8("border:1px solid black"));
					m_winLayout->addWidget(m_previewWinVec[cnt], i, j, m_curr.m_selRowCol.second, m_curr.m_selRowCol.first);
					++cnt;
				}
				else
				{
					continue;
				}
			}
			else
			{
				m_previewWinVec[cnt] = new QFrame(this);
				m_previewWinVec[cnt]->setStyleSheet(QString::fromUtf8("border:1px solid black"));
				m_winLayout->addWidget(m_previewWinVec[cnt], i, j, 1, 1);
				++cnt;
			}
		}
	}
	m_hasMerged = true;
}

void EditCustomizeChannel::RefreshCombo()
{
	m_nameCb->clear();
	for (auto iter = m_cusDataMapPtr->begin(); iter != m_cusDataMapPtr->end(); ++iter)
	{
		m_nameCb->addItem(iter->first);
	}
}

void EditCustomizeChannel::MergeClickedSlot()
{
	for (int i = 0; i < m_previewWinVec.size(); ++i)
	{
		m_winLayout->removeWidget(m_previewWinVec[i]);
		delete m_previewWinVec[i];
	}
	WinLayout();
}

void EditCustomizeChannel::CancelMergeClickedSlot()
{
	for (int i = 0; i < m_previewWinVec.size(); ++i)
	{
		m_winLayout->removeWidget(m_previewWinVec[i]);
		delete m_previewWinVec[i];
	}
	m_previewWinVec.clear();
	m_previewWinVec.resize(m_curr.m_cusChannel.first*m_curr.m_cusChannel.second);
	for (int i = 0; i < m_curr.m_cusChannel.second; ++i)
	{
		for (int j = 0; j < m_curr.m_cusChannel.first; ++j)
		{
			m_previewWinVec[i*m_curr.m_cusChannel.first + j] = new QFrame(this);
			m_previewWinVec[i*m_curr.m_cusChannel.first + j]->setStyleSheet(QString::fromUtf8("border:1px solid black"));
			m_winLayout->addWidget(m_previewWinVec[i*m_curr.m_cusChannel.first + j], i, j, 1, 1);
		}
	}
	m_curr.m_mergeId.clear();
	m_curr.m_selRowCol = std::make_pair(0, 0);
	m_hasMerged = false;
}

void EditCustomizeChannel::ColumChangedSlot(const QString & text)
{
	int input = text.toInt();
	if (input*m_curr.m_cusChannel.second > m_MaxNum || input <= 0)
		return;
	m_curr.m_cusChannel.first = input;
	CancelMergeClickedSlot();
}

void EditCustomizeChannel::RowChangedSlot(const QString & text)
{
	int input = text.toInt();
	if (input*m_curr.m_cusChannel.first > m_MaxNum || input <= 0)
		return;
	m_curr.m_cusChannel.second = input;
	CancelMergeClickedSlot();
}

void EditCustomizeChannel::RenameChangedSlot(const QString & text)
{
	m_reCusName = text;
}

void EditCustomizeChannel::EditClickedSlot()
{
	if (m_reCusName != "" && m_nameSet->find(m_reCusName) != m_nameSet->end())
		return;
	m_cusChannelBtnFn(m_currName, m_reCusName, false, false);
	auto iter = m_cusDataMapPtr->find(m_currName);
	m_cusDataMapPtr->erase(iter);
	if (m_reCusName == "")
	{
		m_reCusName = m_currName;
	}
	else
	{
		auto i = m_nameSet->find(m_currName);
		m_nameSet->erase(i);
		m_nameSet->insert(m_reCusName);
	}
	auto newData = std::make_pair(m_reCusName, m_curr);
	m_cusDataMapPtr->insert(newData);
	auto begin = m_cusDataMapPtr->begin();
	m_currName = begin->first;
	m_curr = begin->second;
	RefreshCombo();
}

void EditCustomizeChannel::CancelClickedSlot()
{
	close();
}

void EditCustomizeChannel::DeleteClickedSlot()
{
	auto iter = m_cusDataMapPtr->find(m_currName);
	m_cusDataMapPtr->erase(iter);
	auto i = m_nameSet->find(m_currName);
	m_nameSet->erase(i);
	auto begin = m_cusDataMapPtr->begin();
	if (begin == m_cusDataMapPtr->end())
	{
		close();
		m_cusChannelBtnFn(m_currName, m_reCusName, true, true);
		return;
	}
	m_cusChannelBtnFn(m_currName, m_reCusName, true, false);
	m_currName = begin->first;
	m_curr = begin->second;
	RefreshCombo();
}

void EditCustomizeChannel::nameChangedSlot(int idx)
{
	auto iter = m_cusDataMapPtr->begin();
	for (int i=0;i<idx;++i)
	{
		++iter;
	}
	m_curr = iter->second;
	m_currName = iter->first;
}
