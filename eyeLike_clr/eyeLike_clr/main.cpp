#include "stdafx.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <queue>
#include <stdio.h>
#include <math.h>
#include <SFML/Audio.hpp>
#include <iomanip>
#include<string>

#include "constants.h"
#include "findEyeCenter.h"
#include "findEyeCorner.h"
using namespace std;
using namespace cv;










#using <System.dll>

using namespace System;
using namespace System::IO::Ports;
using namespace System::Threading;

public ref class PortChat
{
private:
	static bool _continue;
	static SerialPort^ _serialPort;
	static Thread^ readThread;


public:

	static void Main()
	{
		System::String^ name;
		
		StringComparer^ stringComparer = StringComparer::OrdinalIgnoreCase;
		 
		// Create a new SerialPort object with default settings.
		_serialPort = gcnew SerialPort();

		// Allow the user to set the appropriate properties.
		_serialPort->PortName = "COM7";//SetPortName(_serialPort->PortName);
		_serialPort->BaudRate = 115200;//SetPortBaudRate(_serialPort->BaudRate);
		// _serialPort->Handshake = SetPortHandshake(_serialPort->Handshake);

		// Set the read/write timeouts
		_serialPort->ReadTimeout = 500;
		_serialPort->WriteTimeout = 500;

		_serialPort->Open();                      //Connect the serial port
		_continue = true;
		
	}

	static void close() // closes the serial port
	{
		_serialPort->Close();
	}

	static void write(char c)  //writes message on to the epuck via serial port communication
	{
		System::String^ message;
		if(c=='L')
			message = "L";//c+"";
		else if(c=='R')
			message = "R";
		else  if(c=='F')
			message="F";
		else if(c=='B')
			message="B";
			
	_serialPort->WriteLine(System::String::Format("<{0}>: {1}", "Epuck", message));
	
	}

	static void Read()   
	{
		while (_continue)
		{
			try
			{
				System::String^ message = _serialPort->ReadLine();
				Console::WriteLine(message);
			}
			catch (TimeoutException ^) {}
		}
	}

	static System::String^ SetPortName(System::String^ defaultPortName)
	{
		System::String^ portName;

		Console::WriteLine("Available Ports:");
		for each (System::String^ s in SerialPort::GetPortNames())
		{
			Console::WriteLine("   {0}", s);
		}

		Console::Write("COM port({0}): ", defaultPortName);
		portName = Console::ReadLine();

		if (portName == "")
		{
			portName = defaultPortName;
		}
		return portName;
	}

	static Int32 SetPortBaudRate(Int32 defaultPortBaudRate)
	{
		System::String^ baudRate;

		Console::Write("Baud Rate({0}): ", defaultPortBaudRate);
		baudRate = Console::ReadLine();

		if (baudRate == "")
		{
			baudRate = defaultPortBaudRate.ToString();
		}

		return Int32::Parse(baudRate);
	}

	static Parity SetPortParity(Parity defaultPortParity)
	{
		System::String^ parity;

		Console::WriteLine("Available Parity options:");
		for each (System::String^ s in Enum::GetNames(Parity::typeid))
		{
			Console::WriteLine("   {0}", s);
		}

		Console::Write("Parity({0}):", defaultPortParity.ToString());
		parity = Console::ReadLine();

		if (parity == "")
		{
			parity = defaultPortParity.ToString();
		}

		return (Parity)Enum::Parse(Parity::typeid, parity);
	}

	static Int32 SetPortDataBits(Int32 defaultPortDataBits)
	{
		System::String^ dataBits;

		Console::Write("Data Bits({0}): ", defaultPortDataBits);
		dataBits = Console::ReadLine();

		if (dataBits == "")
		{
			dataBits = defaultPortDataBits.ToString();
		}

		return Int32::Parse(dataBits);
	}

	static StopBits SetPortStopBits(StopBits defaultPortStopBits)
	{
		System::String^ stopBits;

		Console::WriteLine("Available Stop Bits options:");
		for each (System::String^ s in Enum::GetNames(StopBits::typeid))
		{
			Console::WriteLine("   {0}", s);
		}

		Console::Write("Stop Bits({0}):", defaultPortStopBits.ToString());
		stopBits = Console::ReadLine();

		if (stopBits == "")
		{
			stopBits = defaultPortStopBits.ToString();
		}

		return (StopBits)Enum::Parse(StopBits::typeid, stopBits);
	}

	static Handshake SetPortHandshake(Handshake defaultPortHandshake)
	{
		System::String^ handshake;

		Console::WriteLine("Available Handshake options:");
		for each (System::String^ s in Enum::GetNames(Handshake::typeid))
		{
			Console::WriteLine("   {0}", s);
		}

		Console::Write("Handshake({0}):", defaultPortHandshake.ToString());
		handshake = Console::ReadLine();

		if (handshake == "")
		{
			handshake = defaultPortHandshake.ToString();
		}

		return (Handshake)Enum::Parse(Handshake::typeid, handshake);
	}
};






