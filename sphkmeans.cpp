#include<iostream>
#include<string>
#include<stdlib.h>
#include<fstream>
#include<math.h>
#include<algorithm>
#include<map>
#include<vector>
#include<set>
#include<stdio.h>
#include<string.h>
#include<ctime>

using namespace std;

int maxIteration=0;
int *objects;
int *dimensions;
float *frequencies;
int objectCount = 0;
int dimensionCount = 0;
int numberOfClusters = 0;
//vector<vector<float>> centroids;
float **centroids;
int *objectCluster;
int *clusterCount;
int maxDimension;
float entropy = 0;
float purity = 0;


int countLines(string filename){
	int count = 0;
	string line;
	ifstream myfile;
	myfile.open(filename);
	while(getline(myfile, line))
		count++;
	myfile.close();
	return count;
}

void writeOutput(string fileName){
	string line;
	ofstream myfile;
	myfile.open(fileName);
	
	if(myfile){
		for(int i = 0; i < objectCount; i++){	
			string temp = to_string(i+1) + "," + to_string(objectCluster[i]);
			myfile << temp << endl;
		}
	}
	myfile.close();
}

void printVector(vector<int> v){

	for(int i = 0; i < v.size() ; i++)
		cout << v[i] << " ";
	cout << endl;
}

void printMap(map< int, vector<int> > t){
	cout << "In printMap \n";
        map< int, vector<int> >::iterator it_t;
	it_t = t.begin();
        while(it_t != t.end()){
                cout<< it_t->first << ": ";
                for(int i=0 ; i<it_t->second.size() ; i++){
                        cout << it_t->second[i] << " ";
                }
                cout << endl;
                it_t++ ;
        }
	cout << "Getting out \n";
}

void normalizeFrequencies(){
	
	for(int i = 0; i < objectCount; i++){
//		cout << i << " " << objects[i] << endl;
		float temp = 0;

		for(int j = ( i==0 ? 0 : objects[i-1] + 1); j <= objects[i]; j++){
			temp = temp + frequencies[j]*frequencies[j];
		}
		temp = sqrt(temp);			
//		cout << temp << endl;		
		for(int j = ( i==0 ? 0 : objects[i-1] + 1); j <= objects[i]; j++){
			frequencies[j] = frequencies[j]/temp;
		}
	}
}

void normalizeCentroids(float *centroid){

//	cout << "In here\n";	
	float sumOfDimensions = 0;
	for (int j = 0; j < maxDimension; j++)
		sumOfDimensions += centroid[j] * centroid[j];
//	cout << sumOfDimensions << endl;	
	sumOfDimensions = sqrt(sumOfDimensions);
	for(int j = 0; j < maxDimension; j++)
		centroid[j] /= sumOfDimensions;
}

float cosine(float *centroid, int object){

	float tempNumerator = 0;

	for(int i = (object==0 ? 0 : objects[object-1]+1); i <= objects[object]; i++){
		tempNumerator = tempNumerator + frequencies[i]*centroid[dimensions[i]];
	}
	return tempNumerator;
}

float  objectiveFunction( map<int, vector<int> > &clusters){

	
	for(int i = 0; i < numberOfClusters; i++)
		for (int j = 0; j < maxDimension; j++)
			centroids[i][j] = 0;	
	
	for(int i = 0; i < objectCount; i++){
		for(int j = (i==0 ? 0 : objects[i-1]+1); j < objects[i]; j++)
                                centroids[objectCluster[i]][dimensions[j]] += frequencies[j];
	}
	
	float  objective = 0;
	for(int i = 0; i < numberOfClusters; i++){
		float tempObjective = 0;
		for (int j = 0; j < maxDimension; j++){
			tempObjective += centroids[i][j] * centroids[i][j];		
		}
		objective += sqrt(tempObjective);		
	}
	cout << "Objective = " << objective << endl;
	return objective;
}

float objectiveFunction2(map<int, vector<int> > &clusters){
	
	float objective = 0;
        for(int i = 0; i < numberOfClusters; i++){
                float tempObjective = 0;
                for (int j = 0; j < clusters[i].size(); j++){
			float temp = cosine(centroids[i], clusters[i][j]);
			objective += temp;
//			cout<< temp<< endl;
		}
	}
//	cout << "Objective2 = " << objective << endl; 
	return objective;
}

