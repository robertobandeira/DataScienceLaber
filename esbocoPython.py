# -*- coding: utf-8 -*-
"""
Created on Tue Sep 19 12:20:20 2017

@author: Thiago
"""

import newspaper;
import re;
#nltk.download()
from nltk.corpus import stopwords;
from collections import Counter
import numpy as np
import pandas as pd

cnn = newspaper.build('http://cnn.com/', language = 'en', memoize_articles = False)
cnn.size()
#cnn.category_urls()

#cnn.articles[5].text

textos = []
textosSujos = []
stopwordsPattern = re.compile(r'\b(' + r'|'.join(stopwords.words('english')) + r')\b\s*')

k = 1
#article = cnn.articles[19]
#article = cnn.articles[26]
for article in cnn.articles[1:30]:
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
    print(k)
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
    #if j == 0:
    #    bagofwords[:,j] = allWords
    freqInText = Counter()
    freqInText.update(textos[j])
    wordsInText = [item[0] for item in freqInText.items()]
    print("Coluna", j, "de", numCols)
    for w in wordsInText:
        bagofwords[allWords.index(w), j] = (freqInText[w])

df = pd.DataFrame(bagofwords,index=allWords)
df.to_csv("dataset.csv", sep = ",", header = False, line_terminator = ';\n')

