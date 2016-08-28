#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/videoio/videoio.hpp>

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

using namespace cv;
using namespace std;


Mat frame; //current video frame
Mat framepupil;
Rect croppedRectangle;
vector<Vec3f> circles;
int radius;
int x=1;
int y=1;

void movemouse(Point &Centroid)
{

}

void mousemove(int x_pos, int y_pos)
{
    ///Strings that will contain the conversions
    string xcord; string ycord;

    ///These are buffers or something? I don't really know... lol.
    stringstream sstr; stringstream sstr2;

    ///Conversion to regular string happens here
    sstr<<1.2*x_pos;//نقطه ایکس (محور طولی) برای شروع صفحه نمایش
    xcord = sstr.str();
    sstr2<<0.9*y_pos;//نقطه وای(محور عرضی-ارتفاع) برای شروع صفحه نمایش
    ycord = sstr2.str();

    ///Getting the command string
    string command = "xdotool mousemove " + xcord + " " + ycord;

    ///Converting command string to a form that system() accepts.
    const char *com = command.c_str();
    system(com);
}


int main()
{
	// Open the video file
	VideoCapture capture("/media/EyeWriter/eclipseCode/22-26/ok.MPG");
	if (!capture.isOpened())
		return 1;

	bool stop(false);

	int delay = 50;

	while (!stop)
	{

		if (!capture.read(frame))
			break;

		GaussianBlur( frame, frame, Size(1, 1), 1, 7 );
		Mat colorgray;
		Mat resizepupil;
		imshow ("org film",frame);

		cvtColor(frame, colorgray, COLOR_BGR2GRAY );

		colorgray.convertTo(colorgray,//افزایش کنتراست
							   -1,//عمق تصویر خروجی-اگر منفی باشد همانند تصویر ورودی خواهد شد
							   1,//عامل ضرب؛ ارزش هر پیکسل در این عدد ضرب خواهد شد
							   10); //این مقدار به پیکسهای ضرب شده اضافه می شود-هر چه بیشتر شود،تصویر سفیدتر می شود

		threshold(colorgray,colorgray, 50, 255, THRESH_BINARY_INV);

		Mat croppedRectangle1=colorgray(Rect(310,220,70,30));
	//	imshow ("croppedRectangle1",croppedRectangle1);//چشم


				//یافتن مردمک

				resize(croppedRectangle1, resizepupil, Size(colorgray.rows * 2, colorgray.cols * 1));


				Mat se(3, 3, CV_8U, Scalar(1));
				se.at<uchar>(0, 0) = 0;
				se.at<uchar>(0, 2) = 0;
				se.at<uchar>(2, 0) = 0;
				se.at<uchar>(2, 2) = 0;

				Mat se2(5, 5, CV_8U, Scalar(1));
				se2.at<uchar>(0, 0) = 0;
				se2.at<uchar>(0, 1) = 0;
				se2.at<uchar>(0, 3) = 0;
				se2.at<uchar>(0, 4) = 0;
				se2.at<uchar>(1, 0) = 0;
				se2.at<uchar>(1, 4) = 0;
				se2.at<uchar>(3, 0) = 0;
				se2.at<uchar>(3, 4) = 0;
				se2.at<uchar>(4, 0) = 0;
				se2.at<uchar>(4, 1) = 0;
				se2.at<uchar>(4, 3) = 0;
				se2.at<uchar>(4, 4) = 0;


					dilate(resizepupil, framepupil, se2,Point(-1,-1),10);
					erode(framepupil, framepupil, se2,Point(-1,-1),2);

					threshold(framepupil,framepupil, 220, 255, THRESH_BINARY);

				//	imshow ("dilate+erode+threshold",framepupil);
Mat resize2;

					resize(framepupil, resize2, Size(framepupil.rows / 1, framepupil.cols / 2));
					dilate(resize2, resize2, se2,Point(-1,-1),15);
					erode(resize2, resize2, se2,Point(-1,-1),15);
					morphologyEx(resize2, resize2, MORPH_GRADIENT,se,Point(-1,-1),25);

						Mat Cany;
						Canny(resize2, Cany, 50, 100, 5);

						  HoughCircles(Cany,//تصویر ورودی که باید سیاه و سفید باشد حتمن
								  circles,//متغییر برداری برای ذخیره سازی مقادیر xc,yc,r
								  HOUGH_GRADIENT,//روش تشخیص
								  1,//رزولوشن تصویر
								  1000,//حداقل فاصله بین مراکز شناسایی شده
						          100,//حداکثر آستانه برای الگوریتم لبه یابی کنی داخلی
								  10,//آستانه برای تشخیص مرکز
								  25,//حداقل شعاع برای شناسایی دایره ها
								  40); // حداکثر شعاع برای شناسایی دایره ها

						    for( size_t i = 0; i < circles.size(); i++ )
						    {
						         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
						         int radius = cvRound(circles[i][2]);
						         // draw the circle center
						         circle( Cany, center, 3, Scalar(125,125,125), 1, 10, 0 );
						         // draw the circle outline
						         circle( Cany, center, radius, Scalar(125,125,125), 5, 8, 0 );
							        cout << "Circles: " << circles.size() << endl;
							         cout << "center " << center << endl;

							     	int xx=(circles[i][1])-x;
	 									x=(circles[i][1]);
	 									xx=abs(xx);
									int yy=(circles[i][1])-y;
									y=(circles[i][1]);
										yy=abs(yy);

										if(circles.size()!=0 ){ //اگر دایره ای در تصویر تشخیص داده شد شرط اجرا شود
											if (xx>20 || yy>20){
											(circles[i][0])=(circles[i][0])*(2);
											(circles[i][1])=(circles[i][1])*(2);
									         mousemove((circles[i][0]),(circles[i][1]));}}
						    }

								imshow ("resize2",resize2);
							//	imshow ("Cany",Cany);


		if (waitKey(delay) >= 0)
			stop = true;
	}

	capture.release();
	waitKey();
}





