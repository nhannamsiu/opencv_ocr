#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace cv;
using namespace std;

void overlayImage(const cv::Mat &background, const cv::Mat &foreground,
  cv::Mat &output, cv::Point2i location)
{
  background.copyTo(output);


  // start at the row indicated by location, or at row 0 if location.y is negative.
  for(int y = std::max(location.y , 0); y < background.rows; ++y)
  {
    int fY = y - location.y; // because of the translation

    // we are done of we have processed all rows of the foreground image.
    if(fY >= foreground.rows)
      break;

    // start at the column indicated by location,

    // or at column 0 if location.x is negative.
    for(int x = std::max(location.x, 0); x < background.cols; ++x)
    {
      int fX = x - location.x; // because of the translation.

      // we are done with this row if the column is outside of the foreground image.
      if(fX >= foreground.cols)
        break;

      // determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
      double opacity =
        ((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3])

        / 255.;


      // and now combine the background and foreground pixel, using the opacity,

      // but only if opacity > 0.
      for(int c = 0; opacity > 0 && c < output.channels(); ++c)
      {
        unsigned char foregroundPx =
          foreground.data[fY * foreground.step + fX * foreground.channels() + c];
        unsigned char backgroundPx =
          background.data[y * background.step + x * background.channels() + c];
        output.data[y*output.step + output.channels()*x + c] =
          backgroundPx * (1.-opacity) + foregroundPx * opacity;
      }
    }
  }
}


int main()
{
 Mat I, Overlay, result;
 string str,temp;
 string app[100];
 int count=0,index=0;
 char cmd[100], img[20], filename[20], opentxt[20];
 cout<<"image name: "; cin>>img;
 cout<<"output name: "; cin>>filename;

 sprintf(cmd,"./images/%s.PNG",img);
 I = imread(cmd);

 Overlay = imread("./images/ip6_overlay.PNG",-1);

 if( !I.data ) { printf("Error loading Image\n"); return -1; }
 if( !Overlay.data ) { printf("Error loading Overlay \n"); return -1; }


 overlayImage(I, Overlay, result, cv::Point(0,0));
 imshow("result", result);

 sprintf(cmd,"./images/%s_overlay.PNG",img);
 imwrite(cmd, result);


	//build img name and filename into cmd and call tesseract.exe
 sprintf(cmd,"tesseract ./images/%s_overlay.PNG ./Output/%s",img,filename);
 system(cmd);
 sprintf(opentxt,"./Output/%s.txt",filename);
 ifstream txt(opentxt);
 getline(txt, str); //get first line of text
 str = ' ' + str;
 while(txt)
 {
   	getline(txt,temp); //continue to get lines until end of file
   	temp = temp + ' ';
  	str.append(temp);  //append temp line to str
 }
 txt.close();
 cout<<"Raw text: "<<endl<<str<<endl;
 for(int i = 0; i < str.size(); ++i) //replace non-alphabet characters by linefeed to clean the string
    {
        if (!((str[i] >= 'a' && str[i]<='z') || (str[i] >= 'A' && str[i]<='Z') || (str[i] >= '0' && str[i]<='9')))
        {
            str[i] = ' ';
        }
    }
 for (int i=0;i<str.length()-1;i++) //scan in str
 {
    	count = i+1;
    	while(str[i]==' ' && str[i+1]>='A' && str[i+1]<='Z') //if current character is whitespace and followed by Upper characer
    	{
    		if ( str[count]==' ' )//loop over until facing another whitespace
			{
				app[index] = str.substr(i+1,count-i-1); //store substring from first to second whitespace to app
			    index++;
				break;
			}
			if (count<str.length()) count++;  //if it is not whitespace then continue scanning
			else break;
    	}
 }
 cout<<"Processed names: "<<endl;
 for (int i=0; i<index;i++)  //if app length>4 (include NULL at the end) then show
 {
        cout<<i+1<<". "<<app[i]<<endl;
 }
  waitKey(0);
 return 0;
}
