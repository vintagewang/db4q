/*
 * $Id: Var.h 1 2012-01-17 02:31:57Z  $
 */
#ifndef LWPR_VAR_H__
#define LWPR_VAR_H__

template <class T>
class Var
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

	Var() : m_p(0) {}
	Var(T *p) : m_p(p) {}
	Var(const Var<T>& v) : m_p(static_cast<T*>(T::Duplicate(v))) {}
	~Var()
	{
		T::Release(m_p);
	}

	void operator=(T *p)
	{
		T::Release(m_p);
		m_p = p;
	}

	void operator=(const Var<T>& v)
	{
		T::Release(m_p);
		m_p = static_cast<T*>(T::Duplicate(v));
	}

	void operator+=(T *p)
	{
		T::Release(m_p);
		m_p = static_cast<T*>(T::Duplicate(p));
	}

	static T *Duplicate(T *p)
	{
		return static_cast<T*>(T::Duplicate(p));
	}

	T *ptr()
	{
		return m_p;
	}

	T *retn()
	{
		T *p = m_p;
		m_p = 0;
		return p;
	}
private:
	T   *m_p;
};

#define DECLAREVAR(T)   typedef Var<T> T ## _var;   \
                        typedef T* T ## _ptr;

#define DECLAREPTR(T)   typedef T* T ## _ptr;

#endif
