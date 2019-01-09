cmake_minimum_required(VERSION 3.13)
project(ergasia3)

set(CMAKE_CXX_STANDARD 11)

add_executable(ergasia3
        include/ApproximateNeighborSearch/AbstractSimilaritySearch.h
        include/ApproximateNeighborSearch/CosineSimilarityLSH.h
        include/ApproximateNeighborSearch/EuclideanSpaceLSH.h
        include/ApproximateNeighborSearch/HashTable.h
        include/ApproximateNeighborSearch/HypercubeProjection.h
        include/ApproximateNeighborSearch/metrics.h
        include/ApproximateNeighborSearch/NDVector.h
        include/ApproximateNeighborSearch/NearestNeighborSearch.h
        include/ApproximateNeighborSearch/similaritySearch.h
        include/ApproximateNeighborSearch/VectorCSVReader.h
        src/ApproximateNeighborSearch/CosineSimilarityLSH.cpp
        src/ApproximateNeighborSearch/EuclideanSpaceLSH.cpp
        src/ApproximateNeighborSearch/HashTable.cpp
        src/ApproximateNeighborSearch/HypercubeProjection.cpp
        src/ApproximateNeighborSearch/metrics.cpp
        src/ApproximateNeighborSearch/NDVector.cpp
        src/ApproximateNeighborSearch/NearestNeighborSearch.cpp
        src/ApproximateNeighborSearch/VectorCSVReader.cpp
        include/clustering/initializing.h
        include/clustering/assigning.h
        include/clustering/updating.h
        include/clustering/Silhouette.h
        include/clustering/StoppingCriterion.h
        include/clustering/cluster.hpp
        include/clustering/DistancesTable.h
        include/clustering/kmeans.hpp
        src/clustering/assigning.cpp
        src/clustering/updating.cpp
        src/clustering/Silhouette.cpp
        src/clustering/StoppingCriterion.cpp
        src/clustering/DistancesTable.cpp
        src/clustering/parameters.cpp
        src/clustering/initializing.cpp


        include/utils.hpp
        include/parameters.hpp

        src/main.cpp
        src/Datasets.cpp
        include/Lexicon.hpp
        include/Datasets.hpp

        src/preprocessing.cpp
        include/preprocessing.hpp
        src/clustering/clustering.cpp
        include/clustering/clustering.h
        src/DatasetStructures.cpp
        src/recommendations.cpp
        include/recommendations.hpp
        src/ProximitySearch.cpp
        include/ProximitySearch.hpp src/UserVectors.cpp include/UserVectors.hpp src/Evaluation.cpp include/Evaluation.hpp)