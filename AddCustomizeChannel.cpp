#include "AddCustomizeChannel.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QComboBox>

AddCustomizeChannel::AddCustomizeChannel(QWidget *parent) :  QWidget(parent),
    m_cusName(""), m_MaxNum(25), m_cusChannel(3,3), m_mergeId(), m_previewWinVec(),
    m_hasMerged(false), m_mousedown(false), m_selRowCol(0,0)
{
	InitUi();
}

void AddCustomizeChannel::SetCustomizeBtnFn(std::function<void(const QString& name)> f)
{
	m_cusChannelBtnFn = f;
}

std::pair<int, int> AddCustomizeChannel::GetCusChannel()
{
	return m_cusChannel;
}

std::set<int> AddCustomizeChannel::GetMergeId()
{
	return m_mergeId;
}

std::pair<int, int> AddCustomizeChannel::GetSelRowCol()
{
	return m_selRowCol;
}

void AddCustomizeChannel::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton && !m_hasMerged)
	{
        for (std::size_t i = 0; i < m_previewWinVec.size(); ++i)
		{
            m_previewWinVec[i]->setStyleSheet(QString::fromUtf8("border:1px solid #1D4ABD;"));
		}
		m_startpoint = event->pos();
		m_mousedown = true;
        m_paintRect->hide();
	}
}

void AddCustomizeChannel::mouseMoveEvent(QMouseEvent * event)
{
	if (event->buttons() == Qt::LeftButton && !m_hasMerged)
	{
		if (m_mousedown) 
		{
            m_endpoint = event->pos();
            int endX = m_endpoint.x();
            int endY = m_endpoint.y();
            int startX = m_startpoint.x();
            int startY = m_startpoint.y();
            if(m_endpoint.x()<m_startpoint.x())
            {
                startX = m_endpoint.x();
                endX = m_startpoint.x();
            }
            if(m_endpoint.y()<m_startpoint.y())
            {
                startY = m_endpoint.y();
                endY = m_startpoint.y();
            }
            QRect rect(QPoint(startX, startY), QPoint(endX, endY));
            m_paintRect->setGeometry(rect);
            m_paintRect->raise();
            update();
            m_paintRect->show();
        }
	}
}

void AddCustomizeChannel::mouseReleaseEvent(QMouseEvent * event)
{
	if (m_mousedown && !m_hasMerged) 
	{
		m_endpoint = event->pos();
		m_mousedown = false;
		QRect rect(m_startpoint, m_endpoint);
		int r = 0,c = 0,rtmp = 0;
		bool find = false;
		m_mergeId.clear();
		for (int i = 0; i < m_cusChannel.second; ++i)
		{
			rtmp = 0;
			for (int j = 0; j < m_cusChannel.first; ++j)
			{
				int idx = i * m_cusChannel.first + j;
				if (rect.intersects(QRect(m_previewWinVec[idx]->pos(), m_previewWinVec[idx]->pos() + QPoint(m_previewWinVec[idx]->width(), m_previewWinVec[idx]->height()))))
				{
					find = true;
					++rtmp;
					m_mergeId.insert(idx);
                    m_previewWinVec[idx]->setStyleSheet("border:1px solid #FF6E35;background-color:#1D4ABD;");
				}
			}
			if (find)
			{
				++c;
				r = rtmp;
				find = false;
			}
		}
		m_selRowCol = { r , c };
		update();
	}
}

/*
void AddCustomizeChannel::paintEvent(QPaintEvent * event)
{
	if (m_paint)
	{
        QPainter painter(this);
		painter.setPen(Qt::red);
		QRect rect(m_startpoint, m_endpoint);
		painter.drawRect(rect);
	}
}
*/

