#pragma once

#include <list>
#include <map>

class PMemClassCTR
{
public:
	typedef	std::list<PMemClass>					PMemClassList;
	typedef PMemClassList::iterator					PMemClassListIt;
	typedef std::map<PMemClass, PMemClassListIt>	PMemClassListItMap;
	typedef PMemClassListItMap::iterator			PMemClassListItMapIt;
private:
	PMemClassListItMap _list_it_map;
public:
	PMemClassList _list;
	PMemClassCTR()
	{
		//
	}

	~PMemClassCTR()
	{
		//
	}

	void push_back(PMemClass ptr)
	{
		static std::list<PMemClass>::iterator value;
		if (_list_it_map.find(ptr) == _list_it_map.end())
		{
			try
			{
				_list.push_back(ptr);
			}
			catch (...)
			{
				logprintf("[Memory plugin] Out of memory: \"PMemClassCTR::push_back(PMemClass ptr)\"");
				throw -1;
			}
			value = _list.end();
			try
			{
				_list_it_map[ptr] = (--value);
			}
			catch (...)
			{
				_list.pop_back();
				logprintf("[Memory plugin] Out of memory: \"PMemClassCTR::push_back(PMemClass ptr)\"");
				throw -1;
			}
		}
	}

	void erase(PMemClass ptr)
	{
		static std::list<PMemClass>::iterator *value;
		try
		{
			_list.erase(*(value = &(_list_it_map.at(ptr))));
			(*value) = _list.end();
		}
		catch (...)
		{
			logprintf("[Memory plugin] Unknown: \"PMemClassCTR::erase(PMemClass ptr)");
			throw -1;
		}
	}

	void clear()
	{
		_list_it_map.clear();
		_list.clear();
	}
		
	bool exists(PMemClass ptr)
	{
		return (_list_it_map.find(ptr) == _list_it_map.end()) ? false : (_list_it_map[ptr] != _list.end());
	}

	void clean_up()
	{
		for (PMemClassListItMapIt _it = _list_it_map.begin(); _it != _list_it_map.end(); ++_it)
		{
		map_foreach_begin:
			if (_it->second == _list.end())
			{
				_list_it_map.erase(_it);
				if ((_it = _list_it_map.begin()) == _list_it_map.end()) break;
				goto map_foreach_begin;
			}
		}
	}
};