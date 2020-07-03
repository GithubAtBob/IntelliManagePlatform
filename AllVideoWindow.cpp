#include "AllVideoWindow.h"
#include "ChannelChoose.h"
#include "VideoWidget.h"
#include "DataInstance.h"
#include "vWidget.h"
#include "DeviceTree.h"
#include "DevSortFilterProxyModel.h"
#include <QCursor>
#include <QPoint>
#include <QDateTime>
#include <QHBoxLayout>
#include <QStandardItem>

AllVideoWindow::AllVideoWindow(std::shared_ptr<VideoControl> vc, DeviceTree* devtree) :
    m_videoControl(vc), m_buttonGroup(new QButtonGroup),
    m_buttonWidget(new QWidget), m_videoLayout(new QGridLayout), m_mainLayout(new QVBoxLayout(this)), m_vWidget(new vWidget(vc,this)),
	m_videoPressed(false), m_videoMove(false), m_selectChannelId(-1), m_targetChannelId(-1), m_name(""), 
    m_nameSet(std::make_shared<std::set<QString>>()), m_addCusChannel(std::make_shared<AddCustomizeChannel>(this)), m_devTree(devtree)
{
    m_windowingWgt = new ChannelChoose(m_addCusChannel, this);
    m_windowingWgt->hide();
    m_videoControl->InitAllWindow(25, this);
	m_videoControl->SetLayout(m_videoLayout);
	m_videoControl->InitLayout();
	InitButton();
    m_vWidget->setLayout(m_videoLayout);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    m_mainLayout->addWidget(m_vWidget, 123);
    m_mainLayout->addWidget(m_buttonWidget, 5);

	setLayout(m_mainLayout);
	SetFn();
    m_addCusChannel->setVisible(false);
    ReadCus();
}

void AllVideoWindow::resetShow(Video::VideoControlEnum i)
{
    m_videoControl->SetVideoControlEnum(i);
}

void AllVideoWindow::SaveCus()
{
    m_videoControl->WriteCusToDatabase();
}

void AllVideoWindow::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		//先将窗体的坐标转换为视频窗体的坐标
		QPoint globalPos = event->globalPos();

        //隐藏分屏窗体
        if(m_isWindowingShow)
        {
            QPoint windowingWgtPos = mapToGlobal(m_windowingWgt->pos());
            QRect rect(windowingWgtPos.x(), windowingWgtPos.y(), m_windowingWgt->width(), m_windowingWgt->height());
            if (rect.contains(globalPos))
            {
                return;
            }
            else
            {
                m_isWindowingShow = false;
            }
        }

		//逐个从所有的窗体比较判断当前是否是属于某个播放窗体
		auto widgets = m_videoControl->GetAllWindow();
		int size = widgets.size();
		int s = m_videoControl->GetStartIdx();
		for(int i=s;i<size;++i)
		{
			auto widget = widgets[i];
			//如果控件不可见则继续,必须是可见的控件
            if (!widget->isVisible())
			{
				continue;
			}

            QPoint widgetPos = static_cast<VideoWidget*>(widget->parentWidget())->mapToGlobal(widget->pos());
			QRect rect(widgetPos.x(), widgetPos.y(), widget->width(), widget->height());
			if (rect.contains(globalPos))
			{
				m_videoPressed = true;
                m_selectChannelId = i;
                DataInstance* d = DataInstance::GetInstance();

                if(m_lastSel != -1)
                    static_cast<VideoWidget*>(widgets[m_lastSel]->parentWidget())->SetbPaint(false);
                m_lastSel = i;
                QStandardItem* lastItem = d->GetCurrItem();
                if(lastItem != nullptr)
                {
                    d->LoadCamIcon(lastItem,false);
                    HWND hwnd = d->GetHwndByItem(lastItem);
                    if(hwnd != NULL)
                    {
                        int idx = m_videoControl->FindByHwnd(hwnd);
                        if(idx != -1)
                        {
                            static_cast<VideoWidget*>(widgets[idx]->parentWidget())->SetbPaint(false);
                        }
                    }
                }
                static_cast<VideoWidget*>(widget->parentWidget())->SetbPaint(true);

                if(widget->IsPlay())
                {
                    d->SetSelHwnd(widget->Gethwnd());
                    QStandardItem* item = d->GetItem(widget->Gethwnd());
                    m_devTree->setCurrentIndex(static_cast<DevSortFilterProxyModel*>(m_devTree->model())->mapFromSource(item->index()));
                    d->SetCurrItem(item);
                    d->LoadCamIcon(item,true);
                }
                else
                {
                    d->SetSelHwnd(NULL);
                    m_devTree->clearSelection();
                    d->SetCurrItem(nullptr);
                }

                /*
                if(m_lastSelIdx!=-1)
                    widgets[m_lastSelIdx]->CancelSelected();
                widget->SetSelected();

                m_lastSelIdx = i;
                */

				break;
			}
		}
	}
	else if (event->button() == Qt::RightButton)
	{
		
	}
	else if (event->button() == Qt::MidButton)
	{
		
	}
}