void AddCustomizeChannel::InitUi()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    setStyleSheet("background-color: #FFFFFF");
    setFixedSize(500, 650);
    QLabel* bgLbl = new QLabel(this);
    bgLbl->setFixedSize(500, 650);
    bgLbl->setStyleSheet("background-color: #FFFFFF");
    m_paintRect = new QLabel(this);
    m_paintRect->setStyleSheet("background-color: transparent;border-style:dotted;border-width:1px;border-color:#FFFFFF");
    m_paintRect->hide();

	QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFont font("MicrosoftYaHei", 16, QFont::Normal);
    font.setPixelSize(16);

    QHBoxLayout* titleLayout = new QHBoxLayout;
    QLabel* titleL = new QLabel(tr("new frame"));
    titleL->setFont(font);
    QPushButton* closeBtn = new QPushButton;
    SetPicBtnAttr(closeBtn, ":/img/guanbi.png");
    connect(closeBtn, &QPushButton::clicked, this, [this](){hide();});
    titleLayout->addWidget(titleL, 0, Qt::AlignLeft);
    titleLayout->addWidget(closeBtn, 0, Qt::AlignRight);

    QHBoxLayout* nameLayout = new QHBoxLayout;
    QLabel* nameL = new QLabel(tr("name"));
    nameL->setFont(font);
    QLineEdit* nameE = new QLineEdit(m_cusName);
	nameLayout->addWidget(nameL);
	nameLayout->addWidget(nameE);

    QStringList stringList;
    for(int i=1;i<=10;++i)
    {
        stringList << QString::number(i);
    }
    QLabel* rowL = new QLabel(tr("colum"));
    rowL->setFont(font);
    QComboBox* xSizeC = new QComboBox();
    xSizeC->addItems(stringList);
    xSizeC->setCurrentText("3");
    QLabel* columL = new QLabel(tr("row"));
    QComboBox* ySizeC = new QComboBox();
    ySizeC->addItems(stringList);
    ySizeC->setCurrentText("3");
    nameLayout->addWidget(rowL);
    nameLayout->addWidget(xSizeC);
    nameLayout->addWidget(columL);
    nameLayout->addWidget(ySizeC);

    m_winLayout = new QGridLayout;
	m_previewWinVec.reserve(m_cusChannel.first*m_cusChannel.second);
	for (int i = 0; i < m_cusChannel.second; ++i)
	{
		for (int j = 0; j < m_cusChannel.first; ++j)
		{
            m_previewWinVec.push_back(new QFrame);
            m_previewWinVec[i*m_cusChannel.first + j]->setStyleSheet("border:1px solid #1D4ABD;");
			m_winLayout->addWidget(m_previewWinVec[i*m_cusChannel.first + j], i, j, 1, 1);
		}
	}

    QHBoxLayout* mergeLayout = new QHBoxLayout;
    QFont btnfont("MicrosoftYaHei", 14, QFont::Normal);
    btnfont.setPixelSize(14);
    QPushButton* mergeButton = new QPushButton(tr("merge"));
    mergeButton->setFont(btnfont);
    SetPicBtnAttr(mergeButton, ":/img/hebing.png");
    connect(mergeButton, &QPushButton::toggled, this, [this, mergeButton](bool checked){
        if(checked)
            SetPicBtnAttr(mergeButton, ":/img/hebing2.png");
        else
            SetPicBtnAttr(mergeButton, ":/img/hebing.png");
    });
    QPushButton* divideButton = new QPushButton(tr("divide"));
    divideButton->setFont(btnfont);
    SetPicBtnAttr(divideButton, ":/img/chaifen.png");
    divideButton->setChecked(true);
    connect(divideButton, &QPushButton::toggled, this, [this, divideButton](bool checked){
        if(checked)
            SetPicBtnAttr(divideButton, ":/img/chaifen2.png");
        else
            SetPicBtnAttr(divideButton, ":/img/chaifen.png");
    });
    QButtonGroup *buttonGround = new QButtonGroup;
    buttonGround->addButton(mergeButton);
    buttonGround->addButton(divideButton);
    buttonGround->setExclusive(true);

    mergeLayout->addWidget(mergeButton);
    mergeLayout->addWidget(divideButton);

    QFont boldfont("MicrosoftYaHeiUI-Bold", 16, QFont::Bold);
    boldfont.setPixelSize(16);
    QHBoxLayout* btnLayout = new QHBoxLayout;
    QPushButton* cancelBtn = new QPushButton(tr("cancel"));
    cancelBtn->setFixedSize(120,36);
    cancelBtn->setStyleSheet("border-radius:8px;color:#1D4ABD;"
                             "background-color:#FFFFFF;"
                             "border-style:solid;border-width:1px;border-color:#1D4ABD");
    cancelBtn->setFont(boldfont);
    QPushButton* sureBtn = new QPushButton(tr("save"));
    sureBtn->setFixedSize(120,36);
    sureBtn->setStyleSheet("border-radius:8px;color:#FFFFFF;background-color:#1D4ABD");
    sureBtn->setFont(boldfont);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(sureBtn);

    mainLayout->addLayout(titleLayout,59);
    mainLayout->addLayout(nameLayout,50);
    mainLayout->addLayout(m_winLayout,460);
	mainLayout->addLayout(mergeLayout,82);
    mainLayout->addLayout(btnLayout,89);

	connect(mergeButton, SIGNAL(clicked()), this, SLOT(MergeClickedSlot()));
    connect(divideButton, SIGNAL(clicked()), this, SLOT(CancelMergeClickedSlot()));
    connect(xSizeC, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(ColumChangedSlot(const QString&)));
    connect(ySizeC, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(RowChangedSlot(const QString&)));
	connect(nameE, SIGNAL(textChanged(const QString&)), this, SLOT(NameChangedSlot(const QString&)));
	connect(sureBtn, SIGNAL(clicked()),this, SLOT(SureClickedSlot()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(CancelClickedSlot()));
}

