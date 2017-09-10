#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  Vec3b val;
  int p[2][2];

  image= imread("great.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu imagem" << endl;

  for(int i = 0; i < 2; i++){
  	cout << "Insira a coordenanda P"<<i+1<<":\n";
  	cout << "x y\n";
  	for(int j = 0; j < 2; j++){
  		if(j == 0){
  			cin >> p[i][j];
  			while (p[i][j]>image.size().height){
  				cout << "coordenada inválida! Valor máximo de altura é "<<image.size().height<<endl<<"Insira novamente"<<endl;
  				cin >> p[i][j];
  			}
  		}else{
  			cin >> p[i][j];
  			while (p[i][j]>image.size().width){
  				cout << "coordenada inválida! Valor máximo de altura é "<<image.size().width<<endl<<"Insira novamente"<<endl;
  				cin >> p[i][j];
  			}
  		}
  	}

  		
  }

  cout << "Coordenadas \n";
  cout << "P1: ("<<p[0][0]<<","<<p[0][1]<<")\nP2: ("<<p[1][0]<<","<<p[1][1]<<")\n";

  namedWindow("janela",WINDOW_AUTOSIZE);

  for(int i=p[0][0];i<p[1][0];i++){
    for(int j=p[0][1];j<p[1][1];j++){
      image.at<uchar>(i,j)=255 - image.at<uchar>(i,j);
    }
  }
  
  imshow("janela", image);  
  imwrite("negativo.png", image);
  waitKey();

  return 0;
}
