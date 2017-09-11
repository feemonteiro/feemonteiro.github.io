#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat alpha_mat, alpha_complement;

double alfa, alpha, center, focus, _decay;
int center_slider = 0;
int center_slider_max = 100;

int focus_slider = 0;
int focus_slider_max = 100;

int decay_slider = 0;
int decay_slider_max = 100;

Mat image1, image2, blended, mod1, mod2;
Mat imageTop;

char TrackbarName[50];

void DoAllTheStuff(){
    alpha_mat = Mat::zeros(image1.rows, image1.cols, CV_32F);
    alpha_complement = Mat::zeros(image1.rows, image1.cols, CV_32F);

    for(int i = 0; i < image1.rows; i++){
        alpha = (tanh((i+center-focus)/_decay) - tanh((i-center-focus)/_decay))/2;
        for(int j = 0; j < image1.cols; j++){
            alpha_mat.at<float>(i, j) = alpha;
            alpha_complement.at<float>(i, j) = 1 - alpha;
        }
    }
    Mat miracle_maker1[] = {alpha_mat, alpha_mat, alpha_mat};
    merge(miracle_maker1, 3, alpha_mat);
    Mat miracle_maker2[] = {alpha_complement, alpha_complement, alpha_complement};
    merge(miracle_maker2, 3, alpha_complement);

    image1.convertTo(mod1, CV_32FC3);
    image2.convertTo(mod2, CV_32FC3);

    multiply(mod1, alpha_mat, mod1);
    multiply(mod2, alpha_complement, mod2);
    add(mod1, mod2, blended);
    blended.convertTo(blended, CV_8UC3);
    imshow("addweighted", blended);
}

void on_trackbar_center(int, void*){
    center = (double) image1.rows*center_slider/center_slider_max ;
    DoAllTheStuff();
}

void on_trackbar_focus(int, void*){
    focus = (double) image1.rows*focus_slider/focus_slider_max ;
    DoAllTheStuff();
}

void on_trackbar_decay(int, void*){
    if (_decay < 1)
        _decay = 1;
    else
        _decay = (double) image1.rows*decay_slider/decay_slider_max ;

    DoAllTheStuff();
}



float kernel2d[] = {.1,.1,.1,
                    .1,.1,.1,
                    .1,.1,.1};

int main(int argvc, char** argv){
  image1 = imread("C:\\Users\\Obluda\\Documents\\qt-projects\\pdi-inversor-cor\\natal.jpg");
  //image2 = imread("C:\\Users\\Obluda\\Documents\\qt-projects\\pdi-inversor-cor\\forte-troca-regioes.jpg");
  Mat mask(3, 3, CV_32F);
  mask = Mat(3, 3, CV_32F, kernel2d);
  filter2D(image1, image2, 0, mask);
  image2.copyTo(imageTop);
  namedWindow("addweighted", 1);

  sprintf( TrackbarName, "Center x %d", center_slider_max );
  createTrackbar( TrackbarName, "addweighted",
                  &center_slider,
                  center_slider_max,
                  on_trackbar_center );
  on_trackbar_center(center_slider, 0 );

  sprintf( TrackbarName, "Focus x %d", focus_slider_max );
  createTrackbar( TrackbarName, "addweighted",
                  &focus_slider,
                  focus_slider_max,
                  on_trackbar_focus );
  on_trackbar_focus(focus_slider, 0 );

  sprintf( TrackbarName, "Decay x %d", decay_slider_max );
  createTrackbar( TrackbarName, "addweighted",
                  &decay_slider,
                  decay_slider_max,
                  on_trackbar_decay );
  on_trackbar_decay(decay_slider, 0 );

  waitKey(0);
  return 0;
}