void entropyAndPurity(map<int, vector<int> > &clusters, string &classFile){

//	cout << "In measurement \n";
	map<int, string> objectClassMap;
	map<string, int> classIdMap;
	string objectID, className;
	int classIdIndex = 0;
        ifstream myfile;
        myfile.open(classFile);

        while(getline(myfile, objectID, ',')){
                getline(myfile, className);
		objectClassMap[stoi(objectID)-1] = className;
		if (!classIdMap.count(className))
			classIdMap[className] = classIdIndex++;
        }	
	
	int measureMatrix[clusters.size()][classIdMap.size()];	
	memset(measureMatrix, 0, sizeof(measureMatrix[0][0]) * clusters.size() * classIdMap.size());

	int clusterSize = clusters.size();
	int classSize = classIdMap.size();


	for(int i = 0; i < clusters.size(); i++){
		
		for (int j = 0; j < clusters[i].size(); j++){
			int classId = classIdMap[objectClassMap[clusters[i][j]]];
			measureMatrix[i][classId] += 1;
		}

	}
	
	int tempSum = 0;	
	for(int i = 0; i < clusters.size(); i++){
		for(int j = 0; j < classIdMap.size(); j++){
			cout << measureMatrix[i][j] << " ";
			tempSum += measureMatrix[i][j];
		}
		cout << endl;
	}
//	cout << "Sum = " << tempSum << endl;
		

	int clusterSum[clusterSize];
	memset(clusterSum, 0, sizeof(clusterSum[0]) * clusterSize);
//	float clusterPurity[clusterSize];
//	memset(clusterPurity, 0, sizeof(clusterPurity[0]) * clusterSize);

	float clusterEntropy;
	float clusterPurity;
	int clusterWeight;
	entropy = 0;
	purity = 0;
	
		
	for (int i = 0; i < clusterSize; i++){
		for(int j = 0; j < classSize; j++){
			clusterSum[i] += measureMatrix[i][j]; 
		}
	}

        for (int i = 0; i < clusterSize; i++){
		clusterEntropy = 0;
		clusterPurity = 0;
                for(int j = 0; j < classSize; j++){
	                float p_ij = float(measureMatrix[i][j]) / float(clusterSum[i]);
			if(p_ij != 0)
	                        clusterEntropy += p_ij * log2(p_ij);            
			if(p_ij > clusterPurity)
				clusterPurity = p_ij;
//			cout << measureMatrix[i][j] << " " << clusterSum[i] << " " <<  p_ij << endl;
		}
//		cout << "Cluster Entropy - " << clusterEntropy << endl;  
		entropy += (clusterSum[i] / float(objectCount)) * clusterEntropy; 
		purity += (clusterSum[i] / float(objectCount)) * clusterPurity;
        }
//	cout << "Entropy = " << entropy << endl;
//	cout << "Purity = " << purity << endl;

//	cout << "Out of here \n";
	
}

int cosine(int object){

//	cout << "Hello";
	float oldSimilarity = 0;
	int clusterNo = 0;
	for(int i = 0; i < numberOfClusters; i++){
		float newSimilarity = 0;
		for(int j = (object==0 ? 0 : objects[object-1]+1); j <= objects[object]; j++){
			newSimilarity += frequencies[j]*centroids[i][dimensions[j]];
		}
			
		if (newSimilarity > oldSimilarity){
			clusterNo = i;
			oldSimilarity = newSimilarity;
		}
//		cout << oldSimilarity << endl;
	}
	return clusterNo;	
}

void sphkmeans(map<int, vector<int> > &clusters){


	map <int, vector<int> > tempClusters;
	int equalCount = 0;
	int unequalCount = 0;
	int tempObjectCluster[objectCount];
	int start_s = clock();
	int stop_s = clock();

	for(int i = 0 ; i < objectCount; i++){
//		int start = i==0 ? 0 : objects[i-1]+1;
//		int stop = objects[i];
		float oldSimilarity = 0;
		int index = 0;
		 for (int j = 0; j < numberOfClusters; j++){
/*			float newSimilarity = 0;
			for(int k = start; k < stop; k++)
				newSimilarity += frequencies[k]*centroids[j][dimensions[k]-1];
*/
			float newSimilarity = cosine(centroids[j], i);
			if(newSimilarity > oldSimilarity){
				oldSimilarity = newSimilarity;
				index = j;
			}
		}
		//int index = cosine(i);
		tempClusters[index].push_back(i);
		tempObjectCluster[i] = index;
	}
	

	stop_s = clock();
//	cout << (stop_s-start_s) / (double)(CLOCKS_PER_SEC / 1000)<< endl;
	if (maxIteration > 0){
		for(int i = 0; i < objectCount; i++){
			if(tempObjectCluster[i] == objectCluster[i])
				equalCount++;
			else
				unequalCount++;
		}
//		cout<< "Object Equal count = " << equalCount << endl;
//		cout<< "Object Unequal count = " << unequalCount << endl;
	}
	

	if(equalCount < int(0.997*objectCount) && maxIteration <= 35){
		start_s = clock();
		for(int i = 0; i < objectCount; i++)
			objectCluster[i] = tempObjectCluster[i];
		
		clusters = tempClusters;

		for(int i = 0; i < numberOfClusters; i++)
			for (int j = 0; j < maxDimension; j++)
				centroids[i][j] = 0;	
//              memset(centroids, 0, sizeof(centroids[0][0])*numberOfClusters*(maxDimension-1));
                memset(clusterCount, 0, sizeof(clusterCount[0])*numberOfClusters);

		for(int i = 0; i < objectCount; i++){
			for(int j = (i==0 ? 0 : objects[i-1]+1); j < objects[i]; j++){
				centroids[objectCluster[i]][dimensions[j]] += frequencies[j];
				clusterCount[objectCluster[i]]++;
//				cout << i << tempObjectCluster[i] << endl; 
			}	
		}

		for(int i = 0; i < numberOfClusters; i++){
			for(int j = 0; j < maxDimension; j++)
				centroids[i][j] /= clusterCount[i];
//			cout << "Cluster Count = " << clusterCount[i] << endl;
			normalizeCentroids(centroids[i]);
		}
		
		maxIteration++;
		stop_s = clock();
// 		cout << (stop_s-start_s) / (double)(CLOCKS_PER_SEC / 1000)<< endl;
		sphkmeans(clusters);
	}

}



