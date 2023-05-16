#include"util.h"

void feature9x9(Mat3f image, vector<float> &feature) {
	int rowStart = ((image.rows / 2) - 4);
	int rowEnd = ((image.rows / 2) + 4);
	int colStart = ((image.cols / 2) - 4);
	int colEnd = ((image.cols / 2) + 4);
	for (int row = rowStart; row <= rowEnd; row++) {
		Vec3f* rowptr = image.ptr<Vec3f>(row);
		for (int col = colStart; col <= colEnd; col++) {
			for (int channel = 0; channel <= 2; channel++) {
				feature.push_back(rowptr[col][channel]);
			}
		}
	}
}

void Hist3D(Mat3f image, vector<float>& feature) {
	const int bins = 8;
	int div = 256 / bins;
	int dim3[3] = { bins, bins, bins };
	Mat Histogram3D = Mat::zeros(3, dim3, CV_32S);
	float normalizing_sum = image.rows * image.cols;
	for (int row = 0; row < image.rows; row++) {
		Vec3f* rowptr = image.ptr<Vec3f>(row);
		for (int col = 0; col < image.cols; col++) {
			int Rindex = int(rowptr[col][0]) / div;
			int Bindex = int(rowptr[col][1]) / div;
			int Gindex = int(rowptr[col][2]) / div;
			Histogram3D.at<int>(Rindex,Bindex,Gindex) += 1;
		}
	}
	// Normalizing the histogram
	for (int x = 0; x < bins; x++) {
		for (int y = 0; y < bins; y++) {
			for (int z = 0; z < bins; z++) {
				float element = float(Histogram3D.at<int>(x,y,z)) / normalizing_sum;
				feature.push_back(element);
			}
		}
	}
}

void MultiHist(Mat3f image, vector<float>& feature) {
	const int bins = 8;
	int div = 256 / bins;
	int dim3[3] = { bins, bins, bins };
	Mat HistUp = Mat::zeros(3, dim3, CV_32S);
	Mat HistDown = Mat::zeros(3, dim3, CV_32S);
	float normalizing_sum = image.rows * image.cols / 2;

	// upper histogram
	for (int row = 0; row < image.rows / 2; row++) {
		Vec3f* rowptr = image.ptr<Vec3f>(row);
		for (int col = 0; col < image.cols; col++) {
			int Rindex = int(rowptr[col][0]) / div;
			int Bindex = int(rowptr[col][1]) / div;
			int Gindex = int(rowptr[col][2]) / div;
			HistUp.at<int>(Rindex, Bindex, Gindex) += 1;
		}
	}
	// lower histogram
	for (int row = image.rows / 2; row < image.rows; row++) {
		Vec3f* rowptr = image.ptr<Vec3f>(row);
		for (int col = 0; col < image.cols; col++) {
			int Rindex = int(rowptr[col][0]) / div;
			int Bindex = int(rowptr[col][1]) / div;
			int Gindex = int(rowptr[col][2]) / div;
			HistDown.at<int>(Rindex, Bindex, Gindex) += 1;
		}
	}

	// Normalizing the histogram and adding to feature
	for (int x = 0; x < bins; x++) {
		for (int y = 0; y < bins; y++) {
			for (int z = 0; z < bins; z++) {
				float element = float(HistUp.at<int>(x, y, z)) / normalizing_sum;
				feature.push_back(element);
			}
		}
	}
	for (int x = 0; x < bins; x++) {
		for (int y = 0; y < bins; y++) {
			for (int z = 0; z < bins; z++) {
				float element = float(HistDown.at<int>(x, y, z)) / normalizing_sum;
				feature.push_back(element);
			}
		}
	}
}

int saveIntoCsv(int featureNum) {
	string directory = "../olympus";
	std::ofstream file;
	if (featureNum == 1) {
		file.open("features1.csv");
	}
	if (featureNum == 2) {
		file.open("features2.csv");
	}
	if (featureNum == 3) {
		file.open("features3.csv");
	}
	for (const auto &filee : fs::directory_iterator(directory)) {
		string img_name = filee.path().filename().string();
		Mat3f image = cv::imread(filee.path().string(), IMREAD_COLOR);
		vector<float> feature;
		if (featureNum == 1) {
			feature9x9(image, feature);
		}
		if (featureNum == 2) {
			Hist3D(image, feature);
		}
		if (featureNum == 3) {
			MultiHist(image, feature);
		}
		file << img_name << ",";
		for (int i = 0; i < feature.size(); i++) {
			if (i == feature.size() - 1) {
				file << feature[i] << "\n";
			}
			else {
				file << feature[i] << ",";
			}
		}
	}
	file.close();
	cout << "DONE SAVING FEATURE " << featureNum << endl;
 	return 0;
}

void loadCSVintoArray(int featureNum, vector<vector<float>> &features) {
	std::ifstream file;
	if (featureNum == 1) {
		file.open("features1.csv");
	}
	if (featureNum == 2) {
		file.open("features2.csv");
	}
	if (featureNum == 3) {
		file.open("features3.csv");
	}
	std::string line;

	while (getline(file, line)) {
		istringstream ss(line);
		vector<float> row_data;
		string cell;
		int col = 1;
		while (std::getline(ss, cell, ',')) {
			if (col == 1) {
				row_data.push_back(stof(cell.substr(4, 4)));
			}
			if (col >= 2) {
				float data = stof(cell);
				row_data.push_back(data);
			}
			col++;
		}
		features.push_back(row_data);
	}
}

float sumSquaredError(vector<float> targetFeatures, vector<float> comparisonFeatures) {
	float sum = 0;
	for (int element = 0; element < targetFeatures.size(); element++) {
		sum += ((targetFeatures[element] - comparisonFeatures[element]) * (targetFeatures[element] - comparisonFeatures[element]));
	}
	return sum;
}

float histIntersection(vector<float> targetFeatures, vector<float> comparisonFeatures) {
	float sum = 0;
	for (int element = 0; element < targetFeatures.size(); element++) {
		sum += min(targetFeatures[element], comparisonFeatures[element]);
	}
	return (1 - sum);
}

vector<vector<float>> sortDistance(vector<vector<float>> distanceArray) {
	int m = distanceArray.size();
	int n = distanceArray[0].size();
	sort(distanceArray.begin(), distanceArray.end(), sortcol);
	return distanceArray;
}

bool sortcol(const vector<float>& v1, const vector<float>& v2)
{
	return v1[1] < v2[1];
}