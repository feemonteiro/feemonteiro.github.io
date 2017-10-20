#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>
#include <sstream>

using namespace std;
using namespace cv;

#define STEP 10
#define JITTER 3
#define RAIO 3

Mat CannyMod(Mat image, int threshold){
   Mat border;
   Canny(image, border, threshold, 3*threshold);
   return border;
}

int main(int argc, char** argv){
  vector<int> yrange;
  vector<int> xrange;

  Mat image, bg, points, tmp;

  int width, height, gray, newstep;
  int x, y;

  // carrega imagem (mudar para testar)
  image= imread("C:\\Users\\Obluda\\Pictures\\hum.jpg",CV_LOAD_IMAGE_GRAYSCALE);

  srand(time(0));

  if(!image.data){
    cout << "nao abriu" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }

  width=image.size().width;
  height=image.size().height;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  points = Mat(height, width, CV_8U, Scalar(255));
  bg = Mat(height, width, CV_8U, Scalar(255));

  random_shuffle(xrange.begin(), xrange.end());

  // fundo
  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
              x = i+rand()%(2*JITTER)-JITTER+1;
              y = j+rand()%(2*JITTER)-JITTER+1;
              gray = image.at<uchar>(x,y);
              circle(points,
                     cv::Point(y,x),
                     6,
                     CV_RGB(gray,gray,gray),
                     -1,
                     CV_AA);
        }
  }
  bg = points.clone();
  //points.deallocate();

  // somador: varia a mudança de raios e do threshold de canny
  int adder = 10;

  // loop testa vários somadores 10 ao limite do laço
  while(adder <= 50){
      //points = Mat(height, width, CV_8U, Scalar(255));
      points = bg.clone();
      // loop relativo ao somador
      for (int now = 10; now < 200; now+=adder){
          newstep = 5*(210 - now)/100;

          xrange.resize(height/newstep);
          yrange.resize(width/newstep);
          iota(xrange.begin(), xrange.end(), 0);
          iota(yrange.begin(), yrange.end(), 0);
          for(uint i=0; i<xrange.size(); i++) xrange[i]= xrange[i]*newstep+newstep/2;
          for(uint i=0; i<yrange.size(); i++) yrange[i]= yrange[i]*newstep+newstep/2;

          // faz o canny pra cada thrsehold
          tmp = CannyMod(image, now);
          ostringstream output;
          output << "cannyx" << adder << ".jpg";
          imwrite(output.str(), tmp);

          // adicionada os círculos caso haja borda detectada
          // também varia o raio do círculo por threshold (raio = rayman)
          for(auto i : xrange){
            random_shuffle(yrange.begin(), yrange.end());
            for(auto j : yrange){
              if(int(tmp.at<uchar>(i,j)) > 0){
                  x = i+rand()%(2*JITTER)-JITTER+1;
                  y = j+rand()%(2*JITTER)-JITTER+1;
                  if (x >= xrange.size()) continue;
                  if (y >= yrange.size()) continue;
                  gray = image.at<uchar>(x,y);
                  circle(points,
                         cv::Point(y,x),
                         7*(210-now)/100,
                         CV_RGB(gray,gray,gray),
                         -1,
                         CV_AA);
              }
            }
          }

      }
      // salva os arquivos pra cada loop
      ostringstream output;
      output << "pontosx" << adder << ".jpg";
      imwrite(output.str(), points);
      adder += 10;
      //points.deallocate();
  }

  return 0;
}
