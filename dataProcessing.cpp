#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <random>

double EuclideanDistanceSquaredVectors( std::vector<int> v1, std::vector<int> v2)
{
	if (v1.size() != v2.size())
		return -1;

	double sum = 0;
	for (int i = 0; i < v1.size(); i++)
	{
		sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
	}
	return sum;
}

double EuclideanDistanceSquaredInt(int * v1, int * v2, int dimension)
{

	double sum = 0;
	for (int i = 0; i < dimension; i++)
	{
		sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
	}
	return sum;
}


double EuclideanDistanceSquaredDouble(double * v1, double* v2, int dimension)
{
	double sum = 0;
	for (int i = 0; i < dimension; i++)
	{
		sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
	}
	return sum;
}


double ** randomGaussianMatrix(int numDocs, int dimension)
{
	double ** matrix = new double*[numDocs];
	for (int i = 0; i < numDocs; i++)
	{
		matrix[i] = new double[dimension];
	}

	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.0, sqrt(1.0 / numDocs));

	for (int i = 0; i < numDocs; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			matrix[i][j] = distribution(generator);
		}
	}
	return matrix;
}

double ** gaussianProjection(std::vector<std::vector<int>> documents, double** Wgaussian, int numDocs, int dimension)
{
	int i, j, k;
	double ** matrix = new double*[numDocs];
	for (int i = 0; i < numDocs; i++)
	{
		matrix[i] = new double[dimension];
	}

	for (int i = 0; i < numDocs; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			double soma = 0;
			for (int k = 0; k < dimension; k++)
			{
				soma += documents[i][k] * Wgaussian[k][j];
			}
			matrix[i][j] = soma;
		}
	}
	return matrix;
}

int ** randomAchiloptasMatrix(int numDocs, int dimension)
{
	int ** matrix = new int*[numDocs];
	for (int i = 0; i < numDocs; i++)
	{
		matrix[i] = new int[dimension];
	}

	int possibleValues[6] = { 1,0,0,0,0,-1 };

	for (int i = 0; i < numDocs; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			matrix[i][j] = possibleValues[rand() % 6];
		}
	}
	return matrix;
}

int ** achiloptasProjection(std::vector<std::vector<int>> documents, int** Wachiloptas, int numDocs, int dimension)
{
	int i, j, k;
	int ** matrix = new int*[numDocs];
	for (int i = 0; i < numDocs; i++)
	{
		matrix[i] = new int[dimension];
	}

	for (int i = 0; i < numDocs; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			double soma = 0;
			for (int k = 0; k < dimension; k++)
			{
				soma += documents[i][k] * Wachiloptas[k][j];
			}
			matrix[i][j] = soma;
		}
	}
	return matrix;
}

int main (void)
{	
	// For more precise clock
	// using clock = std::chrono::high_resolution_clock;
	using clock = std::chrono::steady_clock;
	int numDocs, sizeDocs;
	std::vector<std::string> tokens;
	std::vector<std::vector<int>> documents;

	// testing achloptas matrix
	int ** test = randomAchiloptasMatrix(20, 20);

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			std::cout << test[i][j] << " ";
		}
		std::cout << std::endl;
	}
	return 0 ;

	// Loading Documents

	std::ifstream dataset("dataset.csv");
	if (!dataset.is_open())
		std::cout << "Error trying to open" << std::endl;

	std::string token, frequency;
	int i = 0;
	if (dataset.good())
	{
		std::string numDocsStr, sizeDocsStr;
		getline(dataset, numDocsStr, ',');
		numDocs = atoi(numDocsStr.c_str());
		getline(dataset, sizeDocsStr, ';');
		sizeDocs = atoi(sizeDocsStr.c_str());
		dataset.ignore();

		documents.resize(numDocs);
		for (int i = 0; i < numDocs; i++)
			documents[i].resize(sizeDocs);

		for (int tokenNumber = 0; tokenNumber < sizeDocs && dataset.good() && !dataset.eof(); tokenNumber++)
		{
			getline(dataset, token, ',');
			tokens.push_back(token);
			for (int doc = 0; doc < numDocs - 1; doc++)
			{
				getline(dataset, frequency, ',');
				documents[doc][tokenNumber] = atoi(frequency.c_str());
			}
			getline(dataset, frequency, ';');
			documents[numDocs - 1][tokenNumber] = atoi(frequency.c_str());
			dataset.ignore();
		}
	}
	
	// Calculating and storing euclidean distances and tracking time
	// distances[bigger][smaller]

	std::vector<std::vector<double>> distances (numDocs);
	for (int i = 0; i < numDocs; i++)
	{
		distances[i].resize(i+1);
	}

	clock::time_point start = clock::now();

	for (int i = 0; i < numDocs; i++)
	{
		distances[i][i] = 0.0;
		for (int j = i + 1; j < numDocs; j++)
		{
			distances[j][i] = EuclideanDistanceSquaredVectors(documents[i], documents[j]);
		}
	}

	std::chrono::duration<double> timeToCalculateDistance = clock::now() - start;

	std::cout << "Calculating the distance took " << timeToCalculateDistance.count() << " seconds" << std::endl;

	// printing documents

	for (int i = 0; i < numDocs; i++)
	{
		std::cout << "Document " << i << " :" << std::endl;
		for (int j = 0; j < sizeDocs; j++)
		{
			std::cout << " " << tokens[j] << " " << documents[i][j] << std::endl;
		}
		std::cout << std::endl;
	}

	// main loop

	start = clock::now();
	int ** Wachiloptas = randomAchiloptasMatrix(numDocs, sizeDocs);
	std::chrono::duration<double> timeToCalculateAchiloptasMatrix = clock::now() - start;

	start = clock::now();
	double ** Wgaussian = randomGaussianMatrix(numDocs, sizeDocs);
	std::chrono::duration<double> timeToCalculateGaussianMatrix = clock::now() - start;

	start = clock::now();
	int ** projectedAchiloptas = achiloptasProjection(documents, Wachiloptas, numDocs, sizeDocs);
	std::chrono::duration<double> projectionTimeGaussian = clock::now() - start;

	start = clock::now();
	double ** projectedGaussian = gaussianProjection(documents, Wgaussian, numDocs, sizeDocs);
	std::chrono::duration<double> projectionTimeGaussian = clock::now() - start;

	return 0;
}