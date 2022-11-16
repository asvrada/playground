#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>

#include <iostream>
#include <cmath>
#include <assert.h>
#include <errno.h>

#define _sq(x) ((x) * (x))                          // square
#define _cb(x) abs((x) * (x) * (x))                 // absolute value of cube
#define _cr(x) (unsigned char)(pow((x), 1.0 / 3.0)) // cube root

int get_r(int i, int j)
{
  // return (char)(_sq(cos(atan2(j - 512, i - 512) / 2)) * 255);
  float x=0,y=0;int k;for(k=0;k++<256;){float a=x*x-y*y+(i-768.0)/512;y=2*x*y+(j-512.0)/512;x=a;if(x*x+y*y>4)break;}return log(k)*47;
}
int get_g(int i, int j)
{
  // return (char)(_sq(cos(atan2(j - 512, i - 512) / 2 - 2 * acos(-1) / 3)) * 255);
  float x=0,y=0;int k;for(k=0;k++<256;){float a=x*x-y*y+(i-768.0)/512;y=2*x*y+(j-512.0)/512;x=a;if(x*x+y*y>4)break;}return log(k)*47;
}
int get_b(int i, int j)
{
  // return (char)(_sq(cos(atan2(j - 512, i - 512) / 2 + 2 * acos(-1) / 3)) * 255);
  float x=0,y=0;int k;for(k=0;k++<256;){float a=x*x-y*y+(i-768.0)/512;y=2*x*y+(j-512.0)/512;x=a;if(x*x+y*y>4)break;}return 128-log(k)*23;
}

class MyImage : Fl_Box
{
public:
  MyImage(int h, int w) : Fl_Box(0, 0, h, w), h(h), w(w)
  {
    image = new unsigned char[h * w * 3];

    for (int i = 0; i < h; i++)
    {
      for (int j = 0; j < w; j++)
      {
        image[((i * w) + j) * 3 + 0] = get_r(i, j);
        image[((i * w) + j) * 3 + 1] = get_g(i, j);
        image[((i * w) + j) * 3 + 2] = get_b(i, j);
      }
    }
  }

  ~MyImage()
  {
    delete image;
  }

private:
  unsigned char *image;
  int h, w;

protected:
  void draw()
  {
    fl_draw_image(image, 0, 0, h, w);
  }
};

int program_draw_pixel_image(int argc, char **argv)
{
  int HEIGHT = 1024;
  int WIDTH = 1024;

  Fl_Window window(HEIGHT, WIDTH);
  MyImage image(HEIGHT, WIDTH);

  window.end();
  window.show(argc, argv);
  return Fl::run();
}

int program_load_image(int argc, char **argv)
{
  int HEIGHT = 480;
  int WIDTH = 320;

  char path_image[] = "/Users/jeff/test.png";

  Fl_Window *window = new Fl_Window(HEIGHT, WIDTH);

  Fl_Button *button = new Fl_Button(0, 0, 40, 30, "label");
  Fl_Box *box = new Fl_Box(50, 0, HEIGHT, WIDTH);
  Fl_PNG_Image *image = new Fl_PNG_Image(path_image);

  switch (image->fail())
  {
  case Fl_Image::ERR_NO_IMAGE:
  case Fl_Image::ERR_FILE_ACCESS:
    fl_alert("%s: %s", path_image, strerror(errno)); // shows actual os error to user
    exit(1);
  case Fl_Image::ERR_FORMAT:
    fl_alert("%s: couldn't decode image", path_image);
    exit(1);
  }

  box->image(image);
  assert(image != NULL);

  window->end();
  window->show(argc, argv);

  return Fl::run();
}

int main(int argc, char **argv)
{
  return program_draw_pixel_image(argc, argv);
}
