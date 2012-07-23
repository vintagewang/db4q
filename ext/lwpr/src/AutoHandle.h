/*
 * $Id: AutoHandle.h 1 2012-01-17 02:31:57Z  $
 */
#ifndef LWPR_AUTO_HANDLE_H__
#define LWPR_AUTO_HANDLE_H__

/**
 * AutoHandle主要用来管理动态申请的局部对象或者内存
 */
template <class T>
class AutoHandle
{
public:
	T *operator->() const
	{
		return m_p;
	}

	operator T*() const
	{
		return m_p;
	}

	AutoHandle() : m_p(0) {}
	AutoHandle(T *p) : m_p(p) {}
	~AutoHandle()
	{
		delete m_p;
	}

	void operator=(T *p)
	{
		m_p = p;
	}

	T *ptr()
	{
		return m_p;
	}

private:
	T   *m_p;
};

#endif
