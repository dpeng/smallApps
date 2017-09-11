
#ifndef SINGLETON_H
#define SINGLETON_H

template <class T> 
class Singleton  
{
public:
	static T* instance()
	{ 
		if ( 0 == s_instance )
		{
			s_instance = new T;
		}

		return s_instance;
	}

	static void unInstance()
	{
		if ( 0 != s_instance )
		{
			delete s_instance;
			s_instance = 0;
		}
	}

protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	Singleton(const Singleton<T> &);
	Singleton<T>& operator= (const Singleton<T> &);

	static T* s_instance;
};

template <class T>
T* Singleton<T>::s_instance = 0;

#endif
