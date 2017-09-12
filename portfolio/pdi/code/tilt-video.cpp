#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat alpha_mat, alpha_complement;
double alpha, center, focus, _decay;
Mat image1, image2, blended, mod1, mod2;
Mat imageTop;

// do all the tilt-shifting stuff
void DoAllTheStuff() {
	alpha_mat = Mat::zeros(image1.rows, image1.cols, CV_32F);
	alpha_complement = Mat::zeros(image1.rows, image1.cols, CV_32F);

	for (int i = 0; i < image1.rows; i++) {
		alpha = (tanh((i + (center / 2) - focus) / _decay) - tanh((i - (center / 2) - focus) / _decay)) / 2;
		for (int j = 0; j < image1.cols; j++) {
			alpha_mat.at<float>(i, j) = alpha;
			alpha_complement.at<float>(i, j) = 1 - alpha;
		}
	}
	Mat miracle_maker1[] = { alpha_mat, alpha_mat, alpha_mat };
	merge(miracle_maker1, 3, alpha_mat);
	Mat miracle_maker2[] = { alpha_complement, alpha_complement, alpha_complement };
	merge(miracle_maker2, 3, alpha_complement);

	image1.convertTo(mod1, CV_32FC3);
	image2.convertTo(mod2, CV_32FC3);

	multiply(mod1, alpha_mat, mod1);
	multiply(mod2, alpha_complement, mod2);
	add(mod1, mod2, blended);
	blended.convertTo(blended, CV_8UC3);
}
// median filter
float kernel2d[] = { .111,.111,.111,
.111,.111,.111,
.111,.111,.111 };

int main(int argc, char** argv) {
	Mat mask(3, 3, CV_32F);
	mask = Mat(3, 3, CV_32F, kernel2d);
	image2.copyTo(imageTop);
	int width, height;
	VideoCapture cap;
	cout << cap.get(CV_CAP_PROP_FOURCC) << endl;
	cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('P', 'I', 'M', '1'));

	cap.open("anteseng.avi");

	if (!cap.isOpened()) {
		cout << "cameras indisponiveis";
		return -1;
	}

	int count = cap.get(CV_CAP_PROP_FRAME_COUNT);
	width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	Size framesize(width, height);

	center = (double)height*0.2;
	focus = (double)height*0.5;
	_decay = (double)height*0.01;

	VideoWriter out("depois_nai.avi", CV_FOURCC('P', 'I', 'M', '1'), 20, framesize, true);

	cout << "largura = " << width << endl;
	cout << "altura  = " << height << endl;

	for (int i = 0; i < count; i++) {
		if (i % 3 != 0) { // to discard 2 of each 3 frames
			cap.read(image1);
			continue;
		}
		cap.read(image1); // reads frame
		if (image1.empty())
			continue;
		filter2D(image1, image2, 0, mask); // creates blurred copy
		DoAllTheStuff(); // tilt-shifts
		out << blended; // writes output
						//if(waitKey(30) >= 0) break;
	}

	return 0;
}