/** Constants **/


/** Function Headers */
void detectAndDisplay( cv::Mat frame );
pair<int, int> getPupilCoord(cv::Mat frame);

void playSound(string fileName)  // function to play sound for Left , Right and UP
{
    sf::SoundBuffer buffer;

	//-- Note, either copy these two files(left.wav,right.wav,up.wav) to your current folder, or change these locations
    if (!buffer.loadFromFile("C:/Users/Saurabh Satnalika/Desktop/SFML-2.3.2-windows-vc10-32-bit/SFML-2.3.2/examples/sound/resources/"+fileName))
        return;

    sf::Sound sound(buffer);
    sound.play();

    while (sound.getStatus() == sf::Sound::Playing)
    {
        sf::sleep(sf::milliseconds(100));
    }
  }

void taskForLeft()      // function called when eye is detected at LEFT
{
	string l="left.wav";
	playSound(l);
	printf("Pupil on left\n");
	PortChat::write('L');      // writes 'L' onto the epuck via serial port communication
}

void taskForRight()   // function called when eye is detected at RIGHT
{
	string r="right.wav";
	playSound(r);
	printf("Pupil on right\n");
    PortChat::write('R');
}

void taskForUp()    // function called when eye is detected at UP
{
	string u="up.wav";
	playSound(u);
	printf("Pupil on Up\n");
    PortChat::write('F');
}

/** Global variables */
//-- Note, either copy these two files from opencv/data/haarscascades to your current folder, or change these locations
cv::String face_cascade_name = "C:\\Users\\Saurabh Satnalika\\Desktop\\eyeLike-master\\eyeLike-master\\res\\haarcascade_frontalface_alt.xml";
cv::CascadeClassifier face_cascade;
std::string main_window_name = "Capture - Face detection";
std::string face_window_name = "Capture - Face";
cv::RNG rng(12345);
cv::Mat debugImage;
cv::Mat skinCrCbHist = cv::Mat::zeros(cv::Size(256, 256), CV_8UC1);

int rightRight;
int rightLeft;
int leftRight;
int leftLeft;
int centerLeft;
int centerRight;

int choice;

int frameNumber;
char previous;



void showMenu()   // menu displayed
{
	cout << "\t\tMenu" << endl;
	cout << "***********" << endl;
	cout << "1. Manual Caliberation" << endl;
	cout << "2. I am in a hurry" << endl;    // please press 2 for normal execution
}

void caliberate()
{
	CvCapture* capture;
	cv::Mat frame;

	capture = cvCaptureFromCAM(1);

	if (capture) {
			
			for (int i = 0; i < 10; i++)
				frame = cvQueryFrame(capture);


			vector<int> leftEye;
			vector<int> rightEye;

			playSound("left.wav");
			cout << "See left" << endl;
			for (int i = 0; i < 10; i++)
			{
				frame = cvQueryFrame(capture);
				cv::flip(frame, frame, 1);
				frame.copyTo(debugImage);

				// Apply the classifier to the frame
				if (!frame.empty()) {
					pair<int, int> left = getPupilCoord(frame);
					leftEye.push_back(left.first);
					rightEye.push_back(left.second);
				}
				else {
					printf(" --(!) No captured frame -- Break!");
					break;
				}
				imshow(main_window_name, debugImage);

			}
			sort(leftEye.begin(), leftEye.end());
			sort(rightEye.begin(), rightEye.end());

			leftLeft = leftEye[leftEye.size() / 2];
			leftRight = rightEye[rightEye.size() / 2];

			

			playSound("right.wav");
			cout << "See right" << endl;

			leftEye.clear();
			rightEye.clear();
			cv::Mat calImage;
			for (int i = 0; i < 10; i++)
			{
				frame = cvQueryFrame(capture);
				cv::flip(frame, frame, 1);
				frame.copyTo(debugImage);

				// Apply the classifier to the frame
				if (!frame.empty()) {
					pair<int, int> right = getPupilCoord(frame);
					leftEye.push_back(right.first);
					rightEye.push_back(right.second);
				}
				else {
					printf(" --(!) No captured frame -- Break!");
					break;
				}
				imshow(main_window_name, debugImage);

				calImage = debugImage;

			}
			sort(leftEye.begin(), leftEye.end());
			sort(rightEye.begin(), rightEye.end());

			rightLeft = leftEye[leftEye.size() / 2];
			rightRight = rightEye[rightEye.size() / 2];

			cout << "Caliberating done.." << endl;
			
			line(calImage, cv::Point( 10, leftLeft), cv::Point(100,leftLeft), 200);
			line(calImage, cv::Point(10, rightLeft), cv::Point(100, rightLeft), 200);
			line(calImage, cv::Point(10, leftRight), cv::Point( 100, leftRight), 200);
			line(calImage, cv::Point(10, rightRight), cv::Point(100, rightRight), 200);
			imshow("caliberated image", calImage);
		}
	else
		cout << "Sorry, camera not found" << endl;
}

