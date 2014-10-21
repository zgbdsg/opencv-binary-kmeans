#include"common.h"

void hartigan(Mat& data, int k, Mat& lables, int round){
	int row = data.rows;
	int colum = data.cols;
	int* random = new int[k];
	int* cCount = new int[k];
	Mat centers = Mat::zeros(k, colum, CV_32FC1);

	set<int> randomSet;
	
	for (int i = 0; i < k; i++){
		cCount[i] = 0;
		random[i] = i;
		data.row(random[i]).copyTo(centers.row(i));
		//int tmp = rand() % row;

		//if (randomSet.find(tmp) != randomSet.end()){
		//	random[i] = tmp;
		//	data.row(random[i]).copyTo(centers.row(i));
		//}
		//else{
		//	i--;
		//}

		//for (int j = 0; j < colum; j++){
		//	cout << centers.at<float>(i, j) << "\t";
		//}
		//cout << endl;
	}
	
	//Mat dist(row, k, CV_32FC1);
	Mat distances = Matdistance(data, centers);

	Mat index = Mat::zeros(distances.rows, 1, CV_32FC1);
	Mat dist = Mat::zeros(distances.rows, 1, CV_32FC1);

	MatMin(distances, index, dist);

	for (int i = 0; i < k; i++){
		cCount[i] = 0;
		Mat tmp = Mat::zeros(1, data.cols, CV_32FC1);
		for (int j = 0; j < index.rows; j++){
			if (index.at<float>(j, 0) == i){
				cCount[i] ++;
				tmp = tmp + data.row(j);
			}
		}

		if (cCount[i] != 0){
			tmp = tmp / cCount[i];
		}

		tmp.copyTo(centers.row(i));
	}

	for (int cnt = 0; cnt < round; cnt++){
		for (int id = 0; id < row; id++){
			int newid = id;
			int cluster1 = index.at<float>(id,0);

			if (cCount[cluster1]>1){
				Mat vx = data.row(id);
				Mat vc1 = centers.row(cluster1);

				Mat clusterTmp = (vc1*cCount[cluster1] - vx) / (cCount[cluster1] - 1);
				clusterTmp.copyTo(centers.row(cluster1));

				cCount[cluster1] = cCount[cluster1] - 1;

				Mat dis = Matdistance(vx, centers);

				Mat deltaCost(1,k,CV_32FC1);
				for (int i = 0; i < k; i++){
					deltaCost.at<float>(0, i) = (1.0*cCount[cluster1] / (cCount[cluster1] + 1))*dis.at<float>(0, i);
				}

				int cluster2 = 0;
				float minValue = deltaCost.at<float>(0,0);
				for (int i = 1; i < k; i++){
					float tmpValue = deltaCost.at<float>(0, i);
					if (minValue> tmpValue){
						cluster2 = i;
						minValue = tmpValue;
					}
				}

				if (cluster1 == cluster2){
					vc1.copyTo(centers.row(cluster1));
					cCount[cluster1] = cCount[cluster1] + 1;
				}
				else{
					cout << centers.row(cluster2) << endl;
					cout << cCount[cluster2] << endl;

					Mat tmp = (centers.row(cluster2)*cCount[cluster2] + vx) / (cCount[cluster2] + 1);
					
					cCount[cluster2] = cCount[cluster2] + 1;
					index.at<float>(id, 0) = cluster2;
				}
			}
		}

		for (int i = 0; i < k; i++){
			cCount[i] = 0;
			Mat tmpData = Mat::zeros(1, data.cols, CV_32FC1);
			for (int j = 0; j < index.rows; j++){
				if (index.at<float>(j, 0) == i){
					cCount[i] ++;
					tmpData = tmpData + data.row(j);
				}
			}

			if (cCount[i] != 0){
				tmpData = tmpData / cCount[i];
			}

			tmpData.copyTo(centers.row(i));
		}
	}

	index.copyTo(lables);
}


Mat Matdistance(Mat& x, Mat& y){
	//bsxfun(@minus,bsxfun(@minus,2*X*C',sum(X.^2,2)),sum(C.^2,2)');
	//Mat dist(x.rows, y.rows, CV_32FC1);

	Mat tmpLeft(x.rows, y.rows, CV_32FC1);
	Mat tmpXPowerSum(x.rows, 1, CV_32FC1);
	Mat tmpYPower(1, y.rows, CV_32FC1);

	tmpLeft = 2 * x*y.t();

	for (int i = 0; i < x.rows; i++){
		float sum = 0;
		for (int j = 0; j < x.cols; j++){
			sum += pow(x.at<float>(i, j), 2);
		}
		tmpXPowerSum.at<float>(i, 0) = sum;
	}

	//printMat(tmpXPowerSum);

	for (int i = 0; i < y.rows; i++){
		float sum = 0;
		for (int j = 0; j < y.cols; j++){
			sum += pow(y.at<float>(i, j), 2);
		}
		tmpYPower.at<float>(0, i) = sum;
	}

	//cout << tmpYPower.row(0) << endl;
	//printMat(tmpYPower);

	Mat a = matMinus(tmpLeft, tmpXPowerSum);
	Mat dist = -matMinus(a, tmpYPower);
	return dist;
}

Mat matMinus(Mat& a, Mat& b){
	//bsxfun(@minus,a,b)
	//b col is 1
	Mat result(a.rows, a.cols, CV_32FC1);

	if (b.cols == 1){
		for (int i = 0; i < a.rows; i++){
			for (int j = 0; j < a.cols; j++){
				result.at<float>(i, j) = a.at<float>(i, j) - b.at<float>(i, 0);
			}
		}
	}
	else if (b.rows == 1){
		for (int i = 0; i < a.rows; i++){
			for (int j = 0; j < a.cols; j++){
				result.at<float>(i, j) = a.at<float>(i, j) - b.at<float>(0, j);
			}
		}
	}
	//printMat(result);
	return result;
}

void printMat(Mat& a){
	for (int i = 0; i < a.rows; i++){
		for (int j = 0; j < a.cols; j++){
			cout << a.at<float>(i, j) << "\t";
		}
		cout << endl;
	}
}

void MatMin(Mat& data, Mat& index, Mat& dist){
	// min of ervery row
	//index = Mat::zeros(data.rows, 1, CV_32FC1);
	//dist = Mat::zeros(data.rows, 1, CV_32FC1);
	for (int i = 0; i < data.rows; i++){
		float minDist = data.at<float>(i, 0);
		float minIndex = 0;

		for (int j = 1; j < data.cols; j++){
			if (data.at<float>(i, j) < minDist){
				minDist = data.at<float>(i, j);
				minIndex = j;
			}
		}

		dist.at<float>(i, 0) = minDist;
		index.at<float>(i, 0) = minIndex;
	}
}