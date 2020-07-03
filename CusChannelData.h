#ifndef CUSCHANNELDATA_H
#define CUSCHANNELDATA_H

#include <utility>
#include <set>

struct CusChannelData
{
	//自定义通道
	std::pair<int, int> m_cusChannel;

	//合并编号
	std::set<int> m_mergeId;

	//选中通道几行几列
	std::pair<int, int> m_selRowCol;
};

#endif