int main()                 // main
{
	PortChat::Main();
	frameNumber = -1;
	previous = 'l';

	CvCapture* capture;
	cv::Mat frame;

	if (!face_cascade.load(face_cascade_name)){ printf("--(!)Error loading fac cascade, please change face_cascade_name in source code.\n"); return -1; };
	cv::namedWindow(main_window_name, CV_WINDOW_NORMAL);
	cv::moveWindow(main_window_name, 400, 100);
	cv::namedWindow(face_window_name, CV_WINDOW_NORMAL);
	cv::moveWindow(face_window_name, 10, 100);
	cv::namedWindow("Right Eye", CV_WINDOW_NORMAL);
	cv::moveWindow("Right Eye", 10, 600);
	cv::namedWindow("Left Eye", CV_WINDOW_NORMAL);
	cv::moveWindow("Left Eye", 10, 800);
	cv::namedWindow("aa", CV_WINDOW_NORMAL);
	cv::moveWindow("aa", 10, 800);
	cv::namedWindow("aaa", CV_WINDOW_NORMAL);
	cv::moveWindow("aaa", 10, 800);
	cv::namedWindow("caliberated image", CV_WINDOW_NORMAL);


	createCornerKernels();
	ellipse(skinCrCbHist, cv::Point(113, 155.6), cv::Size(23.4, 15.2),
		43.0, 0.0, 360.0, cv::Scalar(255, 255, 255), -1);
	
	showMenu();

	cin >> choice;

	if (choice == 1)
	{
		caliberate();
		centerLeft = (leftLeft + rightLeft) / 2;
		centerRight = (leftRight + rightRight) / 2;
	}
	
	// Read the video stream
	capture = cvCaptureFromCAM(1);

	if (capture) {
		
		for (int i = 0; i < 10; i++)
			frame = cvQueryFrame(capture);


		while (true) {
			frame = cvQueryFrame(capture);
			// mirror it
			cv::flip(frame, frame, 1);
			frame.copyTo(debugImage);

			// Apply the classifier to the frame
			if (!frame.empty()) {
				frameNumber = (frameNumber + 1) % NO_OF_FRAMES;
				detectAndDisplay(frame);
			}
			else {
				printf(" --(!) No captured frame -- Break!");
				break;
			}

			imshow(main_window_name, debugImage);

			int c = cv::waitKey(10);
			if ((char)c == 'c') { break; }
			if ((char)c == 'f') {
				imwrite("frame.png", frame);
			}

		}
	}

	releaseCornerKernels();
	return 0;
}

