#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <GL/glut.h>

static void display(void)
{
  cv::Mat img = cv::imread("test.jpg");
  cv::flip(img, img, 0);
  cv::cvtColor(img, img, CV_BGR2RGB);

  glClear(GL_COLOR_BUFFER_BIT);
  glDrawPixels(img.cols, img.rows,
               GL_RGB, GL_UNSIGNED_BYTE, img.data);
  glFlush();
}

static void keyboard(unsigned char key, int x, int y)
{
  switch(key) {
  case 'q':
  case 'Q':
  case '\033':
    exit(1);
  default:
    break;
  }
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(640, 640);
  glutCreateWindow("TEST");

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 640, 640, 0.0);
  glViewport(0, 0, 640, 640);

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  
  return 0;
}

/*
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
	Mat im, gray, bg, diff, dst;
	int th = 50;					// 2値化の閾値
	VideoCapture cap(0);
	cap >> im;
	cvtColor(im, bg, CV_BGR2GRAY); // 最初のフレームを背景画像に設定

	while (1){
		cap >>im;
		cvtColor(im, gray, CV_BGR2GRAY);
		absdiff(gray, bg, diff);						// 現フレームと背景画像の差分を取得
		threshold(diff, dst, th, 255, THRESH_BINARY);	// 差分画像を閾値thで2値化
		imshow("Camera", dst);							// 結果を表示
		if (waitKey(1) >= 32){							// スペースキーが押されたら背景更新
			cvtColor(im, bg, CV_BGR2GRAY);
		}
		if (waitKey(1) == 27){							// Escキーが入力されたら終了
			break;     
		}

	}
	return 0;
}
*/
/*
#include <opencv2/opencv.hpp>

int main(int argc,char *argv[]){
  cv::VideoCapture cap(0);

	// 背景差分計算用オブジェクトの生成
	//cv::BackgroundSubtractorGMG backGroundSubtractor;
	//cv::BackgroundSubtractorMOG backGroundSubtractor;
	cv::BackgroundSubtractorMOG2 backGroundSubtractor;

	while (cv::waitKey(1) == -1)
	{
		cv::Mat frame, foreGroundMask, output;

		// 画像取得
		cap >> frame;
		if (frame.empty())
			break;

		// マスク画像の取得
		backGroundSubtractor(frame, foreGroundMask);
	
		// 入力画像にマスク処理を行う
		cv::bitwise_and(frame, frame, output, foreGroundMask);

		cv::imshow("output", foreGroundMask);
	}
}
*/
