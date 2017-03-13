/**
 * @brief Perceptron Algorithm
 *
 * very simple implementation of a single layer Perceptron class
 * In contract to the MultilayerPerceptron class this is a thinner implementation.
 * However, single layer Perceptrons would be also supported by the MultilayerPerceptron class.
 *
 *
 * Author:	Stefan Moebius (mail@stefanmoebius.de)
 *
 * Date:	2011-07-21
 *
 * Licence: Released to the PUBLIC DOMAIN
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 */
#include "perceptron.h"
namespace Sys
{
	namespace ArtificialIntelligence
	{
		namespace EagerLearning
		{
			/**
			 * Creates a new instance of a single layer Perceptron
			 * @param size                  - The number of input neurons
			 * @param learningRate          - float point value between 0.0 and 1.0
			 * @param randomizeWeights      - if true weights are initialized with random values (-0.3 to +0.3)
			 * @throw std::invalid_argument - if an invalid size parameter is declared
			 * @throw std::bad_alloc        - if memory allcation fails
			 */
			Perceptron::Perceptron(int size, float learningRate, bool randomizeWeights)
			{
				if (size < 1)
				{
					throw invalid_argument("Perceptron size cannot be less than 1");
				}
				if (learningRate < 0.0f){learningRate = 0.0f;}
				if (learningRate > 1.0f){learningRate = 1.0f;}
				this->size = size; 
				this->learningRate = learningRate;
				if (randomizeWeights)
				{
					//Initialize with random numbers (-0.3 to +0.3)
					weights = new float[size];
					for (int i = 0; i < size; i++)
					{
						weights[i] = float(rand() % 600) * 0.001f - 0.3f;
					}
					bias = float(rand() % 600) * 0.001f - 0.3f;
				}
				else
				{
					//Initialize with zero
					weights = new float[size]();
					bias = 0.0f;
				}
			}
			/**
			 * Frees all reserved ressources
			 */
			Perceptron::~Perceptron()
			{
				if (weights)
					delete weights;
			}
			/**
			 * calculates the output of the perceptron based on the declared input and weights
			 * @param input                 - input vector with the dimension declared in the constructor
			 * @return                      - result of the calculation (between 0.0 and 1.0) 
			 */
			float Perceptron::calculate(float* input)
			{
				result = bias;
				float* w = weights;
				for (int i = 0; i < size; i++)
				{
					result += (*input++) * (*w++);
				}
				result = activation(result); // make sure result is between 0.0 and 1.0
				return result;
			}
			/**
			 * adapts the weights of the perceptron
			 * @param target                - value which should have been returned by calculate()
			 * @param input                 - the same input data used for calculate()
			 * @return                      - none
			 */
			void Perceptron::feedback(float target, float* input)
			{
				float* w = weights;
				float d = learningRate * error(target, input);
				bias += d;
				for (int i = 0; i < size; i++)
				{
					(*w++) += (*input++) * d;
				}
			}
			/**
			 * Changes the learning rate of the Perceptron
			 * @param learningRate          - float point value between 0.0 and 1.0
			 * @return                      - old learning rate
			 */
			float Perceptron::setLearningRate(float learningRate)
			{
				float oldLearningRate = this->learningRate;
				if (learningRate < 0.0f){learningRate = 0.0f;}
				if (learningRate > 1.0f){learningRate = 1.0f;}
				this->learningRate = learningRate;
				return oldLearningRate;
			}
			/**
			 * Loads weights of the perceptron from an binary file
			 * @param *fileName			- Path to file which should be opened
			 * @throw ios_base::failure - if opening or writing the file fails
			 * @return					- none
			 */
			void Perceptron::loadFile(const char *fileName)
			{
				ifstream file;
				file.exceptions(ios_base::failbit | ios_base::eofbit | ios_base::badbit);
				file.open(fileName, ios::in | ios::binary);
				file.read((char*)&bias, sizeof(float));
				file.read((char*)weights, sizeof(float) * size);
				file.close();
			}
			/**
			 * Saves weights of the perceptron to a binary file.
			 * If the file already exits it will be overwritten.
			 * @param *fileName         - Path to file which should be created
			 * @throw ios_base::failure - if opening or writing the file fails
			 * @return                  - none
			 */
			void Perceptron::saveFile(const char *fileName)
			{
				ofstream file;
				try
				{
					file.exceptions(ios_base::failbit | ios_base::eofbit | ios_base::badbit);
					file.open(fileName, ios::out | ios::binary);
					file.write((char*)&bias, sizeof(float));
					file.write((char*)weights, sizeof(float) * size);
					file.close();
				}
				catch(...)
				{
					//delete partly created file
					file.exceptions(ios_base::goodbit); //make sure no further exception will be thrown
					file.close(); // first close file
					remove(fileName);
					throw; // throw exception again
				}
			}
			/**
			 * lerning rule of the perceptron. The function is called inside of feedback()
			 * @param target                - first parameter of feedback()
			 * @param input                 - second parameter of feedback()
			 * @return                      - desired error
			 */
			float Perceptron::error(float target, float* input)
			{
				return (target - result);
			}
			/**
			 * Activation function of the perceptron. The function is called inside of calculate()
			 * @param res                - the calculated output of calculate()
			 * @return                   - desired output
			 */
			float Perceptron::activation(float res)
			{
				if (res > 1.0f){return 1.0f;}
				if (res < 0.0f){return 0.0f;}
				return res;
			}
		}
	}
}
