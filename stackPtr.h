
#ifndef _STACKPTR_H_
#define _STACKPTR_H_
//#define _USE_STACKPTR_ALLOWALLOCATOR

#include <stddef.h>   // for size_t

#ifdef _USE_STACKPTR_ALLOWALLOCATOR
#include <memory>
using namespace std;
/**
* StackPtr can be used to dynamically allocate memory.
* To improve allocation speed it reserves "maxStack" elements on the Stack.
* If the requested amout is smaller or equal to "maxStack" a pointer to the stack is delivered.
* If the amount is bigger then the memory will be allocated on the heap.
* StackPtr automatically frees the memory in the destructor.
*/
template <class T, size_t maxStack, class _myAlloc = allocator<T> > class StackPtr
{
public:
	/**
	* Allocates memory of the declared amout on the stack or if there is not enought room on the heap
	* @param size           - The number of elements of type T
	* @throw std::bad_alloc - if allocation fails
	*/
	inline StackPtr(size_t size)
	{
		sz = size;
		ptr = (size > maxStack) ? alloc.allocate(size) : &data[0];
	}
	/**
	* Free reserved memory
	*/
	inline ~StackPtr()
	{
		if (ptr != &data[0])
			alloc.deallocate(ptr, sz);
	}
	/**
	* Provides random access to elements
	*/
	inline T& operator[](ptrdiff_t idx)
	{
		return ptr[idx];
	}
	/**
	* Provides the size
	*/
	inline size_t size()
	{
		return sz;
	}
private:
	//disallow copy and assign
	StackPtr(const StackPtr&);               
	void operator=(const StackPtr&);

	T data[maxStack];
	_myAlloc::pointer ptr;
	_myAlloc alloc;
	size_t sz;
};

#else

/**
* StackPtr can be used to dynamically allocate memory.
* To improve allocation speed it reserves "maxStack" elements on the Stack.
* If the requested amount is smaller or equal to "maxStack" a pointer to the stack is delivered.
* If the amout is bigger then the memory will be allocated on the heap.
* StackPtr automatically frees the memory in the destructor.
*/
template <class T, size_t maxStack> class StackPtr
{
public:
	/**
	* Allocates memory of the declared amout on the stack or if there is not enought room on the heap
	* @param size           - The number of elements of type T
	* @throw std::bad_alloc - if allocation fails
	*/
	inline StackPtr(size_t size)
	{
		ptr = (size > maxStack) ? new T[size] : &data[0];
	}
	/**
	* Free reserved memory
	*/
	inline ~StackPtr()
	{
		if (ptr != &data[0])
			delete [] ptr;
	}
	/**
	* Provides random access to elements
	*/
	inline T& operator[](ptrdiff_t idx)
	{
		return ptr[idx];
	}
private:
	//disallow copy and assign
	StackPtr(const StackPtr&);               
	void operator=(const StackPtr&);

	T data[maxStack];
	T* ptr;
};

#endif
#endif
