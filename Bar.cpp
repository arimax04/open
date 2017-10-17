#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define WINDOW_X (650)
#define WINDOW_Y (490)
#define WINDOW_NAME "test5"
#define TEXTURE_HEIGHT (480)
#define TEXTURE_WIDTH (640)
#define MAX_ENEMIES 100


double delta=1/60.0;

GLdouble centerx=0.5,centery=0.5;
GLdouble loseRange=0.1;
GLdouble fieldRange=2;
GLdouble cameraDistance=10;
int score=0;

class mysphere{
private:
   GLdouble pos[2];//位置 0:x 1:y
  GLdouble vec[2];//速度 0:x 1:y
  double dt=delta;
  double radius=0.1;
  double accelerate=1.05;
  bool conflictcounter=true;
  std::string tag;
public:
 
  mysphere(){
    pos[0]=pos[1]=vec[0]=vec[1]=0;
    tag="";
  }
  mysphere(GLdouble posx,GLdouble posy,GLdouble vecx,GLdouble vecy,std::string t){
    pos[0]=posx;
    pos[1]=posy;
    vec[0]=vecx;
    vec[1]=vecy;
    tag=t;
  }
  GLdouble getradius(){
    return radius;
  }
  GLdouble* getpos(){
    return pos;
  }
  GLdouble getposx(){
    return pos[0];
  }
  GLdouble getposy(){
    return pos[1];
  }
  GLdouble* getvec(){
    return vec;
  }
  GLdouble getvecx(){
    return vec[0];
  }
  GLdouble getvecy(){
    return vec[1];
  }
  void setpos(GLdouble x,GLdouble y){
    pos[0]=x;
    pos[1]=y;
  }
  void setposx(GLdouble x){
    pos[0]=x;
  }
  void setposy(GLdouble y){
    pos[1]=y;
  }
  void setvec(GLdouble x,GLdouble y){
    vec[0]=x;
    vec[1]=y;
  }
  void setvecx(GLdouble x){
    vec[0]=x;
  }
  void setvecy(GLdouble y){
    vec[1]=y;
  }
  std::string gettag(){
    return tag;
  }
  void update(){
    dt=delta;
    pos[0]+=vec[0]*dt;
    pos[1]+=vec[1]*dt;
    if(pos[0]>fieldRange+centerx||pos[0]<-fieldRange+centerx){
      vec[0]*=-1;
    }
    if(pos[1]>fieldRange+centery||pos[1]<-fieldRange+centerx){
      vec[1]*=-1;
    }
    if(std::pow(pos[0]-centerx,2)+std::pow(pos[1]-centery,2)<=std::pow(loseRange+radius,2)){
      std::cout<<"You lose!!" <<std::endl;
      std::cout<<"Your Score is "<< score<<"."<<std::endl;
      exit(1);
    }
    glColor3d(128/255.0,0,128/255.0);
    glTranslatef(-pos[0],-pos[1],0.0);
    glutSolidSphere(radius,16,16);
  }
  void updateforPlayer(){
    vec[0]=0;vec[1]=0;
    glColor3d(0,255/255.0,255/255.0);
    glTranslatef(-pos[0],-pos[1],0.0);
    glutSolidSphere(radius,16,16);
  }
  void calcConflict(mysphere& s){
    if(std::pow(pos[0]-s.getposx(),2)+std::pow(pos[1]-s.getposy(),2)<=std::pow(radius+s.getradius(),2)){
      if(conflictcounter){
	conflictcounter=false;
	double r=std::sqrt(std::pow(pos[0]-s.getposx(),2)+std::pow(pos[1]-s.getposy(),2));//位置ベクトルの大きさ
	GLdouble p[]={//球の中心座標をむく単位位置ベクトル
	  (s.getposx()-pos[0])/r,
	  (s.getposy()-pos[1])/r
	};
        if(s.gettag()=="player"){
	  s.setvecx(-p[0]*2);
	  s.setvecy(-p[1]*2);
	}
	GLdouble pv[]={//上の単位ベクトルに垂直なベクトル
	  (s.getposy()-pos[1])/r,
	  -(s.getposx()-pos[0])/r
	};
	GLdouble vr1=vec[0]*p[0]+vec[1]*p[1];//自球から相球への衝突前の速度ベクトル
	GLdouble vr2=s.getvecx()*p[0]+s.getvecy()*p[1];//相球から自球への衝突前の速度ベクトル
	GLdouble va1[]={//自球から相球への衝突後の速度ベクトル
	  p[0]*vr2*accelerate,
	  p[1]*vr2*accelerate
	};
	GLdouble va2[]={//相球から自球への衝突後の速度ベクトル
	  p[0]*vr1*accelerate,
	  p[1]*vr1*accelerate
	};
	//pos[0]+=radius/2*(va1[0]);
	//pos[1]+=radius/2*(va1[1]);
	vec[0]=va1[0]+pv[0]*vec[0];
	vec[1]=va1[1]+pv[1]*vec[1];
	s.setvecx(va2[0]+pv[0]*s.getvecx());
	s.setvecy(va2[1]+pv[1]*s.getvecy());
	//s.setposx(s.getposx()+radius/2*va2[0]);
	//s.setposy(s.getposy()+radius/2*va2[1]);
	if(s.gettag()=="player"){
	  s.setvecx(0);
	  s.setvecy(0);
	  score+=10;
	}
      }
    }else{
      conflictcounter=true;
    }
  }
};

