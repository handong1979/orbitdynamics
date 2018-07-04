/********************************************************************
	created:	2009/04/20
	created:	20:4:2009   11:57
	filename: 	Delay.h
	file ext:	h
	author:		韩冬
	
	purpose:    定义通用延迟类，用一个有最大长度的"先入先出"队列
	            保存延迟数据
*********************************************************************/
#pragma once

#include <deque>
#include <stdexcpt.h>

// TEMPLATE CLASS queue
template<class _Ty,	class _Container = std::deque<_Ty> >	class delay
{	// FIFO queue implemented with a container
public:
	typedef _Container container_type;
	typedef typename _Container::value_type value_type;
	typedef typename _Container::size_type size_type;
	typedef typename _Container::reference reference;
	typedef typename _Container::const_reference const_reference;

	delay()
		:_length(0),c()
	{	// construct with empty container
	}

	delay(int n)
		:_length(n),c()
	{ // construct with empty queue and queue max length is n
	}
	
	void SetLength(size_type num)
	{ // set queue max length
		if(num<1)
			throw length_error("delay::SetLength(): length must be 1 at least");

		// remove some elements to meet the new length
		if(_length>num) 
		{
			for(size_type i=0;i<_length-num;i++)
				pop();
		}

		_length = num;
	}

	int Length() const
	{ // get queue max length
		return _length;
	}

	bool empty() const
	{	// test if queue is empty
		return (c.empty());
	}

	bool full()
	{  // test if queue is full
		return size() == _length;
	}

	size_type size() const
	{	// return length of queue
		return (c.size());
	}

	reference front()
	{	// return first element of mutable queue
		return (c.front());
	}

	const_reference front() const
	{	// return first element of nonmutable queue
		return (c.front());
	}

	reference back()
	{	// return last element of mutable queue
		return (c.back());
	}

	const_reference back() const
	{	// return last element of nonmutable queue
		return (c.back());
	}

	void push(const value_type& _Val)
	{	// insert an element, if size is bigger than
		// max length(_length), pop an element. 
		c.push_back(_Val);
		if( c.size() > _length )
			c.pop_front();
	}

	void pop()
	{	// erase element
		c.pop_front();
	}

private:
	// 延时周期数(队列最大长度)
	size_type _length;
	// FIFO队列
	_Container c;	// the underlying container
};
