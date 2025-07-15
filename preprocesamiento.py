
import pandas as pd
import numpy as np
import re
import nltk
import matplotlib.pyplot as plt
from sklearn.feature_extraction.text import CountVectorizer

# Etiquetas y ruta
etiquetas = ["libro1", "libro2", "libro3", "libro4", "libro5", "libro6"]
carpeta = "libros/"
corpus = []

# Leer los libros desde la carpeta
for etiqueta in etiquetas:
    with open(carpeta + etiqueta + ".txt", "r", encoding="utf-8") as archivo:
        corpus.append(archivo.read())

# Convertir a numpy array y DataFrame
corpus = np.array(corpus)
df_corpus = pd.DataFrame({"documento": corpus, "categoria": etiquetas})

# Vectorización
count_vectorizer = CountVectorizer(min_df=0.0, max_df=1.0)
matriz_conteo = count_vectorizer.fit_transform(corpus)
matriz_conteo_densa = matriz_conteo.toarray()

# Vocabulario y representación
vocabulario = count_vectorizer.get_feature_names_out()
df_bolsa = pd.DataFrame(matriz_conteo_densa, columns=vocabulario)

# Mostrar vocabulario
print(len(vocabulario), vocabulario)

# Guardar vocabulario
np.savetxt("vocab.txt", vocabulario, fmt="%s", delimiter=",")