void init_GL(int argc, char *argv[]);
void init();
void init_CV();
void set_callback_functions();

void counter(int);
void glut_display();
void glut_keyboard(unsigned char key, int x, int y);
void glut_mouse(int button, int state, int x, int y);
void glut_motion(int x, int y);
void glut_idle();
void draw_pyramid();
void set_texture();
void draw_playerRange();
void draw_background();
// グローバル変数 opengl
double g_angle1 = 0.0;
double g_angle2 = -3.141592 / 6;
double g_distance = 10.0;
bool g_isLeftButtonOn = false;
bool g_isRightButtonOn = false;
GLuint g_TextureHandles[3] = {0,0,0};
mysphere player;
mysphere enemies[MAX_ENEMIES];
int nowenemies=0;
int timecounter=1;
//opencv
cv::Mat   frame;
cv::VideoCapture cap;
cv::Mat input_img;
cv::Mat hsv_skin_img=cv::Mat(cv::Size(TEXTURE_WIDTH,TEXTURE_HEIGHT),CV_8UC1);
cv::Mat smooth_img;
cv::Mat hsv_img;
cv::Mat distance_img;
cv::vector< cv::vector< cv::Point > > contours;
double maxarea;
int idx;
int maxid;
double mainx=0,mainy=0;
double playerx=0,playery;



int main(int argc, char *argv[]){
  /* OpenGLの初期化 */
  init_GL(argc,argv);

  /* このプログラム特有の初期化 */
  init();

  /* コールバック関数の登録 */
  set_callback_functions();
  
  /*タイマー*/
  glutTimerFunc(100,counter,0);
  /* メインループ */
  glutMainLoop();

  return 0;
}

void counter(int c){
  timecounter++;
  glutTimerFunc(100,counter,0);
}

void init_GL(int argc, char *argv[]){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(WINDOW_X,WINDOW_Y);
  glutCreateWindow(WINDOW_NAME);
}

void init(){
  //initcv
  cap.open(1);
  if(!cap.isOpened()){
    std::cout<<"error!"<<std::endl;
    exit(0);
  }
  
  //initgl
  glClearColor(0.2, 0.2, 0.2, 0.2);
  glGenTextures(3,g_TextureHandles);
  
  for(int i = 0; i < 3; i++){
    glBindTexture(GL_TEXTURE_2D,g_TextureHandles[i]);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,TEXTURE_WIDTH,TEXTURE_HEIGHT,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  }

  set_texture();
  player=mysphere(0,0,0,0,"player");
  enemies[nowenemies]=mysphere(1.5,1.5,1,1,"enemy");
  nowenemies++;
}

void set_callback_functions(){
  glutDisplayFunc(glut_display);
  glutKeyboardFunc(glut_keyboard);
  glutMouseFunc(glut_mouse);
  glutMotionFunc(glut_motion);
  glutPassiveMotionFunc(glut_motion);
  glutIdleFunc(glut_idle);
}

void glut_keyboard(unsigned char key, int x, int y){
  switch(key){
  case 'q':
  case 'Q':
  case '\033':
    exit(0);
  }
  glutPostRedisplay();
}

void glut_mouse(int button, int state, int x, int y){
  if(button == GLUT_LEFT_BUTTON){
    if(state == GLUT_UP){
      g_isLeftButtonOn = false;
    }else if(state == GLUT_DOWN){
      g_isLeftButtonOn = true;
    }
  }

  if(button == GLUT_RIGHT_BUTTON){
    if(state == GLUT_UP){
      g_isRightButtonOn = false;
    }else if(state == GLUT_DOWN){
      g_isRightButtonOn = true;
    }
  }
}
 
