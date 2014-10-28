#include"common.h"

void hartigan(Mat& data, int k, Mat& lables, int round){
	int row = data.rows;
	int colum = data.cols;
	int* random = new int[k];
	double* cCount = new double[k];
	Mat centers = Mat::zeros(k, colum, CV_64FC1);

	set<int> randomSet;
	
	for (int i = 0; i < k; i++){
		cCount[i] = 0;
		random[i] = i;
		data.row(random[i]).copyTo(centers.row(i));
		//int tmp = rand() % row;

		//if (randomSet.find(tmp) == randomSet.end()){
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
	cout << distances.row(0) << endl;

	Mat index = Mat::zeros(distances.rows, 1, CV_64FC1);
	Mat dist = Mat::zeros(distances.rows, 1, CV_64FC1);

	MatMin(distances, index, dist);
	cout << data.row(4) << endl;

	for (int i = 0; i < k; i++){
		cCount[i] = 0;
		Mat tmp = Mat::zeros(1, data.cols, CV_64FC1);
		for (int j = 0; j < index.rows; j++){
			if ((int)index.at<double>(j, 0) == i){
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
			int cluster1 = index.at<double>(id,0);

			if (cCount[cluster1]>1){
				Mat vx = data.row(id);
				Mat vc1 = centers.row(cluster1);

				Mat clusterTmp = (vc1*cCount[cluster1] - vx) / (cCount[cluster1] - 1.0);
				clusterTmp.copyTo(centers.row(cluster1));

				cCount[cluster1] = cCount[cluster1] - 1;

				Mat dis = Matdistance(vx, centers);
				//cout << dis.row(0) << endl;
				

				Mat deltaCost(1, k, CV_64FC1);
				for (int i = 0; i < k; i++){
					deltaCost.at<double>(0, i) = (cCount[i] / (cCount[i] + 1.0))*dis.at<double>(0, i);
				}
				//cout << deltaCost.row(0) << endl;
				int cluster2 = 0;
				double minValue = deltaCost.at<double>(0,0);
				for (int i = 1; i < k; i++){
					double tmpValue = deltaCost.at<double>(0, i);
					if (minValue>= tmpValue){
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

					Mat tmp = (centers.row(cluster2)*cCount[cluster2] + vx) / (cCount[cluster2] + 1);
					
					cCount[cluster2] = cCount[cluster2] + 1;
					index.at<double>(id, 0) = cluster2;
				}
			}
		}

		for (int i = 0; i < k; i++)
			cCount[i] = 0;

		for (int i = 0; i < k; i++){
			centers.row(i).setTo(Scalar(0)); // put all zero
		}
		for (int i = 0; i < index.rows; i++)
		{
			int a = index.at<double>(i, 0);
			cCount[a]++;
			centers.row(a) += data.row(i);
		}
		for (int i = 0; i < k; i++)
		{
			Mat tmpMat = 1.0*centers.row(i) / cCount[i];
			tmpMat.copyTo(centers.row(i));
		}
			// divide center by ccount
	}

	index.copyTo(lables);
}


Mat Matdistance(Mat& x, Mat& y){
	//bsxfun(@minus,bsxfun(@minus,2*X*C',sum(X.^2,2)),sum(C.^2,2)');
	//Mat dist(x.rows, y.rows, CV_32FC1);

	Mat tmpLeft(x.rows, y.rows, CV_64FC1);
	Mat tmpXPowerSum(x.rows, 1, CV_64FC1);
	Mat tmpYPower(1, y.rows, CV_64FC1);

	//tmpLeft = 2.0 * x*y.t();
	for (int i = 0; i < x.rows; i++){
		for (int j = 0; j < y.rows; j++){
			double tmp = 0;
			for (int k = 0; k < x.cols; k++){
				tmp += x.at<double>(i, k)*y.at<double>(j, k);
			}

			tmpLeft.at<double>(i, j) = 2.0*tmp;
		}
	}
	//cout << tmpLeft.row(0) << endl;

	for (int i = 0; i < x.rows; i++){
		double sum = 0;
		for (int j = 0; j < x.cols; j++){
			sum += pow(x.at<double>(i, j), 2);
		}
		tmpXPowerSum.at<double>(i, 0) = sum;
	}

	//printMat(tmpXPowerSum);

	for (int i = 0; i < y.rows; i++){
		double sum = 0;
		for (int j = 0; j < y.cols; j++){
			sum += pow(y.at<double>(i, j), 2);
		}
		tmpYPower.at<double>(0, i) = sum;
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
	Mat result(a.rows, a.cols, CV_64FC1);

	if (b.cols == 1){
		for (int i = 0; i < a.rows; i++){
			for (int j = 0; j < a.cols; j++){
				result.at<double>(i, j) = a.at<double>(i, j) - b.at<double>(i, 0);
			}
		}
	}
	else if (b.rows == 1){
		for (int i = 0; i < a.rows; i++){
			for (int j = 0; j < a.cols; j++){
				result.at<double>(i, j) = a.at<double>(i, j) - b.at<double>(0, j);
			}
		}
	}
	//printMat(result);
	return result;
}

void printMat(Mat& a){
	for (int i = 0; i < a.rows; i++){
		for (int j = 0; j < a.cols; j++){
			cout << a.at<double>(i, j) << "\t";
		}
		cout << endl;
	}
}

void MatMin(Mat& data, Mat& index, Mat& dist){
	// min of ervery row
	//index = Mat::zeros(data.rows, 1, CV_32FC1);
	//dist = Mat::zeros(data.rows, 1, CV_32FC1);
	for (int i = 0; i < data.rows; i++){
		double minDist = data.at<double>(i, 0);
		double minIndex = 0;

		for (int j = 1; j < data.cols; j++){
			if (data.at<double>(i, j) < minDist){
				minDist = data.at<double>(i, j);
				minIndex = j;
			}
		}

		dist.at<double>(i, 0) = minDist;
		index.at<double>(i, 0) = minIndex;
	}
}