#include "common.h"

vector< std::vector<int> > array_to_matrix(int* m, int rows, int cols) {
	int i, j;
	std::vector< std::vector<int> > r;
	r.resize(rows, std::vector<int>(cols, 0));

	for (i = 0; i<rows; i++)
	{
		for (j = 0; j<cols; j++)
			r[i][j] = m[i*cols + j];
	}
	return r;
}

Mat BestMap(Mat& L1, Mat& L2)
{
	assert(L1.cols == L2.cols && L1.rows == L2.rows);

	Mat t_L1 = L1.t();
	Mat t_L2 = L2.t();
	vector<double> label1 = Unique(t_L1);
	int nclass1 = FindNClass(t_L1);
	vector<double> label2 = Unique(t_L2);
	int nclass2 = FindNClass(t_L2);

	int nclass = (nclass1 >= nclass2) ? nclass1 : nclass2;

	/*effort Matrix*/
	Matrix<double> G_Effort(nclass1, nclass2);
	int* new_G_effort = new int[nclass1 * nclass2];

	/*计算不同赋值的代价*/
	for (int i = 0; i < nclass1; i++)
	{
		for (int j = 0; j < nclass2; j++)
		{
			int effort_ij = 0;
			for (int k = 0; k < L1.rows; k++)
			{
				if (L1.at<double>(k, 0) == label1[i] && L2.at<double>(k, 0) == label2[j])
					effort_ij++;
				//cout << effort_ij<<" ";
			}

			//if (effort_ij==0)
			//	new_G_effort[i * nclass1 + j] = 0;
			//else
			//	new_G_effort[i * nclass1 + j] = effort_ij;
			if (effort_ij)
				G_Effort(i, j) = -effort_ij;
			else
				G_Effort(i, j) = 100000;
			//cout << G_Effort(i, j) << " ";
		}
		//cout << endl;
	}
	/*********************test********************/
	//G_Effort(0, 0) = 250;
	//G_Effort(0, 1) = 400;
	//G_Effort(0, 2) = 350;
	//G_Effort(1, 0) = 400;
	//G_Effort(1, 1) = 600;
	//G_Effort(1, 2) = 350;
	//G_Effort(2, 0) = 200;
	//G_Effort(2, 1) = 400;
	//G_Effort(2, 2) = 250;

	//vector< std::vector<int> > mt = array_to_matrix(new_G_effort, nclass1, nclass2);
	//Hungarian hungarian(mt, nclass1, nclass2, HUNGARIAN_MODE_MAXIMIZE_UTIL);
	/* some output */
	//fprintf(stderr, "cost-matrix:");
	//hungarian.print_cost();

	/* solve the assignement problem */
	//hungarian.solve();

	/* some output */
	//fprintf(stderr, "assignment:");
	//hungarian.print_assignment();
	//vector<vector<int>> result = hungarian.assignment();

	/*hungarian 算法 找出最短匹配即准确率最高的赋值*/
	Munkres m;
	m.solve(G_Effort);
	Mat matchAnswer(nclass1, 1, CV_64FC1);

	for (int row = 0; row < nclass1; row++) {
		for (int col = 0; col < nclass2; col++) {
			//std::cout.width(2);
			//std::cout << G_Effort(row, col) << ",";
			if (G_Effort(row, col) == 0){
				matchAnswer.at<double>(col, 0) = row;
				//cout<<"   match:  "<< col+1 <<"   "<<row+1  << "  ";
			}

			//std::cout << result[row][col] << ",";
			//if (result[row][col] == 1){
			//	matchAnswer.at<float>(row, 0) = col;
			//	cout << "match:  " << matchAnswer.at<float>(row, 0)+1 << "   "<< col +1 << endl;
			//}
		}
		//std::cout << std::endl;
	}

	Mat newl2(L2.rows, L2.cols, CV_64FC1);
	for (int i = 0; i < L2.rows; i++)
	{
		double tmp = L2.at<double>(i, 0);
		int ind = 0;
		for (int j = 0; j < label2.size(); j++)
		{
			if (label2[j] == tmp)
			{
				ind = j;
				break;
			}
		}
		newl2.at<double>(i, 0) = label1[matchAnswer.at<double>(ind, 0)];
	}
	return newl2;
}
