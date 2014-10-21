#include <bitset>
#include <iostream>
#include <string>
#include<fstream>
#include<vector>
#include "common.h"

using namespace std;
String path = "..\\data\\aaaa.txt";
vector<String> data;
ifstream ifile;
ofstream ofile;

void openFile(){

}

void closeFile(){

}

void saveBitSetToFile(Mat& data){
	ofile.open(path);
	if (!ofile.is_open()){
		cout << "open error" << endl;
	}

	cout << "begin" << endl;
	for (int i = 0; i < data.rows; i++){
		string row;
		for (int j = 0; j < data.cols; j++){
			int tmp = data.at<float>(i, j);

			cout << tmp << "\t";
			row.append(to_string(tmp));
		}

		bitset<10000> bits(row);
		/*ofile.write(to_string(bits.to_ulong()));*/

		ofile << bits.to_ulong() << "\n";
	}

	ofile.flush();
	ofile.close();
}

vector<String> getLinesFromFile(int line){
	ifile.open(path);
	if (!ifile.is_open()){
		cout << "open error" << endl;
	}

	String content;
	while (ifile >> content){
		cout << content << endl;
		data.push_back(content);
	}
	ifile.close();
	return data;
}

int distanceUseIndex(int a, int b){
	string dataA;
	string dataB;
	dataA = data[a];
	dataB = data[b];

	unsigned long intdataa = stoull(dataA);
	unsigned long intdatab = stoull(dataB);

	bitset<1000> setA(intdataa);
	bitset<1000> setB(intdatab);

	int sum = 0;
	for (int i = 0; i < 1000; i++){
		if (setA[i] != setB[i])
			sum++;
	}

	cout << "distance:" << sum << endl;
	return sum;
}