void AllVideoWindow::mouseMoveEvent(QMouseEvent * event)
{
	if (m_videoPressed) 
	{
		m_videoMove = true;
		setCursor(QCursor(Qt::OpenHandCursor));           

        //先将窗体的坐标转换为视频窗体的坐标
        QPoint globalPos = event->globalPos();

        //逐个从所有的窗体比较判断当前是否是属于某个播放窗体
        m_videoExist = false;
        auto widgets = m_videoControl->GetAllWindow();
        if(m_selectChannelId != -1)
            widgets[m_selectChannelId]->SetIsMove(true);
        int size = widgets.size();
        int s = m_videoControl->GetStartIdx();
        for (int i = s; i < size; ++i)
        {
            auto widget = widgets[i];
            //如果控件不可见则继续,必须是可见的控件
            if (!widget->isVisible())
            {
                continue;
            }

            QPoint widgetPos = static_cast<VideoWidget*>(widget->parentWidget())->mapToGlobal(widget->pos());
            QRect rect(widgetPos.x(), widgetPos.y(), widget->width(), widget->height());
            if (rect.contains(globalPos))
            {
                m_videoExist = true;
                m_targetChannelId = i;
                break;
            }
        }
	}
}

void AllVideoWindow::mouseReleaseEvent(QMouseEvent * event)
{
	if (!m_videoMove) 
	{
		m_selectChannelId = -1;
		setCursor(Qt::ArrowCursor);
		return;
	}

    auto widgets = m_videoControl->GetAllWindow();
	if (m_selectChannelId == -1)
		return;
    else
        widgets[m_selectChannelId]->SetIsMove(false);
	//如果从所有视频画面中没有找到则说明移出到窗体外
	if (!m_videoExist) 
	{
		widgets[m_selectChannelId]->Close();
		widgets[m_selectChannelId]->SetUrl("");
		/*
		QDateTime current_time = QDateTime::currentDateTime();
		m_winInfo->InsertMsg(QStringList() << current_time.toString("hh:mm:ss")
			<< QString::number(m_selectChannelId) + "deleted video");
		*/
	}
	else 
	{
		//立即交换两个窗体信息
		if (m_targetChannelId != -1 && m_targetChannelId != m_selectChannelId) 
		{
			widgets[m_targetChannelId]->setFocus();

			//重新保存url
			QString urlSel = widgets[m_selectChannelId]->GetUrl();
			QString urlTar = widgets[m_targetChannelId]->GetUrl();

            for(auto widget : widgets)
                static_cast<VideoWidget*>(widget->parentWidget())->SetbPaint(false);
            m_lastSel = -1;

            /*
            widgets[m_selectChannelId]->CancelSelected();
            m_lastSelIdx = -1;
            */

            //重新设置新的控件
            int selidx = m_videoControl->GetWindow(m_selectChannelId)->GetChannelId();
            int taridx = m_videoControl->GetWindow(m_targetChannelId)->GetChannelId();

            m_videoControl->SwapVideoWin(m_selectChannelId, m_targetChannelId);
            m_videoControl->SwapVideoWgt(m_selectChannelId,m_targetChannelId);

            m_videoControl->GetWindow(m_selectChannelId)->SetChannelId(selidx);
            m_videoControl->GetWindow(m_targetChannelId)->SetChannelId(taridx);

            DataInstance* d = DataInstance::GetInstance();
            if(!m_videoControl->GetWindow(m_selectChannelId)->IsPlay())
                d->AddItemFreePool(selidx);
            else
                d->RemoveItemFreePool(selidx);
            if(!m_videoControl->GetWindow(m_targetChannelId)->IsPlay())
                d->AddItemFreePool(taridx);
            else
                d->RemoveItemFreePool(taridx);

            int s = m_videoControl->GetStartIdx();
            m_videoControl->InitLayout(s, m_name);
			/*
			QDateTime current_time = QDateTime::currentDateTime();
			m_winInfo->InsertMsg(QStringList() << current_time.toString("hh:mm:ss") 
				<< QString::number(m_targetChannelId) + " changed to " + QString::number(m_selectChannelId));
			*/
		}
	}

	m_videoPressed = false;
	m_videoMove = false;
	m_targetChannelId = -1;

	//改变鼠标形状
    setCursor(Qt::ArrowCursor);
}

