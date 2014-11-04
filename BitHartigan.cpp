#include"common.h"


void BitHartigan(unsigned char**& data, int rows, int cols, int k, unsigned char*& lables, int round){
	// centers cols is 8 size of data
	int* random = new int[k];
	int* randIndex = new int[k];
	int* cCount = new int[k];
	double** centers = new double*[k];

	for (int i = 0; i < k; i++){
		centers[i] = new double[cols];
	}

	for (int i = 0; i < k; i++){
		cCount[i] = 0;
		random[i] = i;
		randIndex[i] = i;

		for (int a = 0; a < cols; a++){
			centers[i][a] = getDataAt(data, i, a);
		}
	}


}


unsigned char getDataAt(unsigned char**& data, int row, int cols){
	int block = cols / 8;
	int ind = cols % 8;

	unsigned char result = data[row][block] >> ind;
	return result % 2;
}

unsigned char** BitDistance(unsigned char**& data,int drow,int dcol,double**& centers,int crow,int ccol){
	double** tmpLeft = new double*[drow];
	for (int i = 0; i < drow; i++){
		tmpLeft[i] = new double[crow];
	}

	double* tmpXPowerSum = new double[drow];
	double* tmpYPower = new double[crow];

	for ()
}
void oldBitHartigan(Mat& data, int n, int k, Mat& lables, int round){
	int row = data.rows;
	int colum = data.cols;
	int* random = new int[k];
	Mat randindex = Mat::zeros(k, 1, CV_64FC1);
	double* cCount = new double[k];
	Mat centers = Mat::zeros(k, n, CV_64FC1);

	set<int> randomSet;
	Mat tmpCenter(k,data.cols,CV_16SC1);

	for (int i = 0; i < k; i++){
		cCount[i] = 0;
		random[i] = i;
		randindex.at<double>(i, 0) = i;
		data.row(random[i]).copyTo(tmpCenter.row(i));
		//int tmp = rand() % row;

		//if (randomSet.find(tmp) == randomSet.end()){
		//	random[i] = tmp;
		//	data.row(random[i]).copyTo(centers.row(i));
		//}
		//else{
		//	i--;
		//}
	}

	updateCenters(tmpCenter, randindex, centers, n);

	Mat distances = MatBitDistance(data, centers);
	cout << distances.row(0) << endl;

	Mat index = Mat::zeros(distances.rows, 1, CV_64FC1);
	Mat dist = Mat::zeros(distances.rows, 1, CV_64FC1);

	MatMin(distances, index, dist);
	cout << data.row(4) << endl;

	updateCenters(data, index, centers, n);


	for (int cnt = 0; cnt < round; cnt++){
		for (int id = 0; id < row; id++){
			int newid = id;
			int cluster1 = index.at<double>(id, 0);

			if (cCount[cluster1]>1){
				Mat vx = data.row(id);

				Mat vc1 = centers.row(cluster1);

				Mat vxBinary = ToBinary(vx);


				Mat clusterTmp = (vc1*cCount[cluster1] - vxBinary) / (cCount[cluster1] - 1.0);
				clusterTmp.copyTo(centers.row(cluster1));

				cCount[cluster1] = cCount[cluster1] - 1;

				Mat dis = MatBitDistance(vx, centers);
				//cout << dis.row(0) << endl;


				Mat deltaCost(1, k, CV_64FC1);
				for (int i = 0; i < k; i++){
					deltaCost.at<double>(0, i) = (cCount[i] / (cCount[i] + 1.0))*dis.at<double>(0, i);
				}
				//cout << deltaCost.row(0) << endl;
				int cluster2 = 0;
				double minValue = deltaCost.at<double>(0, 0);
				for (int i = 1; i < k; i++){
					double tmpValue = deltaCost.at<double>(0, i);
					if (minValue >= tmpValue){
						cluster2 = i;
						minValue = tmpValue;
					}
				}

				if (cluster1 == cluster2){
					vc1.copyTo(centers.row(cluster1));
					cCount[cluster1] = cCount[cluster1] + 1;
				}
				else{
					//cout << centers.row(cluster2) << endl;
					//cout << cCount[cluster2] << endl;

					Mat tmp = (centers.row(cluster2)*cCount[cluster2] + vxBinary) / (cCount[cluster2] + 1);

					cCount[cluster2] = cCount[cluster2] + 1;
					index.at<double>(id, 0) = cluster2;
				}
			}
		}

		updateCenters(data, index, centers, n);

		//for (int i = 0; i < k; i++)
		//	cCount[i] = 0;

		//for (int i = 0; i < k; i++){
		//	centers.row(i).setTo(Scalar(0)); // put all zero
		//}
		//for (int i = 0; i < index.rows; i++)
		//{
		//	int a = index.at<double>(i, 0);
		//	cCount[a]++;
		//	centers.row(a) += data.row(i);
		//}
		//for (int i = 0; i < k; i++)
		//{
		//	Mat tmpMat = 1.0*centers.row(i) / cCount[i];
		//	tmpMat.copyTo(centers.row(i));
		//}
		// divide center by ccount
	}

	index.copyTo(lables);

}

