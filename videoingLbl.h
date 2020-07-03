#ifndef VIDEOINGLBL_H
#define VIDEOINGLBL_H

#include <QLabel>
class videoingLbl : public QLabel
{
    Q_OBJECT
public:
    videoingLbl(QWidget *parent = Q_NULLPTR);

    void SetIsShow(bool isShow);

private:
    void InitVideoingLbl();

    bool m_isShow = false;

    bool m_isTwinkle = false;

private slots:
    void TwinkleSlot();
};


#endif // VIDEOINGLBL_H
