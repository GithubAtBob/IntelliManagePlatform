#ifndef ELECTRONICMAP_H
#define ELECTRONICMAP_H

#include <QWidget>

class QWebEngineView;
class ElectronicMap : public QWidget
{
    Q_OBJECT

public:
    ElectronicMap(int x, int y, QWidget *parent = Q_NULLPTR);

private:
    void IniUi(int x, int y);

    QWebEngineView* m_view;

};

#endif // ELEMENTMAP_H
