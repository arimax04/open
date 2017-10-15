#include <opencv2/opencv.hpp>
#include <stdio.h>
#define WIDTH 640
#define HEIGHT 320
int size_of_mosaic = 0;

int main(int argc, char *argv[])
{
  // 1. load classifier
  //std::string cascadeName = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml"; //Haar-like
  std::string cascadeName="/home/max/open/Hand.Cascade.1.xml";
  cv::CascadeClassifier cascade;
  if(!cascade.load(cascadeName)){
    printf("ERROR: cascadeFile not found\n");
    return -1;
  }
  
  // 2. initialize VideoCapture
  cv::Mat frame;
  cv::VideoCapture cap;
  cap.open(0);
  cap >> frame;
  
  // 3. prepare window and trackbar
  cv::namedWindow("result", 1);
  cv::createTrackbar("size","result",&size_of_mosaic,30,0);
  cv::Mat background=cv::imread("fruits.jpg");
  
  double scale = 4.0;
  cv::Mat gray, smallImg(cv::saturate_cast<int>(frame.rows/scale),
               cv::saturate_cast<int>(frame.cols/scale), CV_8UC1);
  
  cv::Mat hsv_skin_img=cv::Mat(cv::Size(frame.rows/scale,frame.cols/scale),CV_8UC1);
  cv::Mat smooth_img;
  cv::Mat hsv_img;

  for(;;){
    
    // 4. capture frame
    cap >> frame;
    // convert to gray scale
    cv::cvtColor(frame, gray, CV_BGR2GRAY);
    
    // 5. scale-down the image
    cv::resize(gray,smallImg,smallImg.size(),0,0,cv::INTER_LINEAR);
    cv::equalizeHist(smallImg,smallImg);

    //5.5 hsv
    hsv_skin_img=cv::Scalar(0,0,0);
    cv::medianBlur(frame,smooth_img,7);//ノイズがあるので平滑化
    cv::cvtColor(smooth_img,hsv_img,CV_BGR2HSV);//HSVに変換
     cv::inRange(hsv_img,cv::Scalar(0,58,88),cv::Scalar(25,173,229),hsv_skin_img);
    
    // 6. detect face using Haar-classifier
    std::vector<cv::Rect> faces;
    // multi-scale face searching
    // image, size, scale, num, flag, smallest rect
    clock_t start= clock();
    //cascade.detectMultiScale(smallImg,faces,1.1,2,CV_HAAR_SCALE_IMAGE,cv::Size(30,30));
    cascade.detectMultiScale(hsv_skin_img,faces,1.1,2,CV_HAAR_SCALE_IMAGE,cv::Size(30,30));
    clock_t end=clock();
    /*
    std::cout<<"duration="<<(double)(end-start)<<"sec.\n";
    start=clock();
    cascade.detectMultiScale(smallImg,faces,1.01,2,CV_HAAR_SCALE_IMAGE,cv::Size(30,30));
    end=clock();
    std::cout<<"scaleFactor dicrease.duration="<<(double)(end-start)<<"sec.\n";
    start=clock();
    cascade.detectMultiScale(smallImg,faces,1.1,10,CV_HAAR_SCALE_IMAGE,cv::Size(30,30));
    end=clock();
    std::cout<<"Neighbors increase.duration="<<(double)(end-start)<<"sec.\n";
    start=clock();
    cascade.detectMultiScale(smallImg,faces,100,3,CV_HAAR_SCALE_IMAGE,cv::Size(30,30));
    end=clock();
    std::cout<<"scaleFactor increase.duration="<<(double)(end-start)<<"sec.\n\n\n\n\n";
    */  
/* hatena */
    /* hatena */
    /* hatena */
    /* hatena */

    // 7. mosaic(pixelate) face-region
    for(int i = 0; i < faces.size(); i++){
      cv::Point center;
      int radius;
      center.x = cv::saturate_cast<int>((faces[i].x + faces[i].width * 0.5) * scale);
      center.y = cv::saturate_cast<int>((faces[i].y + faces[i].height * 0.5) * scale);
      radius = cv::saturate_cast<int>((faces[i].width + faces[i].height) * 0.25 * scale);
      
      // mosaic
      if(size_of_mosaic < 1) size_of_mosaic = 1;
      cv::Rect roi_rect(center.x-radius,center.y-radius,radius*2,radius*2);
      cv::Mat mosaic=frame(roi_rect);
      cv::Mat tmp;
      cv::resize(mosaic,tmp,cv::Size(radius/size_of_mosaic,radius/size_of_mosaic),0,0);
      cv::resize(tmp,mosaic,cv::Size(radius*2,radius*2),0,0,CV_INTER_NN);
      /* hatena */
      
      /*
      if(size_of_mosaic<1) size_of_mosaic=1;
       cv::Rect roi_rect(center.x-radius,center.y-radius,radius*2,radius*2);
      cv::Mat mosaic=frame(roi_rect);
      resize(background,mosaic,cv::Size(roi_rect.width,roi_rect.height));
      */
    }
    
    // 8. show mosaiced image to window
    cv::imshow("result", frame);

    int key = cv::waitKey(10);
    if(key == 'q' || key == 'Q')
        break;

  }
 return 0;
}