void glut_motion(int x, int y){
  static int px = -1, py = -1;
  if(g_isLeftButtonOn == true){
    if(px >= 0 && py >= 0){
      g_angle1 += (double)-(x - px)/20;
      g_angle2 += (double)(y - py)/20;
    }
    px = x;
    py = y;
  }else if(g_isRightButtonOn == true){
    if(px >= 0 && py >= 0){
      g_distance += (double)(y - py)/20;
    }
    px = x;
    py = y;
  }else{
    px = -1;
    py = -1;
  }
  glutPostRedisplay();
}

void glut_display(){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, 1.0, 0.1, 100);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(-centerx,-centery,cameraDistance,-centerx,-centery,-1, 0,1,0);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  
  //衝突判定
  for(int i=0;i<nowenemies;i++){
    glPushMatrix();
    for(int j=i+1;j<nowenemies;j++){
      enemies[i].calcConflict(enemies[j]);
    }
    enemies[i].calcConflict(player);
    enemies[i].update();
    
    glPopMatrix();
  }
  
  glPushMatrix();
  //glTranslatef(-mainx,-mainy,0.0);
  //glScalef(0.1,0.1,0.1);
  //draw_pyramid();
  //glutSolidSphere(0.1,16,16);
  //draw_main();
  player.setpos((mainx+playerx)/2,(mainy+playery)/2);
  playerx=mainx;
  playery=mainy;
  player.updateforPlayer();
  glPopMatrix();
  
  glPushMatrix();
  glColor3d(0,0,0);
  glTranslatef(-centerx,-centery,0);
  glutSolidSphere(loseRange,16,16);
  glPopMatrix();
  
  glPushMatrix();
  std::string Sscore=std::to_string(score);
  int size=(int)Sscore.size();
  glRasterPos2f(100,100);
  for(int i=0;i<size;i++){
    char ic=Sscore[i];
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15,ic);
  glPopMatrix();
  }  
  glPushMatrix();
  draw_background();
  glPopMatrix();
  
  glPushMatrix();
  draw_playerRange();
  glPopMatrix();  
  glFlush();
  glDisable(GL_DEPTH_TEST);
  glutSwapBuffers();

}