void AddCustomizeChannel::MergeClickedSlot()
{
    for (std::size_t i = 0; i < m_previewWinVec.size(); ++i)
	{
		m_winLayout->removeWidget(m_previewWinVec[i]);
		delete m_previewWinVec[i];
	}
	m_previewWinVec.clear();
	m_previewWinVec.resize(m_cusChannel.first*m_cusChannel.second - m_mergeId.size()+1);
	bool first = false;
	int cnt = 0;
	for (int i = 0; i < m_cusChannel.second; ++i)
	{
		for (int j = 0; j < m_cusChannel.first; ++j)
		{
			int idx = i * m_cusChannel.first + j;
			if (m_mergeId.find(idx) != m_mergeId.end())
			{
				if (!first)
				{
					first = true;
					m_previewWinVec[cnt] = new QFrame(this);
                    m_previewWinVec[cnt]->setStyleSheet(QString::fromUtf8("border:1px solid #1D4ABD;"));
					m_winLayout->addWidget(m_previewWinVec[cnt], i, j, m_selRowCol.second, m_selRowCol.first);
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
                m_previewWinVec[cnt]->setStyleSheet(QString::fromUtf8("border:1px solid #1D4ABD;"));
				m_winLayout->addWidget(m_previewWinVec[cnt], i, j, 1, 1);
				++cnt;
			}
		}
	}
	m_hasMerged = true;
}

void AddCustomizeChannel::CancelMergeClickedSlot()
{
    for (std::size_t i = 0; i < m_previewWinVec.size(); ++i)
	{
		m_winLayout->removeWidget(m_previewWinVec[i]);
		delete m_previewWinVec[i];
	}
	m_previewWinVec.clear();
	m_previewWinVec.resize(m_cusChannel.first*m_cusChannel.second);
	for (int i = 0; i < m_cusChannel.second; ++i)
	{
		for (int j = 0; j < m_cusChannel.first; ++j)
		{
			m_previewWinVec[i*m_cusChannel.first + j] = new QFrame(this);
            m_previewWinVec[i*m_cusChannel.first + j]->setStyleSheet("border:1px solid #1D4ABD;");
			m_winLayout->addWidget(m_previewWinVec[i*m_cusChannel.first + j], i, j, 1, 1);
		}
	}
	m_hasMerged = false;
    m_mergeId.clear();
    m_selRowCol={};
}

void AddCustomizeChannel::ColumChangedSlot(const QString& text)
{
	int input = text.toInt();
	if (input*m_cusChannel.second > m_MaxNum || input<=0)
		return;
	m_cusChannel.first = input;
	CancelMergeClickedSlot();
}

void AddCustomizeChannel::RowChangedSlot(const QString& text)
{
	int input = text.toInt();
	if (input*m_cusChannel.first > m_MaxNum || input <= 0)
		return;
	m_cusChannel.second = input;
	CancelMergeClickedSlot();
}

void AddCustomizeChannel::NameChangedSlot(const QString & text)
{
	m_cusName = text;
}

void AddCustomizeChannel::SureClickedSlot()
{
	m_cusChannelBtnFn(m_cusName);
	close();
}

void AddCustomizeChannel::CancelClickedSlot()
{
    close();
}

void AddCustomizeChannel::SetPicBtnAttr(QPushButton *btn, const QString &picPath)
{
    btn->setStyleSheet("border:none;");
    btn->resize(QPixmap(picPath).size());
    btn->setIcon(QIcon(picPath));
    btn->setIconSize(btn->size());
    btn->setCheckable(true);
}
