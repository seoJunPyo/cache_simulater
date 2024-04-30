#pragma once
#include <iostream>
#include "LinkedList.h"

class CacheSimulator
{
	static const int NWAY = 8;
	static const int MAX_CACHELISE = 512;
private:
	struct CacheLine
	{
		int _tag;
		bool _valid = false;
		int _miss_cnt = 0;
		int _hit_cnt = 0;
	};

	struct Cache
	{
		CacheLine _line[NWAY];
		int _valid_line = 0;
		int _access_cnt = 0;
		int _conflict_cnt = 0;
 	};

	struct CacheNode
	{
		CacheLine _line;
		int idx;
	};

	void Swap(CacheLine* line1, CacheLine* line2);
	void Init_Line(CacheLine* line, int tag);
	void Flush_Line(const CacheLine* line, int idx);
	void Print_Result();

public :
	~CacheSimulator();
	void CacheHit(void *addr);

private :
	Cache _cache[MAX_CACHELISE / NWAY];
	CList<CacheNode*> _list;
};