void glut_idle(){
  if(timecounter>100){
    timecounter-=100; 
    enemies[nowenemies]=mysphere(2,2,1,1,"enemy");
    nowenemies++;
  }
  cap>>frame;
  input_img=frame;
  cv::flip(input_img,input_img,1);
  cv::flip(input_img,input_img,-1);
  //cv::circle(input,cv::Point(200,100),10,cv::Scalar(0.5,0,0),5,8,0);
  maxarea=0;
  idx=0;
  maxid;
  hsv_skin_img=cv::Scalar(0,0,0);
  cv::medianBlur(input_img,smooth_img,7);
  cv::cvtColor(smooth_img,hsv_img,CV_BGR2HSV);
  cv::inRange(hsv_img,cv::Scalar(0,58,88),cv::Scalar(25,173,229),hsv_skin_img);
  cv::findContours(hsv_skin_img,contours,CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
  for(int i=0;i<contours.size();i++){
    if(maxarea<=contourArea(contours[i])) {
      maxid=i;
      maxarea=contourArea(contours[i]);
    }
  }
  cv::Rect rectOfArea=cv::boundingRect(contours[maxid]);
  cv::rectangle(input_img,rectOfArea.tl(),rectOfArea.br(),1,3);
  cv::Point middle;
  middle.x=(rectOfArea.tl().x+rectOfArea.br().x)/2;
  middle.y=(rectOfArea.tl().y+rectOfArea.br().y)/2;
  mainx=double(middle.x)/TEXTURE_WIDTH;
  mainy=double(middle.y)/TEXTURE_HEIGHT;
  //mainx=double(rectOfArea.tl().x)/TEXTURE_WIDTH;
  //mainy=double(rectOfArea.tl().y)/TEXTURE_HEIGHT;
  cv::circle(input_img,middle,10,cv::Scalar(0.5,0,0),5,8,0);
  cv::cvtColor(input_img,input_img,CV_BGR2RGB);
  glBindTexture(GL_TEXTURE_2D,0);
  glBindTexture(GL_TEXTURE_2D,g_TextureHandles[0]);
  glTexSubImage2D(GL_TEXTURE_2D,0,(TEXTURE_WIDTH-input_img.cols)/2,(TEXTURE_HEIGHT-input_img.rows)/2,input_img.cols,input_img.rows,GL_RGB,GL_UNSIGNED_BYTE,input_img.data);
glBindTexture(GL_TEXTURE_2D,g_TextureHandles[0]);
  
  glutPostRedisplay();
}
void draw_playerRange(){
  GLdouble pointO[] = { -(-fieldRange+centerx),  -(-fieldRange+centery), 0};
  GLdouble pointA[] = {-(fieldRange+centerx),  -(-fieldRange+centery), 0};
  GLdouble pointB[] = {-(fieldRange+centerx), -(fieldRange+centery), 0};
  GLdouble pointC[] = { -(-fieldRange+centerx), -(fieldRange+centery), 0};
  glBindTexture(GL_TEXTURE_2D,g_TextureHandles[1]);
  
  glEnable(GL_TEXTURE_2D);
  
  glColor3d(0.5,0.5,0.5);
  glBegin(GL_POLYGON);
  glTexCoord2d(0.0,0.0);
  glVertex3dv(pointO);
  glTexCoord2d(1.0,0.0);
  glVertex3dv(pointA);
  glTexCoord2d(1.0,1.0);
  glVertex3dv(pointB);
  glTexCoord2d(0.0,1.0);
  glVertex3dv(pointC);
  glEnd();
  
  glDisable(GL_TEXTURE_2D);
  
  
}
  void draw_background(){
  GLdouble pointO[] = { 0,  0, 0};
  GLdouble pointA[] = {-1,  0, 0};
  GLdouble pointB[] = {-1, -1, 0};
  GLdouble pointC[] = { 0, -1, 0};
  
  glEnable(GL_TEXTURE_2D);
  glColor3d(0.5,0.5,0.5);
  glBegin(GL_POLYGON);
  glTexCoord2d(0.0,0.0);
  glVertex3dv(pointO);
  glTexCoord2d(1.0,0.0);
  glVertex3dv(pointA);
  glTexCoord2d(1.0,1.0);
  glVertex3dv(pointB);
  glTexCoord2d(0.0,1.0);
  glVertex3dv(pointC);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  
  
}
  
void draw_pyramid(){
  GLdouble pointO[] = {0.0, 1.0, 0.0};
  GLdouble pointA[] = {1.5, -1.0, 1.5};
  GLdouble pointB[] = {-1.5, -1.0, 1.5};
  GLdouble pointC[] = {-1.5, -1.0, -1.5};
  GLdouble pointD[] = {1.5, -1.0, -1.5};
  
  
  glColor3d(1.0, 0.0, 0.0);
  glBegin(GL_TRIANGLES);
  glVertex3dv(pointO);
  glVertex3dv(pointA);
  glVertex3dv(pointB);
  glEnd();

  glColor3d(1.0, 1.0, 0.0);
  glBegin(GL_TRIANGLES);
  glVertex3dv(pointO);
  glVertex3dv(pointB);
  glVertex3dv(pointC);
  glEnd();

  
  glColor3d(0.0, 1.0, 1.0);
  glBegin(GL_TRIANGLES);
  glVertex3dv(pointO);
  glVertex3dv(pointC);
  glVertex3dv(pointD);
  glEnd();
  
  glPushMatrix();
  glColor3d(1.0, 0.0, 1.0);
  glBegin(GL_TRIANGLES);
  glVertex3dv(pointO);
  glVertex3dv(pointD);
  glVertex3dv(pointA);
  glEnd();
  glColor3d(1.0, 1.0, 1.0);

  glBegin(GL_POLYGON); 
  glVertex3dv(pointA);
  glVertex3dv(pointB);
  glVertex3dv(pointC);
  glVertex3dv(pointD);
  glEnd();  
}

void set_texture(){
  char *inputFileNames[3] = {"baboon.jpg", "www.jpg", "building.jpg"};
  for(int i = 0; i < 3; i++){
    cv::Mat input=cv::imread(inputFileNames[i],1);
    cv::resize(input,input,cv::Size(),640.0/input.cols,480.0/input.rows);
    // BGR -> RGBの変換
    cv::cvtColor(input,input,CV_BGR2RGB);

    glBindTexture(GL_TEXTURE_2D,g_TextureHandles[i]);
    glTexSubImage2D(GL_TEXTURE_2D,0,(TEXTURE_WIDTH-input.cols)/2,(TEXTURE_HEIGHT-input.rows)/2,input.cols,input.rows,GL_RGB,GL_UNSIGNED_BYTE,input.data);
    
  }    
}

