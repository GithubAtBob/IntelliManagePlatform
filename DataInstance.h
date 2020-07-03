#ifndef DATAINSTANCE_H
#define DATAINSTANCE_H

#include "DataType.h"
#include <QString>
#include <map>
#include <windows.h>
#include <set>
#include <vector>

class QStandardItemModel;
class QWidget;
class QStandardItem;
enum class CamStatusEnum;
struct TREE_ITEM_INFO;
class DataInstance
{
public:
    //获取单例
    static DataInstance* GetInstance();

    DataInstance();

/****设备树****/
    //获取设备树
    QStandardItemModel* GetDevTreeMdl();

    //设置设备树父指针
    void SetDevTreePrtPtr(QWidget* prtPtr);

    //初始化设备树
    void IniDevTree();

    //加载摄像头图标
    void LoadCamIcon(QStandardItem* item, bool isChecked);

    //设置当前选中节点
    void SetCurrItem(QStandardItem* item);

    //获取当前选中节点
    QStandardItem* GetCurrItem();

    //根据名字获取节点
    QStandardItem* GetItemByName(const QString& name);
/************/

/****播放数据****/
    //添加句柄返回值键值对
    void AddhwndRetMap(HWND hwnd, long ret);

    //获取返回值
    long GetRet(HWND hwnd, bool isElemap = false);

    void SetElemapRet(long ret);

    //获取所有返回值
    std::vector<long> GetAllRet();

    //移除句柄返回值键值对
    void RemovehwndRetMap(HWND hwnd);

    //清空句柄返回值键值对
    void ClearhwndRetMap();

    //获取播放视频数目
    int GetPlayingNum();

    //初始化空闲池
    void IniFreePool(int size);

    //空闲池移除项
    void RemoveItemFreePool(int idx);

    //空闲池添加项
    void AddItemFreePool(int idx);

    //获取空闲池首项
    int GetFirstFree();

    //添加句柄树节点键值对
    void AddhwndItemMap(HWND hwnd, QStandardItem* item);

    //移除句柄树节点键值对
    void RemovehwndItemMap(HWND hwnd);

    //获取树节点
    QStandardItem* GetItem(HWND hwnd);

    //获取树节点信息
    pCMPInfo GetItemInfo(HWND hwnd, bool isElemap = false);

    //电子地图设置树节点信息
    void SetItemInfo(pCMPInfo info);

    //根据节点找HWND
    HWND GetHwndByItem(QStandardItem* item);

    //清除句柄树节点键值对
    void ClearhwndItemMap();

    //设置选中播放通道句柄
    void SetSelHwnd(HWND hwnd);

    //获取选中播放通道句柄
    HWND GetSelHwnd();
/************/

/****云台数据****/
    //设置云台速度
    void SetPantiltSpeed(int speed);

    //获取云台速度
    int GetPantiltSpeed();
/************/

private:
    static DataInstance* me;

private:
/****设备树****/
    //添加节点
    void AddItem(QStandardItem* item, QStandardItem* parent);

    //设置节点图标
    void SetIcon(QStandardItem* item, const QString& iconPath);

    //设置节点数据
    void SetData(QStandardItem* item, TREE_ITEM_INFO* itemInfo);

    //初始化节点
    void IniNode(QStandardItem* item, QStandardItem* parent, const QString& iconPath, TREE_ITEM_INFO* itemInfo);
/************/

    //设备树
    QStandardItemModel* m_devTreeMdl;

    //句柄返回值键值对
    std::map<HWND, long> m_hwndRetMap;

    //电子地图句柄返回值
    long m_electronicMapRet = -1;

    //空闲池
    std::set<int> m_freePool;

    //句柄树节点键值对
    std::map<HWND, QStandardItem*> m_hwndItemMap;

    //电子地图iteminfo
    pCMPInfo m_elemapItemInfo = nullptr;

    //选中播放通道句柄
    HWND m_selPlayingHwnd = NULL;

    //云台速度
    int m_pantiltSpeed = 0;

    //当前选中节点
    QStandardItem* m_currItem = nullptr;
};

#endif // DATAINSTANCE_H
