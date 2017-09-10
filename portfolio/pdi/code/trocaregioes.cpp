#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image, auxiliar, trocado;
  Vec3b val;
  int meioX, meioY;

  image= imread("great.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "imagem nao abriu" << endl;

  namedWindow("troca",WINDOW_AUTOSIZE);
  
  imshow("troca", image);  
  waitKey();

  image.copyTo(trocado);

  meioX = trocado.size().height/2;
  meioY = trocado.size().width/2;

  auxiliar = trocado(Rect(0,0,meioX, meioY));
  image(Rect(meioX, meioY, meioX, meioY)).copyTo(auxiliar);
  auxiliar = trocado(Rect(meioX, meioY, meioX, meioY));
  image(Rect(0, 0, meioX, meioY)).copyTo(auxiliar);

  auxiliar = trocado(Rect(meioX, 0, meioX, meioY));
  image(Rect(0, meioY, meioX, meioY)).copyTo(auxiliar);
  auxiliar = trocado(Rect(0, meioY, meioX, meioY));
  image(Rect(meioX, 0, meioX, meioY)).copyTo(auxiliar);

  
  imshow("troca", trocado);
  imwrite("trocado.png", trocado);
  waitKey();

  return 0;
}