void findEyes(cv::Mat frame_gray, cv::Rect face) {
  cv::Mat faceROI = frame_gray(face);
  cv::Mat debugFace = faceROI;

  if (kSmoothFaceImage) {
    double sigma = kSmoothFaceFactor * face.width;
    GaussianBlur( faceROI, faceROI, cv::Size( 0, 0 ), sigma);
  }
  //-- Find eye regions and draw them
  int eye_region_width = face.width * (kEyePercentWidth/100.0);
  int eye_region_height = face.width * (kEyePercentHeight/100.0);
  int eye_region_top = face.height * (kEyePercentTop/100.0);
  cv::Rect leftEyeRegion(face.width*(kEyePercentLeftSide/100.0),
                         eye_region_top,eye_region_width,eye_region_height);
  cv::Rect rightEyeRegion(face.width - eye_region_width - face.width*(kEyePercentRightSide/100.0),
                          eye_region_top,eye_region_width,eye_region_height);

  //-- Find Eye Centers
  cv::Point leftPupil = findEyeCenter(faceROI,leftEyeRegion,"Left Eye");
  cv::Point rightPupil = findEyeCenter(faceROI,rightEyeRegion,"Right Eye");
 // cout <<"left pupil centre"<<leftPupil.x<<endl;
  // get corner regions
  cv::Rect leftRightCornerRegion(leftEyeRegion);
  leftRightCornerRegion.width -= leftPupil.x;
  leftRightCornerRegion.x += leftPupil.x;
  leftRightCornerRegion.height /= 2;
  leftRightCornerRegion.y += leftRightCornerRegion.height / 2;
  cv::Rect leftLeftCornerRegion(leftEyeRegion);
  leftLeftCornerRegion.width = leftPupil.x;
 // cout <<"left corner region width "<<leftLeftCornerRegion.width<<endl;
  leftLeftCornerRegion.height /= 2;
  leftLeftCornerRegion.y += leftLeftCornerRegion.height / 2;
  cv::Rect rightLeftCornerRegion(rightEyeRegion);
  rightLeftCornerRegion.width = rightPupil.x;
  rightLeftCornerRegion.height /= 2;
  rightLeftCornerRegion.y += rightLeftCornerRegion.height / 2;
  cv::Rect rightRightCornerRegion(rightEyeRegion);
  rightRightCornerRegion.width -= rightPupil.x;
  rightRightCornerRegion.x += rightPupil.x;
  rightRightCornerRegion.height /= 2;
  rightRightCornerRegion.y += rightRightCornerRegion.height / 2;
  rectangle(debugFace,leftRightCornerRegion,200);
  rectangle(debugFace,leftLeftCornerRegion,200);
  rectangle(debugFace,rightLeftCornerRegion,200);
  rectangle(debugFace,rightRightCornerRegion,200);
  // change eye centers to face coordinates
  rightPupil.x += rightEyeRegion.x;
  rightPupil.y += rightEyeRegion.y;
  leftPupil.x += leftEyeRegion.x;
  leftPupil.y += leftEyeRegion.y;
  // draw eye centers
  circle(debugFace, rightPupil, 3, 1234);
  circle(debugFace, leftPupil, 3, 1234);



  if (choice == 2){
	  if (rightPupil.x >= rightEyeRegion.x && rightPupil.x <= rightEyeRegion.x + leftWinPercent*rightEyeRegion.width && leftPupil.x >= leftEyeRegion.x && leftPupil.x <= leftEyeRegion.x + leftWinPercent*leftEyeRegion.width) // checks if both the pupils is in the left region
	  {
		  if (frameNumber == NO_OF_FRAMES - 1 && previous == 'l')  // checks that the pupils have been pointing to the left region for 'NO_OF_FRAMES' times continously
		  {														// for eg if 'NO_OF_FRAMES is 3 then the pupils of both the eyes must be pointing to the left region for 3 continous frames
			  taskForLeft();  
		  }
		  else if (previous != 'l')
		  {
			  frameNumber = -1;
		  }
		  previous = 'l';

	  }
	  //checks if both the pupils is in the right region
	  if (rightPupil.x <= rightEyeRegion.x + rightEyeRegion.width && rightPupil.x >= rightEyeRegion.x + (1 - rightWinPercent)*rightEyeRegion.width && leftPupil.x <= leftEyeRegion.x + leftEyeRegion.width && leftPupil.x >= leftEyeRegion.x + (1 - rightWinPercent)*leftEyeRegion.width)
	  {

		  if (frameNumber == NO_OF_FRAMES - 1 && previous == 'r')
		  {
			  taskForRight();
		  }
		  else if (previous != 'r')
		  {
			  frameNumber = -1;
		  }
		  previous = 'r';

	  }
	  //checks if both the pupils is in the UP region
	  if (abs(rightPupil.y - rightLeftCornerRegion.y) <= 0.13*rightEyeRegion.height && abs(leftPupil.y - leftLeftCornerRegion.y) <= 0.13*leftEyeRegion.height)
	  {
		  if (frameNumber == NO_OF_FRAMES - 1 && previous == 'u')
		  {
			  taskForUp();
		  }
		  else if (previous != 'u')
		  {
			  frameNumber = -1;
		  }
		  previous = 'u';

	  }
  }
  else
  {
	  int medLeftLeft = leftEyeRegion.x + leftLeft;
	  int medLeftRight = rightEyeRegion.x + leftRight;
	  int medRightLeft = leftEyeRegion.x + rightLeft;
	  int medRightRight = rightEyeRegion.x + rightRight;

	  centerRight = (medLeftRight + medRightRight) / 2;
	  centerLeft =  (medLeftLeft + medRightLeft) / 2;

	  line(debugFace, Point(medLeftLeft,100), Point(medLeftLeft,200), 200);
	  line(debugFace, Point(medLeftRight, 100), Point(medLeftRight, 200), 200);
	  line(debugFace, Point(medRightLeft, 100), Point(medRightLeft, 200), 200);
	  line(debugFace, Point(medRightRight, 100), Point(medRightRight, 200), 200);

	  if (rightPupil.x <= medLeftRight + linePercent*(centerRight-medLeftRight) && leftPupil.x <= medLeftLeft + linePercent*(centerLeft-medLeftLeft))
	  {
		  if (frameNumber == NO_OF_FRAMES - 1 && previous == 'l')
		  {
			  taskForLeft();
		  }
		  else if (previous != 'l')
		  {
			  frameNumber = -1;
		  }
		  previous = 'l';

	  }

	  if (rightPupil.x >= medRightRight - linePercent*(medRightRight-centerRight) && leftPupil.x >= medRightLeft - linePercent*(medRightLeft- centerLeft))
		  {

		  if (frameNumber == NO_OF_FRAMES - 1 && previous == 'r')
		  {
			  taskForRight();
		  }
		  else if (previous != 'r')
		  {
			  frameNumber = -1;
		  }
		  previous = 'r';

	  }

	  if (abs(rightPupil.y - rightLeftCornerRegion.y) <= 0.13*rightEyeRegion.height && abs(leftPupil.y - leftLeftCornerRegion.y) <= 0.13*leftEyeRegion.height)
	  {
		  if (frameNumber == NO_OF_FRAMES - 1 && previous == 'u')
		  {
			  taskForUp();
		  }
		  else if (previous != 'u')
		  {
			  frameNumber = -1;
		  }
		  previous = 'u';

	  }
  }

  //-- Find Eye Corners
 if(kEnableEyeCorner)
 {
    cv::Point2f leftRightCorner = findEyeCorner(faceROI(leftRightCornerRegion), true, false);
    leftRightCorner.x += leftRightCornerRegion.x;
    leftRightCorner.y += leftRightCornerRegion.y;
    cv::Point2f leftLeftCorner = findEyeCorner(faceROI(leftLeftCornerRegion), true, true);
    leftLeftCorner.x += leftLeftCornerRegion.x;
    leftLeftCorner.y += leftLeftCornerRegion.y;
    cv::Point2f rightLeftCorner = findEyeCorner(faceROI(rightLeftCornerRegion), false, true);
    rightLeftCorner.x += rightLeftCornerRegion.x;
    rightLeftCorner.y += rightLeftCornerRegion.y;
    cv::Point2f rightRightCorner = findEyeCorner(faceROI(rightRightCornerRegion), false, false);
    rightRightCorner.x += rightRightCornerRegion.x;
    rightRightCorner.y += rightRightCornerRegion.y;
    circle(faceROI, leftRightCorner, 3, 200);
    circle(faceROI, leftLeftCorner, 3, 200);
    circle(faceROI, rightLeftCorner, 3, 200);
    circle(faceROI, rightRightCorner, 3, 200);

	
 }
  imshow(face_window_name, faceROI);

}


