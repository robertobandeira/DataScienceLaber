# -*- coding: utf-8 -*-
"""
Created on Tue Sep 19 12:20:20 2017

@author: Thiago
"""

import newspaper;
import re;
from nltk.corpus import stopwords
from collections import Counter
import numpy as np
import csv

cnn = newspaper.build('http://cnn.com/', language = 'en', memoize_articles = False)
cnn.size()

textos = []
textosSujos = []
urls = []
stopwordsPattern = re.compile(r'\b(' + r'|'.join(stopwords.words('english')) + r')\b\s*')

csvfile = open('urls.csv','r')
leitor = csv.reader(csvfile)
for row in leitor:
    urls.append(row)    
csvfile.close()
csvfile = open('urls.csv','w')
escritor = csv.writer(csvfile)


## salvar links
## ao fim salvar em csv


k = 1
## while len(urls) < 3000
for article in cnn.articles[1:500]:
    article.download()
    article.html
    if article.html == '':
        continue
    article.parse()
    textoSujo = article.text
    if len(textoSujo) == 0:
        continue
    textosSujos.append(textoSujo.split())
    
    texto = article.text
    texto = texto.lower()
    texto = re.sub('\W+',' ', texto)
    texto = stopwordsPattern .sub('', texto)
    textos.append(texto.split())
    if article.url not in urls and len(urls) < 3000 and article.url != []:
        urls.append(article.url)
        escritor.writerow([article.url])
    k = k+1
    

frequency = Counter()
for text in textos:
    frequency.update((text))

frequencySujos = Counter()
for text in textosSujos:
    frequencySujos.update((text))


frequency.most_common(10)
frequencySujos.most_common(10)

# Numero 2
len(frequency)
len(frequencySujos)


# Construcao da Bag of Words
bagofwords = np.zeros((len(frequency), len(textos)), dtype=np.int)

allWords = [item[0] for item in frequency.items()]
numCols = len(bagofwords[0])
numRows = len(bagofwords)

for j in range(numCols):
    freqInText = Counter()
    freqInText.update(textos[j])
    wordsInText = [item[0] for item in freqInText.items()]
    for w in wordsInText:
        bagofwords[allWords.index(w), j] = freqInText[w]


csvfile.close()