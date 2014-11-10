#include "common.h"

using namespace cv;
using namespace std;

Mat DataRead(string datapath, string filename, string matname)
{
	MATFile *pmatFile = matOpen((datapath + "\\" + filename + ".mat").c_str(), "r");
	if (pmatFile == NULL)
		cout << "MatOpen error!!!" << endl;
	mxArray *pMxArray = matGetVariable(pmatFile, matname.c_str()); //从文件中获取数据到mxArray结构中
	if (pMxArray == NULL)
		cout << "Error reading existing matrix " << matname << "!!!" << endl;
	double *ReadArray = (double*)mxGetData(pMxArray);
	int cols = mxGetM(pMxArray);//行列存储方式不一致，需注意
	int rows = mxGetN(pMxArray);
	Mat ReadMat(rows, cols, CV_64FC1);  //此处主要是自己工程其他模块需使用float型的
	for (int i = 0; i<rows; i++)
	{
		for (int j = 0; j<cols; j++)
		{
			ReadMat.at<double>(i, j) = (double)ReadArray[i*cols + j];
		}
	}
	mxDestroyArray(pMxArray);
	if (matClose(pmatFile) != 0)
		cout << "Error closing file " << pmatFile << endl;
	cout << "Read done!!!" << endl;
	return ReadMat;
}

int DataSave(Mat& SrcMat, string datapath, string filename, string matname)
{
	int rows = SrcMat.rows;
	int cols = SrcMat.cols;
	Mat SaveMat(rows, cols, CV_64FC1);
	if (SrcMat.type() != CV_64FC1)//判断是否为double型的矩阵
	{
		for (int i = 0; i<rows; i++)
		{
			float *src_ptr = SrcMat.ptr<float>(i);
			double *save_ptr = SaveMat.ptr<double>(i);
			for (int j = 0; j<cols; j++)
			{
				*save_ptr++ = src_ptr[j];
			}
		}
	}
	else
		SaveMat = SrcMat.clone();
	MATFile *pmatFile = matOpen((datapath + "\\" + filename + ".mat").c_str(), "w"); //以写的形式打开
	if (pmatFile == NULL)
	{
		cout << "MatOpen error!!!" << endl;
		return EXIT_FAILURE;
	}
	mxArray *pMxArray = mxCreateDoubleMatrix(cols, rows, mxREAL);  //需注意matlab按列储存mat文件，与一般的二维数组、opencv的Mat以行存储不一致，mxCreateDoubleMatrix创建的是double型的pMxArray结构，对应的还有mxCreateNumericMatrix（整型）、mxCreateLogicalMatrix（布尔型）
	if (pMxArray == NULL)
	{
		cout << "Unable to create mxArray, maybe out of memory!!!" << endl;
		return EXIT_FAILURE;
	}
	memcpy((void *)(mxGetPr(pMxArray)), (void *)SaveMat.ptr<float>(0), sizeof(double)*rows*cols);//内存copy，注意源和目的地址的数据类型需一致
	cout << "Saving data---" << datapath << "\\" << filename << endl;
	int status = matPutVariable(pmatFile, matname.c_str(), pMxArray);//put到文件
	if (status != 0) {
		cout << "Error using matPutVariable!!!" << endl;
		return EXIT_FAILURE;
	}
	mxDestroyArray(pMxArray);//结构销毁
	if (matClose(pmatFile) != 0) {
		cout << "Error closing file " << pmatFile << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


/*找到不同类别的个数*/
int FindNClass(Mat& data)
{
	vector<double> vec;
	int nclass = 0;
	for (int i = 0; i < data.cols; i++)
	{
		int j = 0;
		double contain = data.at<double>(0, i);

		int size = vec.size();
		for (j = 0; j < size; j++)
		{
			//contain = data.at<float>(0, i);
			if (vec[j] == contain)
			{
				break;
			}
		}

		//cout << contain << "  ";
		if (j < size)
		{
			continue;
		}
		else
		{
			//cout << "nclass :  "<<nclass << endl;
			nclass = nclass + 1;
			vec.push_back(contain);
		}
	}

	return nclass;
}

/*找出向量中不同类别的类标记*/
vector<double> Unique(Mat& data)
{
	vector<double> vec;
	for (int i = 0; i < data.cols; i++)
	{
		int j = 0;
		double contain = data.at<double>(0, i);

		int size = vec.size();
		for (j = 0; j < size; j++)
		{
			if (vec[j] == contain)
			{
				break;
			}
		}

		//cout << contain << "  ";
		if (j < size)
		{
			continue;
		}
		else
		{
			vec.push_back(contain);
		}
	}

	return vec;
}


void reindex(Mat& data)
{
	assert(data.cols == 1);

	Mat dataTranse = data.t();
	vector<double> labels = Unique(dataTranse);

	for (int i = 0; i < data.rows; i++)
	{
		double now = data.at<double>(i, 0);

		for (int j = 0; j < labels.size(); j++)
		{
			if (now == labels[j])
			{
				data.at<double>(i, 0) = j + 1;
				break;
			}
		}
	}
}

Mat saveDataAsBinary(Mat& data){
	int realcols = data.cols / 8;
	if (data.cols % 8 != 0)
		realcols += 1;

	cout<<"cols:	"<<data.cols << "ralcols:	" << realcols << endl;

	Mat result = Mat::zeros(data.rows, realcols, CV_16SC1);
	for (int i = 0; i < data.rows; i++){
		for (int j = 0; j < realcols;j++){

			int round = ((data.cols - 8 * j)>8) ? 8 : (data.cols - 8 * j);

			for (int k = 0; k < round; k++){
				int t = data.at<float>(i, 8 * j+k);

				if (t == 1)
					result.at<int>(i, j) += pow(2, 7-k);
			}

		}
	}

	return result;
}

unsigned char** converToBinary(Mat& data){
	int realcols = data.cols / 8;
	if (data.cols % 8 != 0)
		realcols += 1;

	cout << "cols:	" << data.cols << "ralcols:	" << realcols << endl;

	unsigned char** result = new unsigned char*[data.rows];
	for (int i = 0; i < data.rows; i++){
		result[i] = new unsigned char[realcols];
	}

	for (int i = 0; i < data.rows; i++){
		for (int j = 0; j < realcols; j++){

			int round = ((data.cols - 8 * j)>8) ? 8 : (data.cols - 8 * j);

			for (int k = 0; k < round; k++){
				int t = data.at<float>(i, 8 * j + k);

				if (t == 1)
					result[i][j] += pow(2, 7 - k);
			}

		}
	}

	return result;
}