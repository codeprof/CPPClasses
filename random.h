/**
* @brief class to create random integer and float numbers
*
* implementation of a random number generator
* float point IEEE754 standard must be supported!
* The generated numbers are not cryptographic secure!
*
* Author:	Stefan Moebius (mail@stefanmoebius.de)
*
* Date:	2011-05-19
*
* Usage:
* THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY
* KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
* PARTICULAR PURPOSE.
*/
#ifndef _RANDOMNUMGENERATOR_H_
#define _RANDOMNUMGENERATOR_H_

#define USE_RANDOM_BUFFER

#include <time.h>
//Visual C++ does not support architecture independant "int32_t" data type before VS 2010
#ifdef _MSC_VER
typedef unsigned int uint32_t;  //Visual C++ also uses "unsigned int" instead of "unsigned __int32" 
#else
#include <stdint.h>
#endif

namespace Sys
{
	namespace Misc
	{
		/**
		* class to create random integer and float numbers
		* The generated numbers are not cryptographically secure!
		*/
		class Random
		{
		public:
			inline Random();
			inline Random(uint32_t seed);
			inline void seed(uint32_t seed);
			inline uint32_t getUInt32();
			inline float getFloat();
			inline float getFloat2();

		private:
#ifdef USE_RANDOM_BUFFER
			uint32_t offsetA, offsetB;
			static const uint32_t BLOCKSIZEA = 2137, BLOCKSIZEB = 1381;
			static const uint32_t rndBaseA[BLOCKSIZEA], rndBaseB[BLOCKSIZEB];
#else
			uint32_t offset;
#endif
		};
	}
}
#endif
