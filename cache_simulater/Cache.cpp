#include "Cache.h"

void CacheSimulator::Swap(CacheLine* line1, CacheLine* line2)
{
	if (line1 == line2)
		return;

	CacheLine temp;

	temp = *line1;
	*line1 = *line2;
	*line2 = temp;
}

//------------------------------------------------------------------------
// ĳ�� ���� ������ ����Ʈ�� �����Ѵ�.
//------------------------------------------------------------------------
void CacheSimulator::Flush_Line(const CacheLine* line, int idx)
{
	CList<CacheNode*>::iterator iter;
	for (iter = _list.begin(); iter != _list.end(); ++iter)
	{
		if ((*iter)->_line._tag == line->_tag && (*iter)->idx == idx)
		{
			(*iter)->_line._hit_cnt += line->_hit_cnt;
			(*iter)->_line._miss_cnt += line->_miss_cnt;

			return;
		}
	}

	_list.push_back(new CacheNode{ *line, idx });
}

void CacheSimulator::Init_Line(CacheLine* line, int tag)
{
	line->_tag = tag;
	line->_hit_cnt = 0;
	line->_miss_cnt = 1;
	line->_valid = true;
}

void CacheSimulator::CacheHit(void *addr)
{
	int idx = ((int)addr >> 6) & 0x3f;
	int tag = (int)addr >> 12;
	Cache* cache = &_cache[idx];
	cache->_access_cnt++;

	//------------------------------------------------------------------------
	// Hit ó��.
	//------------------------------------------------------------------------
	for (int i = 0; i < cache->_valid_line; i++)
	{
		CacheLine* line = &cache->_line[i];
		if (line->_tag == tag)
		{
			line->_hit_cnt++;

			//------------------------------------------------------------------------
			// �ֱٿ� ������ ������ ���� �ڷ� ������.
			//------------------------------------------------------------------------
			for (int j = i; j < cache->_valid_line - 1; j++)
				Swap(&cache->_line[j], &cache->_line[j + 1]);

			return;
		}
	}

	//------------------------------------------------------------------------
	// Miss�̰�, Way�� ��� ������� ��.
	// ���� ������� ���� ĳ�� ������ �����.
	//------------------------------------------------------------------------
	if (cache->_valid_line == NWAY)
	{
		cache->_conflict_cnt++;

		CacheLine* line = &cache->_line[0];
		Flush_Line(line, idx);
		Init_Line(line, tag);
		return;
	}

	//------------------------------------------------------------------------
	// Miss�̰�, Way�� ������ ��.
	//------------------------------------------------------------------------
	Init_Line(&cache->_line[cache->_valid_line], tag);
	cache->_valid_line++;

	return;
}


CacheSimulator::~CacheSimulator()
{
	for (int i = 0; i < MAX_CACHELISE / NWAY; i++)
	{
		Cache* cache = &_cache[i];
		for (int j = 0; j < cache->_valid_line; j++)
			Flush_Line(&cache->_line[j], i);
	}

	Print_Result();
}

void CacheSimulator::Print_Result()
{
	CList<CacheNode*>::iterator iter;
	for (iter = _list.begin(); iter != _list.end(); ++iter)
	{
		CacheLine* line = &(*iter)->_line;
		int idx = (*iter)->idx;
		int tag = line->_tag;

		std::cout << "----------------------------------\n";
		std::cout << "addr : 0x" << (int*)((idx << 6) | (tag << 12)) << "\n";
		std::cout << "miss cout : " << line->_miss_cnt << "\n";
		std::cout << "hit cout : " << line->_hit_cnt << "\n";
		std::cout << "hit rate : " << ((double)line->_hit_cnt / (line->_hit_cnt + line->_miss_cnt)) * 100 << "%\n";
		std::cout << "----------------------------------\n\n";
	}
}