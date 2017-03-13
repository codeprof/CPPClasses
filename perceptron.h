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
#ifndef _PERCEPTRON_H_
#define _PERCEPTRON_H_

#include <iostream> // just there for the NULL definition...
#include <fstream>
#include <random>

using namespace std;

namespace Sys
{
	namespace ArtificialIntelligence
	{
		namespace EagerLearning
		{
			class Perceptron
			{
			public:
				Perceptron(int size, float learningRate = 0.5f, bool randomizeWeights = true);
				virtual ~Perceptron();
				inline float calculate(float* input);
				inline void feedback(float target, float* input);
				inline float setLearningRate(float learningRate);
				void loadFile(const char *fileName);
				void saveFile(const char *fileName);
			protected:
				inline float error(float target, float* input); //can be overwritten to implement own error function
				inline static float activation(float res); //can be overwritten to implement own activation function
				//members
				int size;
				float learningRate, result, bias;
				float* weights;
			private:
				//disallow copy and assign
				Perceptron(const Perceptron&);               
				void operator=(const Perceptron&);
			};
		}
	}
}
#endif

