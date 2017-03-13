/**
 * @brief own implenation of a Principal Component Analysis (PCA)
 *
 * class of Principal component analysis
 *
 * Author:	Stefan, Moebius (mail@stefanmoebius.de)
 *
 * Date:	2011-05-15
 *
 * Usage:
 * THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 */
#ifndef _PCA_H_
#define _PCA_H_

#include <math.h>
#include <string.h>
#include <stdexcept>
#include <vector>

using namespace std;
/*
namespace Sys
{
	namespace Math
	{
		namespace Transformation
		{
			*/
			class PCA
			{
			public:
				static void calcPCA(const float *samples, int numSamples, float* pVecs, int numComponents, int dimension);
			private:
				static void getMean(const float *samples, float *meanVec, int numSamples, int dimension);
				static void subtractVector(float *samples, const float *vec, int numSamples, int dimension);
				static void getComponent(const float *samples, int numSamples, int dimension, float *tmpVec, float *outPCAVec, float &maxlength2);
				static void removeComponent(float *samples, int numSamples, float* pVec, float length2, int dimension);
			};

			/*
		}
	}
}*/
#endif
