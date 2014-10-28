#include "common.h"

Mat NormalizeFea(Mat& fea, int row)
{
	//cout << fea.rows << "  " << fea.cols << endl;
	Mat newfea;
	if (row == 1)
	{
		int nsmp = fea.cols;
		double* powersum = new double[nsmp];


		/*计算每行的平方和*/
		for (int i = 0; i < nsmp; i++)
		{
			powersum[i] = 0;
			for (int j = 0; j < fea.rows; j++)
			{
				//cout << fea.at<float>(i, j) << endl;
				powersum[i] = powersum[i] + fea.at<double>(j, i)*fea.at<double>(j, i);
			}

			if (powersum[i] == 0)
				powersum[i] = 0.00000000000001;
		}
		/*cout << powersum[0] << endl;
		cout << powersum[1] << endl;*/

		/*单位化*/
		Mat diags = Mat::zeros(nsmp, nsmp, CV_64FC1);

		for (int i = 0; i < nsmp; i++)
		{
			diags.at<double>(i, i) = 1.0 / sqrt(powersum[i]);
			//cout << diags.at<float>(i, i) << endl;
		}

		/*cout << diags.at<float>(0, 0) << endl;
		cout << diags.at<float>(1, 1) << endl;*/
		//cout << diags.rows << "  " << diags.cols << endl;
		//cout << fea.rows << "  " << fea.cols << endl;
		newfea = fea * diags;
		/*cout << newfea.at<float>(0, 0) << endl;
		cout << newfea.at<float>(0, 1) << endl;
		cout << newfea.at<float>(0, 2) << endl;*/

		delete[] powersum;
	}
	else
	{
		int nsmp = fea.rows;
		double* powersum = new double[nsmp];

		/*计算每行的平方和*/
		for (int i = 0; i < nsmp; i++)
		{
			powersum[i] = 0;
			for (int j = 0; j < fea.cols; j++)
			{
				powersum[i] = powersum[i] + fea.at<double>(i, j)*fea.at<double>(i, j);
			}

			if (powersum[i] == 0)
				powersum[i] = 0.00000000000001;
		}

		/*单位化*/
		Mat diags = Mat::zeros(nsmp, nsmp, CV_64FC1);

		for (int i = 0; i < nsmp; i++)
		{
			diags.at<double>(i, i) = 1.0 / sqrt(powersum[i]);
		}
		newfea = diags * fea;

		delete[] powersum;
	}

	return newfea;
}