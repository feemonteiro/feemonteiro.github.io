#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

unsigned int equalizer[256];

Mat equalize2(Mat img, Mat hist, int width, int height){
    equalizer[0] = hist.at<uint>(0);
    Mat output = img.clone();
    int pos;
    for(int i = 1; i < 256; i++){
        equalizer[i] = equalizer[i-1] + hist.at<uint>(i);
        //cout << i << " " << equalizer[i] << endl;
    }

    for(int i = 0; i < 256; i++){
        equalizer[i] = equalizer[i]*255/(width*height);
    }
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++){
            pos = img.at<uchar>(i,j);
            output.at<uchar>(i,j) = equalizer[pos];
        }
    return output;
}

int main(int argc, char** argv){
  Mat image, img2, c_image;
  int width, height;
  VideoCapture cap;
  Mat hist, eqhist;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

  cap.open(0);

  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImg(histh, histw, CV_BGR2GRAY, Scalar(0,0,0));
  Mat eqhistImg(histh, histw, CV_BGR2GRAY, Scalar(0,0,0));

  while(1){
    cap >> c_image;
    cvtColor(c_image, image, CV_BGR2GRAY);

    calcHist(&image, 1, 0, Mat(), hist, 1,
             &nbins, &histrange,
             uniform, false);

    normalize(hist, hist, 0, histImg.rows, NORM_MINMAX, -1, Mat());

    histImg.setTo(Scalar(0));

    for(int i=0; i<nbins; i++){
      line(histImg,
           Point(i, histh),
           Point(i, histh-cvRound(hist.at<float>(i))),
           Scalar(255, 255, 255), 1, 8, 0);
    }

    //img2 = equalize2(image, hist, width, height);
    equalizeHist(image, img2);
    calcHist(&img2, 1, 0, Mat(), eqhist, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(eqhist, eqhist, 0, eqhistImg.rows, NORM_MINMAX, -1, Mat());
    eqhistImg.setTo(Scalar(0));

    for(int i=0; i<nbins; i++){
      line(eqhistImg,
           Point(i, histh),
           Point(i, histh-cvRound(eqhist.at<float>(i))),
           Scalar(255, 255, 255), 1, 8, 0);
    }

    histImg.copyTo(image(Rect(0, 0, nbins, histh)));
    eqhistImg.copyTo(img2(Rect(0, 0, nbins, histh)));

    imshow("image", image);
    imshow("equalized image", img2);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
