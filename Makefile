CC = g++
BOOST =

SOURCE_ALL = src/NDVector.cpp src/similaritySearch.cpp src/NearestNeighborSearch.cpp src/VectorTSVReader.cpp src/metrics.cpp
SOURCE_LSH = src/CosineSimilarityLSH.cpp src/EuclideanSpaceLSH.cpp src/HashTable.cpp
SOURCE_HRP = src/HypercubeProjection.cpp

INCLUDE_ALL = include/AbstractSimilaritySearch.h include/metrics.h include/NDVector.h include/NearestNeighborSearch.h include/similaritySearch.h include/VectorTSVReader.h
INCLUDE_LSH = include/CosineSimilarityLSH.h include/EuclideanSpaceLSH.h include/HashTable.h
INCLUDE_HRP = include/HypercubeProjection.h