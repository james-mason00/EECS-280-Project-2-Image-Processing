#include <cassert>
#include "Image.hpp"

// REQUIRES: img points to an Image
//           0 < width && 0 < height
// MODIFIES: *img
// EFFECTS:  Initializes the Image with the given width and height, with
//           all pixels initialized to RGB values of 0.
void Image_init(Image* img, int width, int height) {
  assert(width > 0 && height > 0);  //check width and height greater than 0

  Matrix_init(img->red_channel, width, height);
  Matrix_init(img->green_channel, width, height);
  Matrix_init(img->blue_channel, width, height);

  img->width = width;
  img->height = height;
}

// REQUIRES: img points to an Image
//           is contains an image in PPM format without comments
//           (any kind of whitespace is ok)
// MODIFIES: *img, is
// EFFECTS:  Initializes the Image by reading in an image in PPM format
//           from the given input stream.
// NOTE:     See the project spec for a discussion of PPM format.
void Image_init(Image* img, std::istream& is) {
  is >> img->width;    
  is >> img->height;    //reads until whitespace encountered
  int max_intensity;
  
  is >> max_intensity;
  assert(max_intensity == 255);

  int red;
  int green;    //declare in function scope to be used in loops
  int blue;

  //initialized rgb channels
  Matrix_init(&(img->red_channel), img->width, img->height);
  Matrix_init(&(img->green_channel), img->width, img->height);
  Matrix_init(&(img->blue_channel), img->width, img->height);

  for(int row = 0; row != img->height; ++row) {
    for(int col = 0; col != img->width; ++col) {
      //for each element in image, read in pixel struct values
      is >> red >> green >> blue;  

      //pack elements into respective rgb channels
      *Matrix_at(&(img->red_channel), row, col) = red;
      *Matrix_at(&(img->green_channel), row, col) = green;
      *Matrix_at(&(img->blue_channel), row, col) = blue;
    }
  }
  
}

// REQUIRES: img points to a valid Image
// MODIFIES: os
// EFFECTS:  Writes the image to the given output stream in PPM format.
//           You must use the kind of whitespace specified here.
//           First, prints out the header for the image like this:
//             P3 [newline]
//             WIDTH [space] HEIGHT [newline]
//             255 [newline]
//           Next, prints out the rows of the image, each followed by a
//           newline. Each pixel in a row is printed as three ints
//           for its red, green, and blue components, in that order. Each
//           int is followed by a space. This means that there will be an
//           "extra" space at the end of each line. See the project spec
//           for an example.
void Image_print(const Image* img, std::ostream& os) {
  os << "P3" << "\n";
  os << img->width << " " << img->height << "\n";
  os << "255" << "\n";
  for(int row = 0; row != img->height; ++row) {
    for(int col = 0; col != img->width; ++col) {
      os << *Matrix_at(&(img->red_channel), row, col) << " " << *Matrix_at(&(img->green_channel), row, col) << " " << *Matrix_at(&(img->blue_channel), row, col) << " ";
    }
    os << "\n";
  }
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the width of the Image.
int Image_width(const Image* img) {
  return img->width;
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the height of the Image.
int Image_height(const Image* img) {
  return img->height;
}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// EFFECTS:  Returns the pixel in the Image at the given row and column.
Pixel Image_get_pixel(const Image* img, int row, int column) {
  assert(0 <= row && Image_height(img));
  assert(0 <= column && column < Image_width(img));

  Pixel pix;

  //dereference element pointer and assign its value to pixel struct member variables
  pix.r = *Matrix_at(&(img->red_channel), row, column);
  pix.g = *Matrix_at(&(img->green_channel), row, column);
  pix.b = *Matrix_at(&(img->blue_channel), row, column);

  return pix;   //return pixel struct
}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Sets the pixel in the Image at the given row and column
//           to the given color.
void Image_set_pixel(Image* img, int row, int column, Pixel color) {
  assert(0 <= row && row < Image_height(img));
  assert(0 <= column && column < Image_width(img));
  
  //set pixel rgb channels via their respective pointers
  *Matrix_at(&(img->red_channel), row, column) = color.r;
  *Matrix_at(&(img->green_channel), row, column) = color.g;
  *Matrix_at(&(img->blue_channel), row, column) = color.b;
}

// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  Sets each pixel in the image to the given color.
void Image_fill(Image* img, Pixel color) {
  for(int row = 0; row != Image_height(img); ++row) {
    for(int col = 0; col != Image_width(img); ++col) {
      
      //just call Image_set_pixel on each pixel :)
      Image_set_pixel(img, row, col, color);
    }
  }
}