void AllVideoWindow::resizeEvent(QResizeEvent *size)
{
    if(m_windowingWgt->isVisible())
    {
        QPoint r(width(),height());
        QPoint gloPos = mapToGlobal(r);
        int w = gloPos.x();
        int h = gloPos.y();
        m_windowingWgt->move(w-m_windowingWgt->width()-60,h-m_windowingWgt->height()-60);
    }
    else if(m_addCusChannel->isVisible())
    {
        QPoint r(width(),height());
        QPoint gloPos = mapToGlobal(r);
        int w = gloPos.x();
        int h = gloPos.y();
        m_addCusChannel->move(w-520,h-710);
    }
}

void AllVideoWindow::InitButton()
{
    m_buttonWidget->setStyleSheet("background-color: #FFFFFF");
    QHBoxLayout* btnLyt = new QHBoxLayout(m_buttonWidget);
    QPushButton* captureBtn = new QPushButton;
    SetPicBtnAttr(captureBtn, ":/img/zhuapai1.png", ":/img/zhuapai.png", tr("capture"));
    connect(captureBtn, &QPushButton::clicked, this, [this](){
        m_videoControl->CaptureAll();
    });
    QPushButton* videotapingBtn = new QPushButton;
    SetPicBtnAttr(videotapingBtn, ":/img/luxiang1.png", ":/img/luxiangzhong.png", tr("videotaping"));
    connect(videotapingBtn, &QPushButton::clicked, this, [this](){
        if(!m_videoingAll)
        {
            m_videoControl->StartVideoingAll();
            m_videoingAll = true;
        }
        else
        {
            m_videoControl->StopVideoingAll();
            m_videoingAll = false;
        }
    });
    QPushButton* talkingBtn = new QPushButton;
    SetPicBtnAttr(talkingBtn, ":/img/yuyin1.png", ":/img/yuyin.png", tr("talking"));
    QPushButton* closeAllBtn = new QPushButton;
    SetPicBtnAttr(closeAllBtn, ":/img/quanbuguanbi1.png",":/img/quanbuguanbi.png", tr("closeAll"));
    connect(closeAllBtn, &QPushButton::clicked, this, [this](){
        m_videoControl->CloseAll();
    });
    QPushButton* pollBtn = new QPushButton;
    SetPicBtnAttr(pollBtn, ":/img/lunxun1.png", ":/img/lunxun2.png", tr("polling"));
    QPushButton* windowingBtn = new QPushButton;
    SetPicBtnAttr(windowingBtn, ":/img/fenping1.png", ":/img/fenping2.png", tr("windowing"));
    connect(windowingBtn, &QPushButton::clicked, this, [this](){
        QPoint r(width(),height());
        QPoint gloPos = mapToGlobal(r);
        int w = gloPos.x();
        int h = gloPos.y();
        m_windowingWgt->move(w-m_windowingWgt->width()-60,h-m_windowingWgt->height()-60);
        if(!m_isWindowingShow)
        {
            m_windowingWgt->show();
            m_isWindowingShow = true;
        }
        else
        {
            m_windowingWgt->hide();
            m_isWindowingShow = false;
        }
    });
    QPushButton* fullWindowBtn = new QPushButton;
    SetPicBtnAttr(fullWindowBtn, ":/img/quangping1.png", ":/img/quangping2.png", tr("fullScreen"));
    connect(fullWindowBtn, &QPushButton::clicked, this, [this](){
        if (m_vWidget->isFullScreen())
        {
            m_vWidget->setWindowFlags(Qt::SubWindow);
            m_vWidget->showNormal();
        }
        else
        {
            m_vWidget->setWindowFlags(Qt::Window);
            m_vWidget->showFullScreen();
        }
    });
    btnLyt->addWidget(captureBtn,0,Qt::AlignLeft);
    btnLyt->addWidget(videotapingBtn,0,Qt::AlignLeft);
    btnLyt->addWidget(talkingBtn,0,Qt::AlignLeft);
    btnLyt->addWidget(closeAllBtn,0,Qt::AlignLeft);
    btnLyt->addStretch();
    btnLyt->addWidget(pollBtn,0,Qt::AlignRight);
    btnLyt->addWidget(windowingBtn,0,Qt::AlignRight);
    btnLyt->addWidget(fullWindowBtn,0,Qt::AlignRight);
    /*
	for (int i=0; i<static_cast<int>(Video::VideoControlEnum::Customize); ++i)
	{
		QPushButton* btn = new QPushButton(Video::VideoControlString[i], this);
		m_buttonGroup->addButton(btn, i);
        btnLyt->addWidget(btn);
	}
	connect(m_buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(ButtonJudge(int)));

	connect(m_addBtn, SIGNAL(clicked()), this, SLOT(AddCusBtn()));
    btnLyt->addWidget(m_addBtn);
	connect(m_editBtn, SIGNAL(clicked()), this, SLOT(EditCusBtn()));
	m_editBtn->setEnabled(false);
    btnLyt->addWidget(m_editBtn);
    */
}