cv::Mat findSkin (cv::Mat &frame) {
  cv::Mat input;
  cv::Mat output = cv::Mat(frame.rows,frame.cols, CV_8U);

  cvtColor(frame, input, CV_BGR2YCrCb);

  for (int y = 0; y < input.rows; ++y) {
    const cv::Vec3b *Mr = input.ptr<cv::Vec3b>(y);
//    uchar *Or = output.ptr<uchar>(y);
    cv::Vec3b *Or = frame.ptr<cv::Vec3b>(y);
    for (int x = 0; x < input.cols; ++x) {
      cv::Vec3b ycrcb = Mr[x];
//      Or[x] = (skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) > 0) ? 255 : 0;
      if(skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) == 0) {
        Or[x] = cv::Vec3b(0,0,0);
      }
    }
  }
  return output;
}

/**
 * @function detectAndDisplay
 */
void detectAndDisplay( cv::Mat frame ) {
  std::vector<cv::Rect> faces;
  //cv::Mat frame_gray;

  std::vector<cv::Mat> rgbChannels(3);
  cv::split(frame, rgbChannels);
  cv::Mat frame_gray = rgbChannels[2];

 
  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE|CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(150, 150) );
//  findSkin(debugImage);

  for( int i = 0; i < faces.size(); i++ )
  {
    rectangle(debugImage, faces[i], 1234);
  }
  //-- Show what you got
  if (faces.size() > 0) {
    findEyes(frame_gray, faces[0]);
  }
}

