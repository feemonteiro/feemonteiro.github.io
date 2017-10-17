#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image) {
	Mat tmp, A, B, C, D;

	// se a imagem tiver tamanho impar, recorta a regiao para
	// evitar cópias de tamanho desigual
	image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
	int cx = image.cols / 2;
	int cy = image.rows / 2;

	// reorganiza os quadrantes da transformada
	// A B   ->  D C
	// C D       B A
	A = image(Rect(0, 0, cx, cy));
	B = image(Rect(cx, 0, cx, cy));
	C = image(Rect(0, cy, cx, cy));
	D = image(Rect(cx, cy, cx, cy));

	// A <-> D
	A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);

	// C <-> B
	C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}

int main(int, char**) {
	//VideoCapture cap;
	//Mat image, filteredImg;
	Mat imaginaryInput, complexImage, multsp;
	Mat padded, filter, mag;
	Mat image, imagegray, tmp;
	Mat_<float> realInput, zeros;
	vector<Mat> planos;
	float yl = 9, yh = 30, d0 = 43, d = 0; //parametros do filtro homomorfico

	// guarda tecla capturada
	char key;

	// valores ideais dos tamanhos da imagem
	// para calculo da DFT
	int dft_M, dft_N;

	// abre a câmera default
	/*cap.open(0);
	if (!cap.isOpened())
		return -1;*/

	image = imread("C:\\img\\noite.png", CV_LOAD_IMAGE_GRAYSCALE);
	if (!image.data) {
		cout << "imagem nao carregou corretamente\n";
		return(-1);
	}


	// identifica os tamanhos otimos para
	// calculo do FFT
	dft_M = getOptimalDFTSize(image.rows);
	dft_N = getOptimalDFTSize(image.cols);

	// realiza o padding da imagem
	copyMakeBorder(image, padded, 0,
		dft_M - image.rows, 0,
		dft_N - image.cols,
		BORDER_CONSTANT, Scalar::all(0));

	// parte imaginaria da matriz complexa (preenchida com zeros)
	zeros = Mat_<float>::zeros(padded.size());

	// prepara a matriz complexa para ser preenchida
	complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

	// a função de transferência (filtro frequencial) deve ter o
	// mesmo tamanho e tipo da matriz complexa
	filter = complexImage.clone();

	// cria uma matriz temporária para criar as componentes real
	// e imaginaria do filtro ideal
	tmp = Mat(dft_M, dft_N, CV_32F);

	// prepara o filtro homomorfico
	for (int i = 0; i<dft_M; i++) {
		for (int j = 0; j<dft_N; j++) {
			d = sqrt(pow(i - dft_M / 2.0, 2) + pow(i - dft_N / 2.0, 2));
			d = -exp(-pow(d / d0, 2));
			d = 1 + d;
			tmp.at<float>(i, j) = (yh - yl)*d + yl;
			
		}
	}

	// cria a matriz com as componentes do filtro e junta
	// ambas em uma matriz multicanal complexa
	Mat comps[] = { tmp, tmp };
	merge(comps, 2, filter);

		imagegray = image;
		imshow("original", image);

		// realiza o padding da imagem
		copyMakeBorder(imagegray, padded, 0,
			dft_M - image.rows, 0,
			dft_N - image.cols,
			BORDER_CONSTANT, Scalar::all(0));

		// limpa o array de matrizes que vao compor a
		// imagem complexa
		planos.clear();
		// cria a compoente real
		realInput = Mat_<float>(padded);

		// insere as duas componentes no array de matrizes
		planos.push_back(realInput);
		planos.push_back(zeros);

		// combina o array de matrizes em uma unica
		// componente complexa
		merge(planos, complexImage);

		// calcula o dft
		dft(complexImage, complexImage);

		// realiza a troca de quadrantes
		deslocaDFT(complexImage);

		// aplica o filtro frequencial
		mulSpectrums(complexImage, filter, complexImage, 0);

		// limpa o array de planos
		planos.clear();

		// troca novamente os quadrantes
		deslocaDFT(complexImage);

		cout << complexImage.size().height << endl;
		// calcula a DFT inversa
		idft(complexImage, complexImage);

		// imagem filtrada
		split(complexImage, planos);

		// normaliza a parte real para exibicao
		normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
		imshow("filtrada", planos[0]);

		key = (char)waitKey(0);
		
		imwrite("original.png", image);
		imwrite("filtrada.png", planos[0]);

	return 0;
}
