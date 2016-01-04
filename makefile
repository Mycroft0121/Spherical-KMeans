preprocess: 
	g++ -o preprocess preprocessing.cpp	

kmeans: 
	g++-4.8 -std=c++11 -O2 -o sphkmeans sphkmeans.cpp