void AllVideoWindow::CusBtnJudge()
{
	m_videoControl->SetStartIdx(0);
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	m_videoControl->SetVideoControlEnum(Video::VideoControlEnum::Customize);
    m_name = btn->objectName();
	m_videoControl->InitCusLayout(m_name);
}

void AllVideoWindow::AddCusBtn()
{
	m_addCusChannel->show();
}

/*
void AllVideoWindow::EditCusBtn()
{
	auto mapData = m_videoControl->GetMapData();
	if (mapData->size() == 0)
		return;
	m_editCusChannel = std::make_shared<EditCustomizeChannel>(mapData, m_nameSet);
	auto cusBtnFn = [this](const QString& name, const QString& rename, bool isDelete, bool isDelCom) {
		if ((rename == "" || rename == name) && !isDelete)
			return;
		auto iter = m_cusBtnMap.find(name);
		m_buttonLayout->removeWidget(iter->second);
		delete iter->second;
		m_cusBtnMap.erase(iter);

		if (!isDelete)
		{
			QPushButton* btn = new QPushButton(rename, this);
			m_cusBtnMap.insert(std::make_pair(rename, btn));
			m_buttonLayout->addWidget(btn);
			connect(btn, SIGNAL(clicked()), this, SLOT(CusBtnJudge()));
		}

		if (isDelCom)
		{
			m_editBtn->setEnabled(false);
		}
	};
	m_editCusChannel->SetCustomizeBtnFn(cusBtnFn);
	m_editCusChannel->show();
}
*/

void AllVideoWindow::SetFn()
{
    auto fullFn = [this](int idx) {
        if (m_vWidget->isFullScreen())
		{
            m_vWidget->setWindowFlags(Qt::SubWindow);
            m_vWidget->showNormal();
			int s = m_videoControl->GetStartIdx();
			m_videoControl->InitLayout(s, m_name);
            int i = m_videoControl->FindById(idx);
            if(i != -1)
            {
                auto win = m_videoControl->GetWindow(i);
                win->SetIsFull(false);
            }
		}
		else
		{
            m_vWidget->setWindowFlags(Qt::Window);
            m_vWidget->showFullScreen();
			int i = m_videoControl->FindById(idx);
			if (i != -1)
			{
				m_videoControl->FullVideo(i);
                auto win = m_videoControl->GetWindow(i);
                win->SetIsFull(true);
			}
		}
	};
	auto widgets = m_videoControl->GetAllWindow();
	foreach (auto widget , widgets)
	{
		widget->SetFullFn(fullFn);
        widget->SetCloseAllFn([this](){
            m_videoControl->CloseAll();
        });
	}
	auto cusBtnFn = [this](QString name) {
		if (m_nameSet->find(name) != m_nameSet->end())
			return;
        //m_editBtn->setEnabled(true);
        QPushButton* btn = new QPushButton;
        btn->setObjectName(name);
		m_nameSet->insert(name);
		m_cusBtnMap.insert(std::make_pair(name, btn));
        m_windowingWgt->AddCusBtn(btn);
		CusChannelData cusData;
		cusData.m_cusChannel = m_addCusChannel->GetCusChannel();
		cusData.m_mergeId = m_addCusChannel->GetMergeId();
		cusData.m_selRowCol = m_addCusChannel->GetSelRowCol();
		m_videoControl->AddCusData(name, cusData);
		connect(btn, SIGNAL(clicked()), this, SLOT(CusBtnJudge()));
	};
	m_addCusChannel->SetCustomizeBtnFn(cusBtnFn);
}

