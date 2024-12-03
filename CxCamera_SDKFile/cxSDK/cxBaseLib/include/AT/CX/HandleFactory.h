/*************************************************************************************
@brief definition of generic HandleFactory used in cx libraries.
@file : HandleFactory.h
@package : CxBase library
@copyright (c) 2017, Automation Technology GmbH.

@version 01.09.2017 initial version

*************************************************************************************/
#pragma once
#ifndef CX_HANDLEFACTORY_H_INCLUDED
#define CX_HANDLEFACTORY_H_INCLUDED

#include <assert.h>
#include <list>
#include <mutex>
#include <algorithm>

namespace AT {
	namespace cx {

	/** HandleFactory template

		Class is thread safe and uses a mutex for protecting its handle container.
		Typically this class is used as singleton, all items should have been released before the factory gets deleted.
		The implementation does not automatically release objects when destructor is called.
		For creating a unique handle from object a cast operator from object type T to handle type H must exist.
	*/
	template<typename T, typename H=void*>
	class HandleFactory {
	public:
		typedef T object_type;
		typedef H handle_type;
		typedef std::list<handle_type> HandleContainer;

		HandleFactory() {}
		~HandleFactory() {}

		bool isEmpty() const
		{
			std::lock_guard<std::mutex> lck(m_mtx);
			return m_handles.empty();
		}

		handle_type createHandle(object_type* obj)
		{
			handle_type h = obj;
			std::lock_guard<std::mutex> lck(m_mtx);
			if(std::find(m_handles.begin(), m_handles.end(), h) == m_handles.end())
				m_handles.push_front(h);
			return h;
		}

		/** deletes the given handle and return the linked object.
			returns NULL if handle was not found.
		*/
		object_type* deleteHandle(handle_type h)
		{
			object_type* obj = NULL;
			if (CX_INVALID_HANDLE != h)
			{
				std::lock_guard<std::mutex> lck(m_mtx);
				typename HandleContainer::iterator it = std::find(m_handles.begin(), m_handles.end(), h);
				if (it != m_handles.end())
				{
					obj = reinterpret_cast<object_type*>(*it);
					m_handles.erase(it);
				}
			}
			return obj;
		}

		/** deletes the last handle and returns the linked object.
			returns NULL if handle container is empty.

			\code{.cpp}
				while(auto obj=factory.deleteBack())
					delete obj;
			\endcode
		*/
		object_type* deleteBack()
		{
			object_type* obj = NULL;
			std::lock_guard<std::mutex> lck(m_mtx);
			if (!m_handles.empty())
			{
				obj = reinterpret_cast<object_type*>(m_handles.back());
				m_handles.pop_back();
			}
			return obj;
		}

		/** return Object for given handle.
			returns NULL if handle was not found.
		*/
		object_type* getObj(handle_type h)
		{
			object_type* obj = NULL;
			if (CX_INVALID_HANDLE != h)
			{
				std::lock_guard<std::mutex> lck(m_mtx);
				typename HandleContainer::iterator it = std::find(m_handles.begin(), m_handles.end(), h);
				if (it != m_handles.end())
					obj = reinterpret_cast<object_type*>(*it);
			}
			return obj;
		}

	private:
		std::mutex m_mtx;           // mutex for handle container
		HandleContainer m_handles;
	};

	} // namespace cx
}	// namespace AT
#endif
