#include"common.h"


void BitHartigan(unsigned char**& data, int rows, int cols, int k, int*& lables, int round, int**& dataMap){
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

	//for (int i = 0; i < cols; i++){
	//	int tmp = getDataAt(data, 0, i);
	//	cout << tmp;
	//}
	//cout << endl;

	for (int i = 0; i < k; i++){
		cCount[i] = 0;
		//random[i] = i;
		randIndex[i] = i;

		int tmp = rand() % rows;

		if (randomSet.find(tmp) == randomSet.end()){
			random[i] = tmp;

			for (int a = 0; a < cols; a++){
				int block = a / 8;
				int ind = a % 8;
				centers[i][a] = dataMap[data[tmp][block]][ind];
				//centers[i][a] = getDataAt(data, tmp, a);
			}

			//data.row(random[i]).copyTo(centers.row(i));
		}
		else{
			i--;
		}

		//for (int a = 0; a < cols; a++){
		//	centers[i][a] = getDataAt(data, i, a);
		//}
	}

	//compute distatnce between centers and data

	double** distances = BitDistance(data, rows, cols, centers, k, cols,dataMap);

	int* index = new int[rows];
	double* dist = new double[rows];

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

	//for (int i = 0; i < rows; i++)
	//	cout << index[i] << "\t";

	//cout << endl;
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

		for (int x = 0; x < cols; x++){
			int block = x / 8;
			int ind = x % 8;
			centers[a][x] += dataMap[data[j][block]][ind];
			//centers[a][x] += getDataAt(data, j, x);
		}
	}

	//for (int i = 0; i < cols; i++)
	//	cout << centers[0][i] << "\t";

	//cout << endl;
	for (int i = 0; i < k; i++){

		for (int j = 0; j < cols; j++){
			centers[i][j] = centers[i][j]/cCount[i];
		}
	}


	//begin iteration

	for (int cnt = 0; cnt < round; cnt++){
		for (int id = 0; id < rows; id++){
			int newid = id;
			int cluster1 = index[id];

			if (cCount[cluster1]>1){
				unsigned char* vx = data[id];
				//double* vc1 = centers[cluster1];
				double* vc1 = new double[cols];
				for (int a = 0; a < cols; a++){
					vc1[a] = centers[cluster1][a];
				}

				for (int a = 0; a < cols; a++){
					int block = a / 8;
					int ind = a % 8;

					int tmp = dataMap[data[id][block]][ind];
					centers[cluster1][a] = (vc1[a] * cCount[cluster1] - tmp) / (cCount[cluster1] - 1.0);
					//centers[cluster1][a] = (vc1[a]*cCount[cluster1] - getDataAt(data,id,a)) / (cCount[cluster1] - 1.0);
				}

				cCount[cluster1] = cCount[cluster1] - 1;

				double* dis = VectorBitDistance(vx, cols, centers, k, cols,dataMap);
				double* deltaCost = new double[k];

				for (int i = 0; i < k; i++){
					deltaCost[i] = (cCount[i] / (cCount[i] + 1.0))*dis[i];
				}

				int cluster2 = 0;
				double minValue = deltaCost[0];
				for (int i = 1; i < k; i++){
					double tmpValue = deltaCost[i];
					if (minValue >= tmpValue){
						cluster2 = i;
						minValue = tmpValue;
					}
				}

				if (cluster1 == cluster2){
					for (int a = 0; a < cols; a++){
						centers[cluster1][a] = vc1[a];
					}
					cCount[cluster1] = cCount[cluster1] + 1;
				}
				else{
					for (int a = 0; a < cols; a++){
						int block = a / 8;
						int ind = a % 8;

						int tmp = dataMap[data[id][block]][ind];
						centers[cluster2][a] = (centers[cluster2][a] * cCount[cluster2] + tmp) / (cCount[cluster2] + 1.0);
						//centers[cluster2][a] = (centers[cluster2][a] * cCount[cluster2] + getDataAt(data, id, a)) / (cCount[cluster2] + 1.0);
					}

					cCount[cluster2] = cCount[cluster2] + 1;
					index[id] = cluster2;
				}
			}
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

			for (int x = 0; x < cols; x++){
				int block = x / 8;
				int ind = x % 8;
				centers[a][x] += dataMap[data[j][block]][ind];
				//centers[a][x] += getDataAt(data, j, x);
			}
		}

		for (int i = 0; i < k; i++){

			for (int j = 0; j < cols; j++){
				centers[i][j] = centers[i][j] / cCount[i];
			}
		}
	}

	for (int i = 0; i < rows; i++){
		lables[i] = index[i];
	}

}