Mat MatBinaryDistance(Mat& x,Mat& y){
	Mat distance = Mat::zeros(x.rows, y.rows, CV_64FC1);

	for (int i = 0; i < x.rows; i++){
		for (int j = 0; j < y.rows; j++){
			double sum = 0;
			for (int k = 0; k < y.cols; k++){

				int t = x.at<int>(i,k) >> k;
				int tmp = t % 2;
				if (tmp != 0)
					sum += pow(tmp*y.at<double>(j, k), 2);
			}

			distance.at<double>(i, j) = sum;
		}
	}
}

Mat MatBitDistance(Mat& x, Mat& y){
	Mat distance = Mat::zeros(x.rows,y.rows,CV_64FC1);

	for (int i = 0; i < x.rows; i++){
		for (int j = 0; j < y.rows; j++){
			double sum = 0;
			for (int k = 0; k < y.cols; k++){
				int block = k / 8;
				int bit = k % 8;

				int tmpData = x.at<unsigned int>(i, block);
				int t = tmpData >> bit;
				int tmp = t % 2;
				if (tmp!= 0)
					sum += pow(tmp*y.at<double>(j, k),2);
			}

			distance.at<double>(i,j) = sum;
		}
	}
}



void updateCenters(Mat& data,Mat& index,Mat& centers,int n){
	/*
	*index data index
	*size index's size
	*/
	int* cCnt = new int[centers.rows];
	for (int i = 0; i < centers.rows; i++){
		cCnt[i] = 0;
		centers.row(i).setTo(Scalar(0));
	}

	for (int i = 0; i < n; i++){
		//update the ith bit
		int block = i / 8;
		int bit = i % 8;
			//sum the jth bit 
			double sum = 0;

			for (int k = 0; k < data.rows; k++){

				int centerK = index.at<double>(k,0);
				int tmpData = data.at<double>(k,block);
				int x = tmpData >> bit;

				centers.at<double>(centerK,i) += (x % 2);
				cCnt[centerK] ++;
			}
			
			for (int a = 0; a < centers.rows; a++){
				centers.at<double>(a, i) = centers.at<double>(a, i) / cCnt[a];
			}
		
	}
}

Mat ToBinary(Mat& data){
	Mat binaryMat = Mat::zeros(data.rows, 8 * data.cols, CV_16SC1);
	for (int i = 0; i < data.rows; i++){
		for (int j = 0; j < 8 * data.cols; j++){
			int block = j / 8;
			int bit = j % 8;

			int tmpData = data.at<int>(i, block);
			int x = tmpData >> bit;

			binaryMat.at<int>(i, j) = (x % 2);
		}
	}

	return binaryMat;
}