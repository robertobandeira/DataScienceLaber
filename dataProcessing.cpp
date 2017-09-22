#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

double EuclideanDistanceSquared( vector<pair<string, int>> v1, vector<pair<string, int>> v2)
{
	if (v1.size() != v2.size())
		return -1;

	double sum = 0;
	for (int i = 0; i < v1.size(); i++)
	{
		sum += (v1[i].second - v2[i].second) * (v1[i].second - v2[i].second);
	}
	return sum;
}

int main (void)
{	
	// For more precise clock
	// using clock = std::chrono::high_resolution_clock;
	using clock = std::chrono::steady_clock;
	int numDocs, sizeDocs;
	vector<vector<pair<string, int>>> documents;
	// Loading Documents

	ifstream dataset("dataset.csv");
	if (!dataset.is_open())
		cout << "Error trying to open" << endl;

	string token, frequency;
	int i = 0;
	if (dataset.good())
	{
		string numDocsStr, sizeDocsStr;
		getline(dataset, numDocsStr, ',');
		numDocs = atoi(numDocsStr.c_str());
		getline(dataset, sizeDocsStr, ';');
		sizeDocs = atoi(sizeDocsStr.c_str());
		dataset.ignore();

		documents.resize(numDocs);
		for (int i = 0; i < numDocs; i++)
			documents[i].resize(sizeDocs);

		int doc = 0;
		int row = 0;
		while (dataset.good() && !dataset.eof())
		{
			getline(dataset, token, ',');
			getline(dataset, frequency, ';');
			dataset.ignore();

			documents[doc][row].first = token;
			documents[doc][row].second = atoi(frequency.c_str());
			row++;
			if (row == sizeDocs)
			{
				row = 0;
				doc++;
			}
		}
	}
	
	// Calculating and storing euclidean distances and tracking time

	vector<vector<double>> distances (numDocs);
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

	chrono::duration<double> timeToCalculateDistance = clock::now() - start;

	cout << "Calculating the distance took " << timeToCalculateDistance.count() << " seconds" << endl;

	// printing documents

	for (int i = 0; i < numDocs; i++)
	{
		cout << "Document " << i << " : " << endl;
		for (int j = 0; j < sizeDocs; j++)
		{
			cout << " " << documents[i][j].first << " " << documents[i][j].second << endl;
		}
		cout << endl;
	}

	return 0;
}