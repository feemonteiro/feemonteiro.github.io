#include <iostream>
#include <cstdio>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  Vec3b val;

  image= imread("C:\\Users\\Obluda\\Documents\\qt-projects\\pdi-inversor-cor\\forte.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu forte.jpg" << endl;

  int p1_x, p1_y, p2_x, p2_y;
  printf("Insira as coordenadas do ponto P1: ");
  scanf("%d %d", &p1_x, &p1_y);
  printf("Insira as coordenadas do ponto P2: ");
  scanf("%d %d", &p2_x, &p2_y);

  namedWindow("janela",WINDOW_AUTOSIZE);

  for(int i = p1_y; i < p2_y; i++)
    for(int j = p1_x; j < p2_x; j++)
      image.at<uchar>(i,j) = 255 - image.at<uchar>(i,j);

  imshow("janela", image);
  waitKey();
}