int main(int argc, char* argv[]){

	if(argc < 2){
		cout << "Usage:" << argv[1] << " input-file class-file #clusters #trials output-file \n \n";
		return 1;
	}

	string inputFile = argv[1];
	string classFile = argv[2];
	numberOfClusters = atoi(argv[3]);
	int numberOfTrials = atoi(argv[4]);
	string outputFile = argv[5];
	int startTime = 0;
	int stopTime = 0;

	objectCount = countLines(classFile);
	objects = (int *)malloc(objectCount*sizeof(int));
	dimensionCount = countLines(inputFile);
	dimensions = (int *)malloc(dimensionCount*sizeof(int));
	frequencies = (float *)malloc(dimensionCount*sizeof(float));

	string objectID, dimensionNO, frequency;
	int tempIndex = 0;
	ifstream myfile;
	myfile.open(inputFile);
	
	while(getline(myfile, objectID, ',')){
		getline(myfile, dimensionNO, ',');
		getline(myfile, frequency);
		
		objects[stoi(objectID)-1] = tempIndex;
		dimensions[tempIndex] = stoi(dimensionNO)-1;
		frequencies[tempIndex] = stof(frequency);
		tempIndex++;
	}
	
	maxDimension = *max_element(dimensions, dimensions+dimensionCount) + 1;
//	cout << maxDimension << endl;
	normalizeFrequencies();
	
	centroids = (float**)malloc(numberOfClusters*sizeof(float*));
	for(int i = 0; i < numberOfClusters; i++){
		centroids[i] = (float*)malloc((maxDimension)*sizeof(float));
	}
	clusterCount = (int*)malloc(numberOfClusters*sizeof(int*));

	set<int> random;
	float maxObjective = 0;
	map<int, vector<int> >finalClusters;
	objectCluster = (int*)malloc(objectCount*sizeof(int));	

	startTime = clock();
	for(int i = 0; i < numberOfTrials; i++){
		map <int, vector<int> > clusters;
		maxIteration = 0;
		srand(i*2+1);
		int index;

		for(int i = 0; i < numberOfClusters; i++)
			for (int j = 0; j < maxDimension; j++)
				centroids[i][j] = 0;
		memset(clusterCount, 0, sizeof(clusterCount[0])*numberOfClusters);

		for (int j = 0; j < numberOfClusters; j++){
			index = rand()%(objectCount);

			while(random.count(index))
				index = rand()%(objectCount);
			random.insert(index);
	
			for(int k = ( index==0 ? 0 : objects[index-1] + 1); k <= objects[index]; k++){
        	              	centroids[j][dimensions[k]] = frequencies[k];
			}
	        }
		random.clear();
//		cout << "*********** Trial No " << i+1 << endl;
		sphkmeans(clusters);
//		cout  << "Max Iterations = " << maxIteration << endl;
//		printMap(clusters);	
		float getObjective = objectiveFunction2(clusters);
		if(getObjective > maxObjective){
			maxObjective = getObjective;
			finalClusters = clusters;
		}
//		objectiveFunction(finalClusters);
//		cout << "*****************************\n \n";
//		break;
	}

	stopTime = clock();
	float executionTime =  (stopTime - startTime) / (double)(CLOCKS_PER_SEC / 1000);

//	cout << "Number of Clusters =" << numberOfClusters << endl;

	cout << "\n ********************** " << inputFile << "|" << numberOfClusters <<" *****************\n" << endl;
	entropyAndPurity(finalClusters, classFile);
	writeOutput(outputFile);
	cout << "\nMax Objective = " << maxObjective << endl;
	cout << "Entropy = " << entropy * -1 << endl;
	cout << "Purity = " << purity << endl;


	ofstream rfile;
	rfile.open("Readings", ios_base::app);
	rfile << inputFile << "|" << to_string(numberOfClusters) << "|" << to_string(executionTime) << "|" << to_string(objectCount) << "|" << to_string(maxDimension) << "|" << to_string(dimensionCount) << "|" << to_string(maxObjective) << "|" <<  to_string(entropy * -1) << "|" << to_string(purity) << "|"<<  endl; 
	rfile.close();

	free(objects);
	free(dimensions);
	free(frequencies);

	for(int i = 0; i < numberOfClusters; i++)
		free(centroids[i]);

	free(centroids);
	free(clusterCount);
	free(objectCluster);
	return 0;

}
