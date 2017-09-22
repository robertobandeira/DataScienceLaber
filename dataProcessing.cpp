#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

double EuclideanDistanceSquared( std::vector<int> v1, std::vector<int> v2)
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

int main (void)
{	
	// For more precise clock
	// using clock = std::chrono::high_resolution_clock;
	using clock = std::chrono::steady_clock;
	int numDocs, sizeDocs;
	std::vector<std::string> tokens;
	std::vector<std::vector<int>> documents;

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

		documents.resize(sizeDocs);
		for (int i = 0; i < sizeDocs; i++)
			documents[i].resize(numDocs);

		for (int tokenNumber = 0; tokenNumber < sizeDocs && dataset.good() && !dataset.eof(); tokenNumber++)
		{
			getline(dataset, token, ',');
			tokens.push_back(token);
			for (int doc = 0; doc < numDocs - 1; doc++)
			{
				getline(dataset, frequency, ',');
				documents[tokenNumber][doc] = atoi(frequency.c_str());
			}
			getline(dataset, frequency, ';');
			documents[tokenNumber][numDocs - 1] = atoi(frequency.c_str());
			dataset.ignore();
		}
	}
	
	// Calculating and storing euclidean distances and tracking time

	std::vector<std::vector<double>> distances (numDocs);
	for (int i = 0; i < numDocs; i++)
	{
		distances[i].resize(i + 1);
	}

	clock::time_point start = clock::now();

	for (int i = 0; i < numDocs; i++)
	{
		distances[i][i] = 0.0;
		for (int j = i + 1; j < numDocs; j++)
		{
			distances[j][i] = EuclideanDistanceSquared(documents[i], documents[j]);
		}
	}

	std::chrono::duration<double> timeToCalculateDistance = clock::now() - start;

	std::cout << "Calculating the distance took " << timeToCalculateDistance.count() << " seconds" << std::endl;

	// printing documents

	for (int j = 0; j < numDocs; j++)
	{
		std::cout << "Document " << i << " :" << std::endl;
		for (int i = 0; i < sizeDocs; i++)
		{
			std::cout << " " << tokens[i] << " " << documents[i][j] << std::endl;
		}
	}

	return 0;
}