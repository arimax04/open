#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <stdio.h>
#include <typeinfo>
#include <GL/glut.h>
void init_glut(int ,char*);

#define WIDTH 640
#define HEIGHT 320

void init_glut(int argc,char* argv[]){
   glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(640, 640);
  glutCreateWindow("TEST");
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 640, 640, 0.0);
  glViewport(0, 0, 640, 640);
}

int main(int argc,char *argv[]){
  init_glut(argc,argv);
  std::cout<<"version:" << CV_VERSION << std::endl;
  cv::VideoCapture cap(1);
  cap.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT);
  if(!cap.isOpened()){
    cap.open(0);
    if(!cap.isOpened()){
      printf("かめらが検出されませんでした。");
      exit(0);
    }
  }
  
  cv::Mat input_img;
  cv::Mat hsv_skin_img=cv::Mat(cv::Size(WIDTH,HEIGHT),CV_8UC1);
  cv::Mat smooth_img;
  cv::Mat hsv_img;
  cv::Mat distance_img;
  cv::vector< cv::vector< cv::Point > > contours;
  cv::namedWindow("input_img",CV_WINDOW_AUTOSIZE);
  double maxarea;
  int idx;
  int maxid;
  std::vector< cv::Point > storage;
   
  //背景差分
  /*
  cv::Mat im,gray,bg,diff,dst;
  int th = 20;
  cap >> im;
  cv::cvtColor(im,bg,CV_BGR2GRAY);
  */
  while(1){
    /*  
    cap>>im;
    cv::cvtColor(im,gray,CV_BGR2GRAY);
    cv::absdiff(gray,bg,diff);
    cv::threshold(diff,dst,th,255,CV_THRESH_BINARY);
    cv::imshow("Camera",dst);
    if(cv::waitKey(1)){
      cv::cvtColor(im,bg,CV_BGR2GRAY);
    }
    if(cv::waitKey(1)==27){
      break;
    }
    */
      
    maxarea=0;
    idx=0;
    maxid;
    hsv_skin_img=cv::Scalar(0,0,0);
    cap>>input_img;
    cv::medianBlur(input_img,smooth_img,7);//ノイズがあるので平滑化
    cv::cvtColor(smooth_img,hsv_img,CV_BGR2HSV);//HSVに変換
    //cv::cvtColor(smooth_img,distance_img,CV_BGR2HSV);
    cv::inRange(hsv_img,cv::Scalar(0,58,88),cv::Scalar(25,173,229),hsv_skin_img);
    //cv::inRange(distance_img,cv::Scalar(0,58,89),cv::Scalar(25,173,229),distance_img);
    //cv::cvtColor(distance_img,distance_img,CV_HSV2BGR);
    cv::distanceTransform(hsv_skin_img,distance_img,CV_DIST_L2,3);
    cv::findContours(hsv_skin_img,contours,CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
    std::vector<cv::Point> hull(contours.size());
    for(int i=0;i<contours.size();i++){
      if(maxarea<=contourArea(contours[i])) {
	maxid=i;
	maxarea=contourArea(contours[i]);
      }
    }
    
    //cv::polylines(input_img,contours[maxid],true,cv::Scalar(0,255,0),2);
    cv::Rect rectOfArea=cv::boundingRect(contours[maxid]);
    cv::rectangle(input_img,rectOfArea.tl(),rectOfArea.br(),1,3);
    cv::circle(input_img,rectOfArea.tl(),10,cv::Scalar(0.5,0,0),5,8,0);
    
    cv::convexHull(contours[maxid],hull);
    //std::cout << hull[0] << std::endl;
    if(!hull.empty()){
      for(auto h=hull.begin();h!=hull.end();h++){
	cv::circle(distance_img,*h,10,cv::Scalar(0.5,0,0),5,8,0);
      }
    }
    
    std::cout << maxarea<<":"<<maxid<<std::endl;
    
    cv::imshow("input_img",input_img);
    cv::imshow("distance_img",distance_img);
    
    if(cv::waitKey(30)>=0){
      break;
    }
    
  }
}
