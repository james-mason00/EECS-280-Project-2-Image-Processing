#include <cassert>
#include <vector>
#include "processing.hpp"

using namespace std;

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_left is provided for you.
// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the left (counterclockwise).
void rotate_left(Image* img) {

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image aux;
  Image_init(&aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(&aux, width - 1 - c, r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_right is provided for you.
// REQUIRES: img points to a valid Image.
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the right (clockwise).
void rotate_right(Image* img){

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image aux;
  Image_init(&aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(&aux, c, height - 1 - r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// v DO NOT CHANGE v ------------------------------------------------
// The implementation of diff2 is provided for you.
static int squared_difference(Pixel p1, Pixel p2) {
  int dr = p2.r - p1.r;
  int dg = p2.g - p1.g;
  int db = p2.b - p1.b;
  // Divide by 100 is to avoid possible overflows
  // later on in the algorithm.
  return (dr*dr + dg*dg + db*db) / 100;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// ------------------------------------------------------------------
// You may change code below this line!



// REQUIRES: img points to a valid Image.
//           energy points to a Matrix.
// MODIFIES: *energy
// EFFECTS:  energy serves as an "output parameter".
//           The Matrix pointed to by energy is initialized to be the same
//           size as the given Image, and then the energy matrix for that
//           image is computed and written into it.
//           See the project spec for details on computing the energy matrix.
void compute_energy_matrix(const Image* img, Matrix* energy) {
  Matrix_init(energy, img->width, img->height);
  int max_energy = 0;
  for(int row = 1; row < (img->height - 1); ++row) {
    for(int col = 1; col < (img->width - 1); ++col) {
      Pixel N = Image_get_pixel(img, row + 1, col);
      Pixel S = Image_get_pixel(img, row - 1, col);
      Pixel W = Image_get_pixel(img, row, col - 1);
      Pixel E = Image_get_pixel(img, row, col + 1);
      int current_energy = squared_difference(N, S) + squared_difference(W, E);
      *Matrix_at(energy, row, col) = current_energy;
      if(current_energy > max_energy) {
        max_energy = current_energy;
      }
    }
  }
Matrix_fill_border(energy, max_energy);
}


// REQUIRES: energy points to a valid Matrix.
//           cost points to a Matrix.
//           energy and cost aren't pointing to the same Matrix
// MODIFIES: *cost
// EFFECTS:  cost serves as an "output parameter".
//           The Matrix pointed to by cost is initialized to be the same
//           size as the given energy Matrix, and then the cost matrix is
//           computed and written into it.
//           See the project spec for details on computing the cost matrix.
void compute_vertical_cost_matrix(const Matrix* energy, Matrix *cost) {
  Matrix_init(cost, energy->width, energy->height);

  //fill in first row of cost matrix with each pixel's energy
  for(int col = 0; col != Matrix_width(cost); ++col) {
    *Matrix_at(cost, 0, col) = *Matrix_at(energy, 0, col);
  }
  
  //loop through remaining rows, using reccurence relation to compute cost
  for(int row = 1; row != Matrix_height(cost); ++row) {
    //left edge
    *Matrix_at(cost, row, 0) = *Matrix_at(energy, row, 0) + min(*Matrix_at(cost, row - 1, 0), *Matrix_at(cost, row - 1, 1));

    //middle
    for(int col = 1; col != Matrix_width(cost) - 1; ++col) {
      *Matrix_at(cost, row, col) = *Matrix_at(energy, row, col) + min(*Matrix_at(cost, row -1, col -1), min(*Matrix_at(cost, row - 1, col), *Matrix_at(cost, row - 1, col + 1)));
    }
    //right edge
    int last_col = Matrix_width(cost) - 1;
    *Matrix_at(cost, row, last_col) = *Matrix_at(energy, row, last_col) + min(*Matrix_at(cost, row - 1, last_col), *Matrix_at(cost, row - 1, last_col - 1));
  }
}


// REQUIRES: cost points to a valid Matrix
// EFFECTS:  Returns the vertical seam with the minimal cost according to the given
//           cost matrix, represented as a vector filled with the column numbers for
//           each pixel along the seam, with index 0 representing the lowest numbered
//           row (top of image). The length of the returned vector is equal to
//           Matrix_height(cost).
//           While determining the seam, if any pixels tie for lowest cost, the
//           leftmost one (i.e. with the lowest column number) is used.
//           See the project spec for details on computing the minimal seam.
//           Note: When implementing the algorithm, compute the seam starting at the
//           bottom row and work your way up.
vector<int> find_minimal_vertical_seam(const Matrix* cost) {
  vector<int> seam;

  //note: matrix_col_of_min_val_in_row accounts for ties already
  //also note this function is [start, end)

  //find column in bottom row with minimum energy
  int min_cost_i = Matrix_column_of_min_value_in_row(cost, cost->height - 1, 0, cost->width);
  seam.insert(seam.begin(), min_cost_i);   //start seam vector

  //work upwards, finding "upstairs neighbor" with lowest cost
  for(int row = cost->height - 2; row >= 0; --row) {
    if(min_cost_i == cost->width - 1) {    //consider only directly above, and above left
      min_cost_i = Matrix_column_of_min_value_in_row(cost, row, min_cost_i - 1, min_cost_i + 1);
    } else if (min_cost_i - 1 < 0) {    //consider only directly above and above right
      min_cost_i = Matrix_column_of_min_value_in_row(cost, row, min_cost_i, min_cost_i + 2);
    } else {    //consider directly above, above left, and above right
      min_cost_i = Matrix_column_of_min_value_in_row(cost, row, min_cost_i - 1, min_cost_i + 2);
    }
    seam.insert(seam.begin(), min_cost_i);
  }
  return seam;
}


// REQUIRES: img points to a valid Image with width >= 2
//           seam.size() == Image_height(img)
//           each element x in seam satisfies 0 <= x < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Removes the given vertical seam from the Image. That is, one
//           pixel will be removed from every row in the image. The pixel
//           removed from row r will be the one with column equal to seam[r].
//           The width of the image will be one less than before.
//           See the project spec for details on removing a vertical seam.
// NOTE:     Declare a new variable to hold the smaller Image, and
//           then do an assignment at the end to copy it back into the
//           original image.
void remove_vertical_seam(Image *img, const vector<int> &seam) {
  Image aux;
  int height = Image_height(img);
  int width = Image_width(img);
  Image_init(&aux, width - 1, height);    //bc we are removing one vertical column

  assert(width >= 2);   //check requirements 
  assert(seam.size() == Image_height(img));

  //copy one row at a time, skipping vertical seam
  for(int row = 0; row != height; ++row) {
    int new_col = 0;    //to shift left
    for(int col = 0; col != width; ++col) {
      if(col != seam[row]) {    //only copy if current column isn't contained in seam
        Image_set_pixel(&aux, row, new_col, Image_get_pixel(img, row, col));
        ++new_col;    //move col to shift less left for when seam encountered
      }
    }
  }
  *img = aux;   //copy back into original
}

// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width of the given Image to be newWidth by using
//           the seam carving algorithm. See the spec for details.
void seam_carve_width(Image *img, int newWidth) {
  assert(0 < newWidth && newWidth <= Image_width(img));

  while(Image_width(img) > newWidth) {
     //create and initialize energy & cost matrix with same dims as original image
    Matrix energy;
    Matrix cost;
    Matrix_init(&energy, Image_width(img), Image_height(img));
    Matrix_init(&cost, Image_width(img), Image_height(img));

    //compute matrices
    compute_energy_matrix(img, &energy);
    compute_vertical_cost_matrix(&energy, &cost);
    vector<int> vseam = find_minimal_vertical_seam(&cost);
    remove_vertical_seam(img, vseam);
  }
}

// REQUIRES: img points to a valid Image
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the height of the given Image to be newHeight.
// NOTE:     This is equivalent to first rotating the Image 90 degrees left,
//           then applying seam_carve_width(img, newHeight), then rotating
//           90 degrees right.
void seam_carve_height(Image *img, int newHeight) {
  assert(0 < newHeight && newHeight <= Image_height(img));
  rotate_left(img);
  seam_carve_width(img, newHeight);
  rotate_right(img);

}

// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width and height of the given Image to be newWidth
//           and newHeight, respectively.
// NOTE:     This is equivalent to applying seam_carve_width(img, newWidth)
//           and then applying seam_carve_height(img, newHeight).
void seam_carve(Image *img, int newWidth, int newHeight) {
  assert(0 < newWidth && newWidth <= Image_width(img));
  assert(0 < newHeight && newHeight <= Image_height(img));

  seam_carve_width(img, newWidth);
  seam_carve_height(img, newHeight);
}
