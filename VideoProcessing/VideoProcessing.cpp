#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include <stdint.h>
#include <iostream>
#include <fstream>

using namespace cv;

bool isOdd(size_t value)
{
	return value % 2 == 1;
}

uint8_t sortAndGetMedian(std::vector<uint8_t> & values)
{
	std::sort(values.begin(), values.end());

	if (isOdd(values.size()))
	{
		return values[values.size() / 2];
	}

	return (values[values.size() / 2 - 1] + values[values.size() / 2 - 1]) / 2;
}

// Takes all values of an image between input indexes and returns their median value.
uint8_t imagePartToMedianValue(const Mat & image, int up, int down, int left, int right)
{
	std::vector<uint8_t> values;
	values.reserve((right - left) * (down - up));

	for (int r = up; r < down; r++)
	{
		for (int c = left; c < right; c++)	
		{
			values.push_back(image.at<uint8_t>(r, c));
		}
	}
	return sortAndGetMedian(values);
}

int main(int argc, char** argv)
{
	if (argc < 4)
	{
		std::cout << "Not enough arguments" << std::endl;
		return -1;
	}

	const char* videoName = argv[1];
	const int colPartitions = atoi(argv[2]);
	const int rowPartitions = atoi(argv[3]);

	if (colPartitions <= 0 || rowPartitions <= 0)
	{
		std::cout << "Invalid number of desired partitions" << std::endl;
		return -1;
	}

	VideoCapture cap(videoName);

	if (!cap.isOpened())
	{
		std::cout << "Error opening video file" << std::endl;
		return -1;
	}

	std::ofstream myfile("result.csv");
	// Write a timestamp at the beginning of the first line.
	myfile << (cap.get(CV_CAP_PROP_POS_MSEC) / 1000.0);

	Mat image;
	cap >> image;

	// Based on the dimensions input as parameters, we get the number of pixels per dimension.
	// We use integer division to get the same amount of pixels for every cell.
	// This might cut off pixels at the end.
	const int partCols = image.cols / colPartitions;
	const int partRows = image.rows / rowPartitions;

	while (!image.empty())
	{
		cvtColor(image, image, COLOR_BGR2GRAY);
		for (int r = 0; r < rowPartitions; r++)
		{
			for (int c = 0; c < colPartitions; c++)
			{
				// Write a new median number for every partition cell.
				myfile << ",";
				const double result = imagePartToMedianValue(image, r * partRows, (r + 1) * partRows, c * partCols, (c + 1) * partCols);
				myfile << result;
			}
		}

		// End line and write another timestamp.
		myfile << "\n";
		myfile << (cap.get(CV_CAP_PROP_POS_MSEC) / 1000.0);
		

		cap >> image;
	}

	myfile.close();
	return 0;
}