//unsigned char getDataAt(unsigned char**& data, int row, int cols){
//	int block = cols / 8;
//	int ind = cols % 8;
//
//	unsigned char result = data[row][block] >> ind;
//	//cout << result % 2 << endl;
//	return result % 2;
//}
//
//unsigned char getVectorDataAt(unsigned char*& data, int row, int cols){
//	int block = cols / 8;
//	int ind = cols % 8;
//
//	unsigned char result = data[block] >> ind;
//	return result % 2;
//}


double* VectorBitDistance(unsigned char*& data, int dcol, double**& centers, int crow, int ccol, int**& dataMap){
	//bsxfun(@minus,bsxfun(@minus,2*X*C',sum(X.^2,2)),sum(C.^2,2)');
	int drow = 1;
	double** tmpLeft = new double*[drow];
	for (int i = 0; i < drow; i++){
		tmpLeft[i] = new double[crow];
	}

	for (int i = 0; i < drow; i++){
		for (int j = 0; j < crow; j++){

			double sum = 0;
			for (int k = 0; k < dcol; k++){
				int block = k / 8;
				int ind = k % 8;
				sum += 2.0*dataMap[data[block]][ind] * centers[j][k];

				//sum += 2.0*getVectorDataAt(data, i, k) * centers[j][k];
			}

			tmpLeft[i][j] = sum;
		}
	}

	double* tmpXPowerSum = new double[drow];
	double* tmpYPower = new double[crow];

	for (int i = 0; i < drow; i++){
		double sum = 0;
		for (int j = 0; j < dcol; j++){
			int block = j / 8;
			int ind = j % 8;
			int tmp = dataMap[data[j]][ind];

			sum += tmp*tmp;
			//sum += pow(getVectorDataAt(data, i, j), 2);
		}

		tmpXPowerSum[i] = sum;
	}

	for (int i = 0; i < crow; i++){
		double sum = 0;
		for (int j = 0; j < ccol; j++){
			sum += pow(centers[i][j], 2);
		}

		tmpYPower[i] = sum;
	}

	for (int i = 0; i < drow; i++){
		for (int j = 0; j < crow; j++){
			tmpLeft[i][j] = -tmpLeft[i][j] + tmpXPowerSum[i] + tmpYPower[j];
		}
	}

	return tmpLeft[0];
}

double** BitDistance(unsigned char**& data, int drow, int dcol, double**& centers, int crow, int ccol, int**& dataMap){
	//bsxfun(@minus,bsxfun(@minus,2*X*C',sum(X.^2,2)),sum(C.^2,2)');
	assert(dcol == ccol);
	double** tmpLeft = new double*[drow];
	for (int i = 0; i < drow; i++){
		tmpLeft[i] = new double[crow];
	}

	for (int i = 0; i < drow; i++){
		for (int j = 0; j < crow; j++){

			double sum = 0;
			//cout << "aaa:" << dataMap[0][0] << endl;
			//inner product
			for (int k = 0; k < dcol; k++){
				//getData use search
				int block = k / 8;
				int ind = k % 8;
				sum += (double)dataMap[data[i][block]][ind] * centers[j][k];
				//sum += (double)getDataAt(data,i,k) * centers[j][k];
			}

			tmpLeft[i][j] = 2.0*sum;
		}
	}

	double* tmpXPowerSum = new double[drow];
	double* tmpYPower = new double[crow];

	for (int i = 0; i < drow; i++){
		double sum = 0;
		for (int j = 0; j < dcol; j++){
			int block = j / 8;
			int ind = j % 8;
			sum += dataMap[data[i][block]][ind];
			//sum += pow((int)getDataAt(data, i, j), 2);
		}

		tmpXPowerSum[i] = sum;
	}

	for (int i = 0; i < crow; i++){
		double sum = 0;
		for (int j = 0; j < ccol; j++){
			sum += centers[i][j]*centers[i][j];
		}

		tmpYPower[i] = sum;
	}

	for (int i = 0; i < drow; i++){
		for (int j = 0; j < crow; j++){
			tmpLeft[i][j] = -tmpLeft[i][j]+tmpXPowerSum[i]+tmpYPower[j];
		}
	}

	return tmpLeft;
}