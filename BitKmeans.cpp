#include"common.h"

void BitKmeans(unsigned char**& data, int rows, int cols, int k, int*& lables, int round, double**& dataMap){
	// centers cols is 8 size of data
	int* random = new int[k];
	int* randIndex = new int[k];
	int* cCount = new int[k];
	double** centers = new double*[k];

	set<int> randomSet;

	// init centers
	for (int i = 0; i < k; i++){
		centers[i] = new double[cols];
	}

	for (int i = 0; i < k; i++){
		cCount[i] = 0;
		//random[i] = i;
		randIndex[i] = i;

		int tmp = rand() % rows;

		if (randomSet.find(tmp) == randomSet.end()){
			random[i] = tmp;

			for (int x = 0; x < cols / 8; x++){
				memcpy(&centers[i][x * 8], &dataMap[data[tmp][x]][0], 8);
			}
			//cout << tmp << ",";
		}
		else{
			i--;
		}

	}

	//cout << endl;

	int* index = new int[rows];
	double* dist = new double[rows];

	double** oldCenters = new double*[k];
	for (int i = 0; i < k; i++){
		oldCenters[i] = new double[cols];
		for (int j = 0; j < cols; j++){
			oldCenters[i][j] = 0;
		}
	}

	double* datapower = dataPower(data,rows,cols,dataMap);

	for (int r = 0; r < round; r++){
		//cout << r << " ";
		for (int i = 0; i < k; i++){

			for (int j = 0; j < cols; j++){
				oldCenters[i][j] = centers[i][j];
			}
		}

		double* centerpower = centersPower(centers, k, cols);

		double** distances = BitDistance(data, rows, cols, centers, k, cols,dataMap,datapower,centerpower);

		//double** distances = KmeansBitDistance(data, rows, cols, centers, k, cols);

		for (int i = 0; i < rows; i++){
			double minDist = distances[i][0];
			double minIndex = 0;

			for (int j = 1; j < k; j++){
				if (distances[i][j] < minDist){
					minDist = distances[i][j];
					minIndex = j;
				}
			}

			dist[i] = minDist;
			index[i] = minIndex;
		}

		//update centers
		for (int i = 0; i < k; i++){
			cCount[i] = 0;
			for (int j = 0; j < cols; j++){
				centers[i][j] = 0;
			}
		}

		for (int j = 0; j < rows; j++){
			int a = index[j];
			cCount[a] ++;

			for (int x = 0; x < cols / 8; x++){
				for (int k = 0; k < 8;k++)
					centers[a][x * 8+k] += dataMap[data[j][x]][k];
			}

		}

		for (int i = 0; i < k; i++){

			for (int j = 0; j < cols; j++){
				if (cCount[i] != 0)
					centers[i][j] = centers[i][j] / cCount[i];
				else
					centers[i][j] = 0;
			}
		}

		double sum = 0;
		for (int i = 0; i < k; i++){

			for (int j = 0; j < cols; j++){
				double tmp = oldCenters[i][j] - centers[i][j];
				
				if (_isnan(tmp) != 0)
					tmp = 0;

				//cout << tmp << ",";
				sum += tmp*tmp;
			}
			//cout << endl;
		}

		//cout << "round " << r << " delta cost " << sum << ";";
		if (sum < 0.0000001f){
			break;
		}
	}
	//cout << endl;
	for (int i = 0; i < rows; i++){
		lables[i] = index[i];
	}
}

/***********************************************************************************************************/

double** KmeansBitDistance(unsigned char**& data, int drow, int dcol, double**& centers, int crow, int ccol, double**& dataMap){
	int realcols = dcol / 8;
	double** dis = new double*[drow];
	for (int i = 0; i < drow; i++){
		dis[i] = new double[crow];
	}

	double*** disMap = new double**[crow];

	for (int i = 0; i < crow; i++){
		disMap[i] = new double*[realcols];
		for (int j = 0; j < realcols; j++){
			disMap[i][j] = new double[256];
		}
	}

	for (int i = 0; i < crow; i++){
		for (int j = 0; j < realcols; j++){
			for (unsigned char k = 0; k < 256; k++){
				double sum = 0;
				for (int x = 0; x < 8; x++){
					int tmp = (k >> x)%2;
					cout << tmp;
					if (tmp == 1){
						sum += pow(centers[i][8 * realcols + k]-1,2);
					}
					else{
						sum += pow(centers[i][8 * realcols + k], 2);
					}
				}

				disMap[i][j][k] = sum;
				cout << " ";
			}
			cout << endl;
		}
	}


	for (int i = 0; i < drow; i++){
		for (int j = 0; j < crow; j++){

			double sum = 0;
			for (int k = 0; k < realcols; k++){
				int index = data[i][k];
				sum += disMap[i][k][index];
			}

			dis[i][j] = sum;
		}
	}

	return dis;
}