pair<int,int> getPupil(cv::Mat frame_gray, cv::Rect face) {
	cv::Mat faceROI = frame_gray(face);
	cv::Mat debugFace = faceROI;

	if (kSmoothFaceImage) {
		double sigma = kSmoothFaceFactor * face.width;
		GaussianBlur(faceROI, faceROI, cv::Size(0, 0), sigma);
	}
	//-- Find eye regions and draw them
	int eye_region_width = face.width * (kEyePercentWidth / 100.0);
	int eye_region_height = face.width * (kEyePercentHeight / 100.0);
	int eye_region_top = face.height * (kEyePercentTop / 100.0);
	cv::Rect leftEyeRegion(face.width*(kEyePercentLeftSide / 100.0),
		eye_region_top, eye_region_width, eye_region_height);
	cv::Rect rightEyeRegion(face.width - eye_region_width - face.width*(kEyePercentRightSide / 100.0),
		eye_region_top, eye_region_width, eye_region_height);

	//-- Find Eye Centers
	cv::Point leftPupil = findEyeCenter(faceROI, leftEyeRegion, "Left Eye");
	cv::Point rightPupil = findEyeCenter(faceROI, rightEyeRegion, "Right Eye");
	// cout <<"left pupil centre"<<leftPupil.x<<endl;
	// get corner regions
	cv::Rect leftRightCornerRegion(leftEyeRegion);
	leftRightCornerRegion.width -= leftPupil.x;
	leftRightCornerRegion.x += leftPupil.x;
	leftRightCornerRegion.height /= 2;
	leftRightCornerRegion.y += leftRightCornerRegion.height / 2;
	cv::Rect leftLeftCornerRegion(leftEyeRegion);
	leftLeftCornerRegion.width = leftPupil.x;
	// cout <<"left corner region width "<<leftLeftCornerRegion.width<<endl;
	leftLeftCornerRegion.height /= 2;
	leftLeftCornerRegion.y += leftLeftCornerRegion.height / 2;
	cv::Rect rightLeftCornerRegion(rightEyeRegion);
	rightLeftCornerRegion.width = rightPupil.x;
	rightLeftCornerRegion.height /= 2;
	rightLeftCornerRegion.y += rightLeftCornerRegion.height / 2;
	cv::Rect rightRightCornerRegion(rightEyeRegion);
	rightRightCornerRegion.width -= rightPupil.x;
	rightRightCornerRegion.x += rightPupil.x;
	rightRightCornerRegion.height /= 2;
	rightRightCornerRegion.y += rightRightCornerRegion.height / 2;
	rectangle(debugFace, leftRightCornerRegion, 200);
	rectangle(debugFace, leftLeftCornerRegion, 200);
	rectangle(debugFace, rightLeftCornerRegion, 200);
	rectangle(debugFace, rightRightCornerRegion, 200);
	// change eye centers to face coordinates
/*	rightPupil.x += rightEyeRegion.x;
	rightPupil.y += rightEyeRegion.y;
	leftPupil.x += leftEyeRegion.x;
	leftPupil.y += leftEyeRegion.y;
	// draw eye centers
	circle(debugFace, rightPupil, 3, 1234);
	circle(debugFace, leftPupil, 3, 1234);
	
	imshow(face_window_name, faceROI);*/
	return make_pair(leftPupil.x, rightPupil.x);
}

pair<int, int> getPupilCoord(cv::Mat frame)
{
	std::vector<cv::Rect> faces;
	std::vector<cv::Mat> rgbChannels(3);
	cv::split(frame, rgbChannels);
	cv::Mat frame_gray = rgbChannels[2];
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(150, 150));
	for (int i = 0; i < faces.size(); i++)
	{
		rectangle(debugImage, faces[i], 1234);
	}
	if (faces.size() > 0) {
		return getPupil(frame_gray, faces[0]);
	}
	return make_pair(-1, -1);
}