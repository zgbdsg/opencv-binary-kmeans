#include "common.h"

using namespace cv;
using namespace std;

Mat DataRead(string datapath, string filename, string matname)
{
	MATFile *pmatFile = matOpen((datapath + "\\" + filename + ".mat").c_str(), "r");
	if (pmatFile == NULL)
		cout << "MatOpen error!!!" << endl;
	mxArray *pMxArray = matGetVariable(pmatFile, matname.c_str()); //���ļ��л�ȡ���ݵ�mxArray�ṹ��
	if (pMxArray == NULL)
		cout << "Error reading existing matrix " << matname << "!!!" << endl;
	double *ReadArray = (double*)mxGetData(pMxArray);
	int cols = mxGetM(pMxArray);//���д洢��ʽ��һ�£���ע��
	int rows = mxGetN(pMxArray);
	Mat ReadMat(rows, cols, CV_64FC1);  //�˴���Ҫ���Լ���������ģ����ʹ��float�͵�
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
	if (SrcMat.type() != CV_64FC1)//�ж��Ƿ�Ϊdouble�͵ľ���
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
	MATFile *pmatFile = matOpen((datapath + "\\" + filename + ".mat").c_str(), "w"); //��д����ʽ��
	if (pmatFile == NULL)
	{
		cout << "MatOpen error!!!" << endl;
		return EXIT_FAILURE;
	}
	mxArray *pMxArray = mxCreateDoubleMatrix(cols, rows, mxREAL);  //��ע��matlab���д���mat�ļ�����һ��Ķ�ά���顢opencv��Mat���д洢��һ�£�mxCreateDoubleMatrix��������double�͵�pMxArray�ṹ����Ӧ�Ļ���mxCreateNumericMatrix�����ͣ���mxCreateLogicalMatrix�������ͣ�
	if (pMxArray == NULL)
	{
		cout << "Unable to create mxArray, maybe out of memory!!!" << endl;
		return EXIT_FAILURE;
	}
	memcpy((void *)(mxGetPr(pMxArray)), (void *)SaveMat.ptr<float>(0), sizeof(double)*rows*cols);//�ڴ�copy��ע��Դ��Ŀ�ĵ�ַ������������һ��
	cout << "Saving data---" << datapath << "\\" << filename << endl;
	int status = matPutVariable(pmatFile, matname.c_str(), pMxArray);//put���ļ�
	if (status != 0) {
		cout << "Error using matPutVariable!!!" << endl;
		return EXIT_FAILURE;
	}
	mxDestroyArray(pMxArray);//�ṹ����
	if (matClose(pmatFile) != 0) {
		cout << "Error closing file " << pmatFile << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


/*�ҵ���ͬ���ĸ���*/
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

/*�ҳ������в�ͬ��������*/
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