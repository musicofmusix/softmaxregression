// MNISTSoftmaxRegression.cpp : Defines the entry point for the console application.
//Multi-Class softmax regression + Stochastic Gradient Descent optimimsation for weights
//Why? Cause it's easy!

#include "stdafx.h"
#include <iostream>
#include <random>
#include <ctime>
#include "Eigen/Dense"
#include <fstream> //THIS #include must come AFTER 3rd party libs!

using namespace Eigen;
using namespace std;

const int NUMBER_OF_EXAMPLES = 1000;
const double LEARINING_RATE = 0.03;

void copy_features(Ref<MatrixXi> examples, Ref<RowVectorXd> features, int n) //Are these passed by reference?? - Yeah.
	{
	for (int i = 0; i < 28 * 28; i++)
		{
		if (examples(n, i) == 0) { features(i) = 0; } //int to double
		else { features(i) = 1; }
		}
	}

void init_matrix(Ref<MatrixXd> mat) //Matrix matrix(m,n)
	{
	//Type of random number distribution
	
	std::uniform_real_distribution<double> dist(-0.4, 0.4);  //(min, max) NORMALISATION!!!
	std::mt19937 rng; //RNG
	rng.seed(std::random_device{}()); //Initialise with non-deterministic seeds
	
	for (int i = 0; i < mat.rows(); i++)
		{
		for (int j = 0; j < mat.cols(); j++)
			{
			//RNG TEST
			/*
			for (int i = 0; i < 10; i++)
				{
				std::cout << dist(rng) << std::endl;
				}
			*/
			mat(i, j) = dist(rng);
			}
		}
	}	

void softmax(Ref<RowVectorXd> logits, Ref<RowVectorXd> probabilities)
	{
	double sum = 0;
	for (int i = 0; i < 10; i++)
		{
		sum += exp(logits(i));
		}
	for (int i = 0; i < 10; i++)
		{
		probabilities(i) = exp(logits(i)) / sum;
		}
	}

int main()
	{
	streampos size;
	char * memblock;

	ifstream file("C:\\Users\\MusicOfMusiX\\Desktop\\EXAMPLES.bin", ios::in | ios::binary | ios::ate);
	if (file.is_open())
		{
		size = file.tellg();
		memblock = new char[size];
		file.seekg(0x10, ios::beg);
		file.read(memblock, size);
		file.close();

		cout << "The entire file content is now in memory\n";
		//FILE READ TEST
		/*
		for (int num = 0; num < 10; num++)
		{
		for (int i = 0; i < 28; i++)
		{
		for (int j = 0; j < 28; j++)
		{
		int pixel = *(memblock + 28 * 28 * num + 28 * i + j);
		if (pixel == 0)
		{
		cout << 0 << " ";
		}
		else
		{
		cout << 1 << " ";
		}
		}
		cout << "\n";
		}
		cout << "\n";
		}
		*/

		MatrixXi EXAMPLES(NUMBER_OF_EXAMPLES, 28 * 28); //Creating a fixed matrix causes a stack overflow....
		Matrix<double, 28 * 28, 10> WEIGHTS;
		RowVectorXd BIASES(10);
		for (int i = 0; i < NUMBER_OF_EXAMPLES; i++)
			{
			for (int j = 0; j < 28 * 28; j++)
				{
				EXAMPLES(i, j) = *(memblock + 28 * 28 * i + j);
				}
			}
		delete[] memblock;

		init_matrix(WEIGHTS);
		init_matrix(BIASES);

		for (int i = 0; i < 1; i++) //MAIN LOOP
			{
			RowVectorXd FEATURES(28 * 28);
			copy_features(EXAMPLES, FEATURES, i);
			//FEATURE COPY TEST

			for (int h = 0; h < 28; h++)
				{
				for (int j = 0; j < 28; j++)
					{
					double pixel = FEATURES(28 * h + j);
					cout << pixel << " ";
					}
				cout << "\n";
				}
			cout << "\n";

			RowVectorXd LOGITS(10);
			LOGITS = (FEATURES * WEIGHTS);
			LOGITS = LOGITS + BIASES;
			
			//LOGITS OUTPUT TEST
			
			cout << "LOGITS: \n";
			
			for (int logit = 0; logit < 10; logit++)
				{
				cout << LOGITS(logit) << " ";
				}
			
			cout << "\n\n";
			

			RowVectorXd PROBABILITIES(10);
			softmax(LOGITS, PROBABILITIES);

			//PROBABILITIES OUTPUT TEST
			
			cout << "PROBABILITIES (POST-SOFTMAX): \n";

			for (int prob = 0; prob < 10; prob++)
				{
				cout << PROBABILITIES(prob) << " ";
				}

			cout << "\n\n";
			
			}
		}
	else cout << "Unable to open file";
	cout << "PROGRAM FINIHSED";
	std::getchar();
	}

