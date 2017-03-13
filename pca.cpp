#include "pca.h"
namespace Sys
{
	namespace Math
	{
		namespace Transformation
		{

			void PCA::getMean(const float *samples, float *meanVec, int numSamples, int dimension)
			{
				if ((samples != NULL) && (meanVec != NULL) && (numSamples > 0) && (dimension > 0))
				{
					for (int i = 0; i < dimension; i++)
					{
						float mean = 0.0f;
						for (int j = 0; j < numSamples; j++)
						{
							mean += samples[j * dimension + i];
						}
						meanVec[i] = mean / (float)numSamples;
					}
				}
			}
			/**
			 * subtracts the mean from all sample vectors
			 * @param samples	 -  Pointer to an array of sample vectors. The number of components must fit the delcared "dimension".  
			 * @param numSamples -  Number of sample vectors in "samples"
			 * @param dimension  -  Number of dimensions of each vector
			 * @return           -  none
			 */
			void PCA::subtractVector(float *samples, const float *vec, int numSamples, int dimension)
			{
				if ((samples != NULL) && (vec != NULL) && (numSamples > 0) && (dimension > 0))
				{
					for (int i = 0; i < dimension; i++)
					{
						for (int j = 0; j < numSamples; j++)
						{
							samples[j * dimension + i] -= vec[i];
						}
					}
				}
			}
			/**
			 * Some kind of implentation of a Principal component analysis
			 * Here the principal component is calculated as follows:
			 * On each sample vector we add all other sample vectors multiplied with the dot product of these two vectors.
			 * @param samples	 -  Pointer to an array of sample vectors. The number of components must fit the delcared "dimension".  
			 * @param numSamples -  Number of sample vectors in "samples"
			 * @param dimension  -  Number of dimensions of each sample vector
			 * @param tmpVec	 -  pointer to a vector of the same dimension like the vectors in the smaple array which is declared in "dimension".
			 * @param outPCAVec  -  pointer to a vector (which will be filled with the "principal component") of the same dimension like the vectors in the smaple array which is declared in "dimension".
			 * @param maxlength2 -  Parameter which will be filled with the quadratic length of the "principal component"
			 * @return           -  none
			 */
			void PCA::getComponent(const float *samples, int numSamples, int dimension, float *tmpVec, float *outPCAVec, float &maxlength2)
			{
				if ((samples != NULL) && (tmpVec != NULL) && (outPCAVec != NULL) && (numSamples != NULL) && (dimension != NULL))
				{
					//make sure temorary vector is initlialized  with 0.0
					for (int i = 0; i < dimension; i++)
					{
						tmpVec[i] = 0.0f;
					}

					for (int j = 0; j < numSamples; j++)
					{
						float length2 = 0.0f;
						for (int i = 0; i < numSamples; i++)
						{
							//Calculate dot product between the two sample vectors
							float dot = 0.0f;
							for (int dim = 0; dim < dimension; dim++)
							{
								dot += samples[j * dimension + dim] *  samples[i * dimension + dim];
							}
							for (int dim = 0; dim < dimension; dim++)
							{
								tmpVec[dim] += dot * samples[i * dimension + dim];
							}
						}
						//Calcuate quadratic length of the vector
						for (int dim = 0; dim < dimension; dim++)
						{
							length2 +=  tmpVec[dim] * tmpVec[dim];
						}
						if (length2 >= maxlength2)
						{
							//overwrite vector
							for (int dim = 0; dim < dimension; dim++)
							{
								outPCAVec[dim] =  tmpVec[dim];
							}
							maxlength2 = length2;
						}
					}
				}
			}
			/**
			 * removes the declared component for all sample vectors
			 * @param samples	 -  Pointer to an array of sample vectors. The number of components must fit the delcared "dimension".  
			 * @param numSamples -  Number of sample vectors in "samples"
			 * @param pVec		 -  Pointer to the component which should be removed
			 * @param length2	 -  quadratic lenth of the component vector
			 * @param dimension  -  Number of dimensions of each vector
			 * @return           -  none
			 */
			void PCA::removeComponent(float *samples, int numSamples, float* pVec, float length2, int dimension)
			{
				if ((samples != NULL) && (pVec != NULL) && (length2 > 0.0f) && (dimension > 0))
				{
					for (int i = 0; i < numSamples; i++)
					{
						float dot = 0.0f;
						for (int dim = 0; dim < dimension; dim++)
						{
							dot += samples[i * dimension + dim] * pVec[dim];
						}
						dot /= length2;

						for (int dim = 0; dim < dimension; dim++)
						{
							samples[i * dimension + dim] -= dot * pVec[dim];
						}
					}
				}
			}

			void PCA::calcPCA(const float *samples, int numSamples, float* pOutVecs, int numComponents, int dimension)
			{
				if (samples == NULL)
				{
					throw std::invalid_argument("samples cannot be null!");
				}	
				if (pOutVecs == NULL)
				{
					throw std::invalid_argument("pVecs cannot be null!");
				}	
				if (numSamples <= 0)
				{
					throw std::invalid_argument("numSamples cannot be less than 1!");
				}	
				if (dimension <= 0)
				{
					throw std::invalid_argument("dimension cannot be less than 1!");
				}
				if (numComponents <= 0)
				{
					throw std::invalid_argument("numComponents cannot be less than 1!");
				}
				if (numComponents > dimension)
				{
					throw std::invalid_argument("numComponents cannot be greater than dimension!");
				}
				float length2;
				vector<float> samplesCopy(numSamples * dimension); 
				vector<float> tmpVec(dimension);
				memcpy(&samplesCopy[0], samples, numSamples * dimension * sizeof(float));
				//remove mean
				getMean(&samplesCopy[0], &tmpVec[0], numSamples, dimension);
				subtractVector(&samplesCopy[0], &tmpVec[0], numSamples, dimension);

				for (int i = 0; i < numComponents; i++)
				{
					//calculate prinicpal component
					getComponent(&samplesCopy[0], numSamples, dimension, &tmpVec[0], pOutVecs, length2);
					//remove component
					removeComponent(&samplesCopy[0], numSamples, pOutVecs, length2, dimension);
					
					if (length2 > 0)
					{
						float scale = 1.0f / sqrt(length2);
						for (int j=0; j < dimension; j++)
						{
							pOutVecs[j] *= scale;
						}
					}
					//next vector
					pOutVecs += dimension;
				}
			}
		}
	}
}
