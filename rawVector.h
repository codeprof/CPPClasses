/**
 * @brief Helper class for management of dynamic raw arrays
 *
 * implementation of dynamic raw arrays
 *
 * Author:	Stefan Moebius (mail@stefanmoebius.de)
 *
 * Date:	2011-05-19
 *
 * Licence: Released to the PUBLIC DOMAIN
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 */
#ifndef _RAWVECTORCLASS_H_
#define _RAWVECTORCLASS_H_
#include <stdlib.h>
#include <string.h>
#include <stdexcept>

namespace Sys
{
	namespace Array
	{
		//Array helper class
		/**
		 * Helper class for management of dynamic raw arrays.
		 * In contrast to the vector class this class DOES NOT CALL THE CONSTRUCTOR for elements.
		 * Thus it should be used only with primitive types like int, float, double...
		 */
		template<class T> class rawVector //used for dynamic memory allocation
		{
		public:
			rawVector();
			rawVector(int dim);
			rawVector(T *src, int dim);
			rawVector(const rawVector&);
			~rawVector();
			void operator=(const rawVector&);
			inline T& operator[](int idx);
			inline int getSize();
			inline T* getPointer();
			void clear();
			void setVector(int offset, const T *vec, int dim);
		private:
			T* pointer;
			int sz;
		};

		#pragma region "Public Methods of class rawVector"
		/**
		 * Initializes a rawVector with NULL pointer
		 * @return - none
		 */
		template<class T> rawVector<T>::rawVector()
		{
			pointer = NULL;
			sz = 0;
		}
		/**
		 * Initializes a rawVector with the declared dimension
	     * @throw std::bad_alloc - if memory allocation fails	
		 */
		template<class T> rawVector<T>::rawVector(int dim)
		{
			pointer = (T*)calloc(dim, sizeof(T));
			if (pointer)
			{
				sz = dim;
			}
			else
			{
				throw std::bad_alloc();
			}
		}
		/**
		 * Initializes a rawVector with a copy of the declared pointer with declared dimension.
	     * @throw std::invalid_argument - if invalid pointer or size
	     * @throw std::bad_alloc        - if memory allocation fails
		 */
		template<class T> rawVector<T>::rawVector(T *src, int dim)
		{
			if (dim > 0)
			{
				if ((src != NULL))
				{
					pointer = (T*)malloc(dim * sizeof(T));
					if (pointer != NULL)
					{
						memcpy(pointer, src, dim * sizeof(T));
						sz = dim;
					}
					else
					{
						throw std::bad_alloc();
					}
				}
				else
				{
					throw std::invalid_argument("invalid pointer");
				}
			}
			else
			{
				throw std::invalid_argument("invalid size");
			}
		}
		/**
		 * create a copy of the declared array
		 * @return - none
		 */
		template<class T> rawVector<T>::rawVector(const rawVector<T>& arr)
		{
			//This must be done first (because clear will be called)
			pointer = NULL;
			sz = 0;
			(*this) = arr;
		}
		/**
		 * Frees reserved memory
		 * @return - none
		 */
		template<class T> rawVector<T>::~rawVector()
		{
			clear();
		}
		/**
		 * copy content of the array
		 * @return					- none
	     * @throw std::bad_alloc	- if memory allocation fails
		 */
		template<class T> void rawVector<T>::operator=(const rawVector<T>& arr)
		{
			clear(); //Make sure old memory is freed correctly
			if (arr.pointer)
			{
				this->pointer = (T*)malloc(arr.sz * sizeof(T));
				if (this->pointer)
				{
					memcpy(this->pointer, arr.pointer, arr.sz * sizeof(T));
					this->sz = arr.sz;
				}
				else
				{
					throw std::bad_alloc();
				}
			}
			else
			{
				this->pointer = NULL;
				this->sz = 0;
			}
		}
		/**
		 * Returns the entry at the declared offset
		 * @return reference to element of type T
		 */		
		template<class T> T& rawVector<T>::operator[](int idx)
		{
			return pointer[idx];
		}

		/**
		 * Returns the number of elements in the array
		 * @return integer
		 */
		template<class T> int rawVector<T>::getSize()
		{
			return sz;
		}
		/**
		 * Returns a pointer to the first element in the array
		 * @return pointer to first element
		 */
		template<class T> T* rawVector<T>::getPointer()
		{
			return pointer;
		}
		/**
		 * Removes all elements from the array
		 * @return none
		 */
		template<class T> void rawVector<T>::clear()
		{
			if (pointer)
			{
				free(pointer);
				pointer = NULL;
			}
			sz = 0;
		}
		/**
		 * Writes the declared vector to the declared position in the array.
		 * If neccessary the allocated memory region will be increased.
		 * @param arr				- reference to the array
		 * @param offset			- offset in sizeof(T) steps
		 * @param *vec				- pointer to the vector which should be copied
		 * @param dim				- number of elements of the vector which should be copied
	     * @throw std::bad_alloc	- if memory allocation fails	
		 * @return void
		 */
		template<class T> void rawVector<T>::setVector(int offset, const T *vec, int dim)
		{
			bool result = false;
			T *ptr;
			int newSize;
			if (this->pointer)
			{
				if (offset + dim > this->sz)
				{
					//array is not big enough at the moment, so we have to resize it
					newSize = (dim + offset);
					ptr = (T*)realloc((void*)this->pointer, newSize * sizeof(T));
					if (ptr)
					{
						this->pointer = ptr;
						ptr += offset;
						memcpy(ptr, vec, sizeof(T) * dim);
						this->sz = newSize;
						result = true;
					}
				}
				else
				{
					//array is already big enough
					ptr = this->pointer + offset;
					memcpy(ptr, vec, dim * sizeof(T));
					result = true;
				}
			}
			else
			{
				//array does not exist, so allocate new array
				newSize = (dim + offset);
				ptr = (T*)malloc(newSize * sizeof(T));
				if (ptr)
				{
					this->pointer = ptr;
					ptr += offset;
					memcpy(ptr, vec, sizeof(T) * dim);
					this->sz = newSize;
					result = true;
				}
			}
			if (result == false)
			{
				throw std::bad_alloc();
			}
		}
		#pragma endregion
	}
}
#endif
