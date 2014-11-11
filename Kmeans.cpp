#include"common.h"

void Kmeans(Mat& data, int k, Mat& lables, int round){
	int row = data.rows;
	int colum = data.cols;
	int* random = new int[k];
	double* cCount = new double[k];
	Mat centers = Mat::zeros(k, colum, CV_64FC1);

	set<int> randomSet;

	for (int i = 0; i < k; i++){
		cCount[i] = 0;
		random[i] = i;
		//data.row(random[i]).copyTo(centers.row(i));
		int tmp = rand() % row;

		if (randomSet.find(tmp) == randomSet.end()){
			random[i] = tmp;
			data.row(random[i]).copyTo(centers.row(i));
		}
		else{
			i--;
		}

		//for (int j = 0; j < colum; j++){
		//	cout << centers.at<float>(i, j) << "\t";
		//}
		//cout << endl;
	}

	Mat index = Mat::zeros(data.rows, 1, CV_64FC1);
	Mat dist = Mat::zeros(data.rows, 1, CV_64FC1);

	for (int r = 0; r < round; r++){
		Mat distances = Matdistance(data, centers);

		MatMin(distances, index, dist);

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

	}

	index.copyTo(lables);
}