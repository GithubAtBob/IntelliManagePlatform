#ifndef CHANNELCHOOSE_H
#define CHANNELCHOOSE_H

#include <QWidget>
#include <set>

class AllVideoWindow;
class QPushButton;
class AddCustomizeChannel;
class QButtonGroup;
class ChannelChoose : public QWidget
{
    Q_OBJECT

public:
    ChannelChoose(std::shared_ptr<AddCustomizeChannel> addCus, QWidget *parent = nullptr);

    void AddCusBtn(QPushButton* btn);
private:
    void  InitUi();

    //设置图片按钮属性
    void SetPicBtnAttr(QPushButton* btn, const QString& picPath);

    AllVideoWindow* m_prtPtr;

    std::shared_ptr<AddCustomizeChannel> m_addCus;

    QPushButton* m_addCusBtn;

    int m_pos = 20;

    QButtonGroup* m_btnGrp;
};

#endif // CHANNELCHOOSE_H
