import csv
import operator
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

with open('dataset.csv', newline='') as csvfile:
	readerDataset = csv.reader(csvfile, delimiter=',')
	numRow = 0
	numCol = 0
	tokens = {}
	
	print("Reading csv")
	for row in readerDataset:
		totalFrequency = 0
		for i in range(1, len(row)-1):
			totalFrequency += int(row[i])
		totalFrequency += int(row[len(row)-1][0])
		tokens[row[0]] = totalFrequency

	print("Sorting tokens")
	sortedTokens = sorted(tokens.items(), key=operator.itemgetter(1))

	print("Most frequent tokens")
	mostFrequent = {}
	a = 0
	for token,frequency in reversed(sortedTokens):
		if (a > 10):
			break
		mostFrequent[token] = frequency
		a+=1

	fig, ax = plt.subplots()
	n, bins, patches = ax.hist(mostFrequent.values(), 10, normed=1)
	ax.set_xlabel('Tokens')
	ax.set_ylabel('Frequency')
	ax.set_title('Most Frequent Tokens')

	fig.tight_layout()
	plt.show()

#	a = 0
#	for token,frequency in reversed(sortedTokens):
#		if (a > 10):
#			break
#		print(token," ",frequency)
#		a+=1
