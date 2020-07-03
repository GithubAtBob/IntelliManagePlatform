#ifndef TREEITEMINFO_H
#define TREEITEMINFO_H

#include <QVariant>

enum class NodeTypeEnum
{
    root = 0,
    group = 1,
    dev = 2
};

enum class CamTypeEnum
{
    gun = 0,
    hemisphere = 8
};

enum class CamStatusEnum
{
    online = 0,
    offline = 1,
    playing = 2
};

//树节点信息
struct TREE_ITEM_INFO
{
    int NodeType;    //节点类型：分组、设备等
    void* dev;       //节点信息：分组信息、设备信息等
    bool isPlay;
    TREE_ITEM_INFO()
    {
        NodeType = 0;
        dev = NULL;
        isPlay = false;
    }
    void operator = (const TREE_ITEM_INFO & obA)
    {
        this->NodeType = obA.NodeType;
        this->dev = obA.dev;
        this->isPlay = obA.isPlay;
    }
};
Q_DECLARE_METATYPE(TREE_ITEM_INFO*)

#endif // TREEITEMINFO_H