void AllVideoWindow::SetPicBtnAttr(QPushButton* btn, const QString& picPath, const QString& hoveredPath, const QString& tipStr)
{
    btn->setObjectName(tipStr);
    btn->setFixedSize(QPixmap(picPath).size());
    btn->setStyleSheet("QPushButton{background-image: url("+picPath+"); border: none;}"
                "QPushButton#" + tipStr + ":hover{background-image: url("+hoveredPath+"); border: none;}");
    btn->setToolTip(tipStr);
}

void AllVideoWindow::ReadCus()
{
    m_videoControl->ReadCusFromDatabase();
    auto cusMap = m_videoControl->GetMapData();
    for(auto i=cusMap->begin();i!=cusMap->end();++i)
    {
        QString name = i->first;
        if (m_nameSet->find(name) != m_nameSet->end())
            return;
        //m_editBtn->setEnabled(true);
        QPushButton* btn = new QPushButton;
        btn->setObjectName(name);
        m_nameSet->insert(name);
        m_cusBtnMap.insert(std::make_pair(name, btn));
        m_windowingWgt->AddCusBtn(btn);
        CusChannelData cusData;
        cusData.m_cusChannel = m_addCusChannel->GetCusChannel();
        cusData.m_mergeId = m_addCusChannel->GetMergeId();
        cusData.m_selRowCol = m_addCusChannel->GetSelRowCol();
        m_videoControl->AddCusData(name, cusData);
        connect(btn, SIGNAL(clicked()), this, SLOT(CusBtnJudge()));
    }
}

void AllVideoWindow::ButtonJudge(int buttonId)
{
	switch (buttonId)
	{
		case static_cast<int>(Video::VideoControlEnum::Full):
		{
			m_videoControl->SetStartIdx(0);
			resetShow(Video::VideoControlEnum::Full);
			m_videoControl->InitLayout();
			break;
	  }
		case static_cast<int>(Video::VideoControlEnum::Four):
		{
			m_videoControl->SetStartIdx(0);
			resetShow(Video::VideoControlEnum::Four);
			m_videoControl->InitLayout();
			break;
	  }
		case static_cast<int>(Video::VideoControlEnum::Six):
		{
			m_videoControl->SetStartIdx(0);
			resetShow(Video::VideoControlEnum::Six);
			m_videoControl->InitLayout();
			break;
		}
		case static_cast<int>(Video::VideoControlEnum::Eight):
		{
			m_videoControl->SetStartIdx(0);
			resetShow(Video::VideoControlEnum::Eight);
			m_videoControl->InitLayout();
			break;
		}
		case static_cast<int>(Video::VideoControlEnum::Nine) :
		{
			m_videoControl->SetStartIdx(0);
			resetShow(Video::VideoControlEnum::Nine);
			m_videoControl->InitLayout();
			break;
		}
		case static_cast<int>(Video::VideoControlEnum::Sixteen) :
		{
			m_videoControl->SetStartIdx(0);
			resetShow(Video::VideoControlEnum::Sixteen);
			m_videoControl->InitLayout();
			break;
		}
        case static_cast<int>(Video::VideoControlEnum::Twentyfive) :
        {
            m_videoControl->SetStartIdx(0);
            resetShow(Video::VideoControlEnum::Twentyfive);
            m_videoControl->InitLayout();
            break;
        }
		default:
			break;
	}
}
