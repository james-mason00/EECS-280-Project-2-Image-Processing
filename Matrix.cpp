#include <cassert>
#include "Matrix.hpp"

// REQUIRES: mat points to a Matrix
//           0 < width && 0 < height
// MODIFIES: *mat
// EFFECTS:  Initializes *mat as a Matrix with the given width and height,
//           with all elements initialized to 0.
void Matrix_init(Matrix* mat, int width, int height) {
  assert(width > 0 && height > 0);  //checks width and height are positive numbers
  mat->height = height;   //can't access member variables directly
  mat->width = width;
  mat->data.assign(width * height, 0);
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: os
// EFFECTS:  First, prints the width and height for the Matrix to os:
//             WIDTH [space] HEIGHT [newline]
//           Then prints the rows of the Matrix to os with one row per line.
//           Each element is followed by a space and each row is followed
//           by a newline. This means there will be an "extra" space at
//           the end of each line.
void Matrix_print(const Matrix* mat, std::ostream& os) {
  os << Matrix_width(mat) << " " << Matrix_height(mat) << "\n";
  int minRow = 0;
  int maxRow = Matrix_height(mat);
  int minCol = 0;
  int maxCol = Matrix_width(mat);
  for(int r = minRow; r < maxRow; ++r) {
    for(int c = minCol;  c < maxCol; ++c) {
        const int *elementPtr = Matrix_at(mat, r, c);
        os << *elementPtr << " ";
    }
    os << "\n";
  }
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the width of the Matrix.
int Matrix_width(const Matrix* mat) {
  return mat->width;
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the height of the Matrix.
int Matrix_height(const Matrix* mat) {
  return mat->height;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// MODIFIES: (The returned pointer may be used to modify an
//            element in the Matrix.)
// EFFECTS:  Returns a pointer to the element in the Matrix
//           at the given row and column.
int* Matrix_at(Matrix* mat, int row, int column) {
  //check desired row is positive and within bounds of matrix
  assert(row >= 0 && row < Matrix_height(mat));
  //check desired columns is positive and within bounds of matrix
  assert(column >= 0 && column < Matrix_width(mat));
  int index = (row * Matrix_width(mat)) + column;   //formula for 1d vector index
  int *ptr_at = &(mat->data.at(index));
  return ptr_at;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// EFFECTS:  Returns a pointer-to-const to the element in
//           the Matrix at the given row and column.
const int* Matrix_at(const Matrix* mat, int row, int column) {
  //check desired row is positive and within bounds of matrix
  assert(row >= 0 && row < Matrix_height(mat));
  //check desired columns is positive and within bounds of matrix
  assert(column >= 0 && column < Matrix_width(mat));
  int index = (row * Matrix_width(mat)) + column;   //formula for 1d vector index
  const int *ptr_at_const = &(mat->data.at(index));   //pointer to const
  return ptr_at_const;
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element of the Matrix to the given value.
void Matrix_fill(Matrix* mat, int value) {
  for(int r = 0; r < Matrix_height(mat); ++r) {    //iterate through rows
    for(int c = 0; c < Matrix_width(mat); ++c) {   //iterate through columns
      *Matrix_at(mat, r, c) = value;
    }
  }
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element on the border of the Matrix to
//           the given value. These are all elements in the first/last
//           row or the first/last column.
void Matrix_fill_border(Matrix* mat, int value) {
  int topRow = 0;
  int leftColumn = 0;
  int bottomRow = Matrix_height(mat) - 1;
  int rightColumn = Matrix_width(mat) - 1;
  for(int r = 0; r <= bottomRow; ++r) {
    *Matrix_at(mat, r, leftColumn) = value;  //fill left column
    *Matrix_at(mat, r, rightColumn) = value;  //fill right column
  }
  for(int c = 0; c <= rightColumn; ++c) {
    *Matrix_at(mat, topRow, c) = value;    //fill top row
    *Matrix_at(mat, bottomRow, c) = value;   //fill bottom row
  }
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the value of the maximum element in the Matrix
int Matrix_max(const Matrix* mat) {
  int max = 0;
  for(int r = 0; r < Matrix_height(mat) - 1; ++r) {
    for(int c = 0; c < Matrix_width(mat) - 1; ++c) {
      const int *elementPtr = Matrix_at(mat, r, c);
      if(*elementPtr > max) {
        max = *elementPtr;
      }
    }
  }
  return max;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the column of the element with the minimal value
//           in a particular region. The region is defined as elements
//           in the given row and between column_start (inclusive) and
//           column_end (exclusive).
//If multiple elements are minimal, returns the column of
//the leftmost one.
int Matrix_column_of_min_value_in_row(const Matrix* mat, 
  int row, int column_start, int column_end) {
  assert(row >= 0  && row < Matrix_height(mat));
  assert(column_start >= 0 && column_end <= Matrix_width(mat));
  assert(column_start < column_end);
  int minConstrainedElement = 9999999;
  int minElementCol;
  for(int c = column_start; c < column_end; ++c) {
    const int * ptrElement = Matrix_at(mat, row, c);
    if(*ptrElement < minConstrainedElement) {
      minConstrainedElement = *ptrElement;
      minElementCol = c; 
    }
  }
return minElementCol;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the minimal value in a particular region. The region
//is defined as elements in the given row and between
//column_start (inclusive) and column_end (exclusive).
int Matrix_min_value_in_row(const Matrix* mat, 
int row, int column_start, int column_end) {
  assert(row >= 0  && row < Matrix_height(mat));
  assert(0 <= column_start && 
  column_end <= Matrix_width(mat));
  assert(column_start < column_end);
  int minElementCol = Matrix_column_of_min_value_in_row(mat, 
  row, column_start, column_end);
  const int * minElementPtr = Matrix_at(mat, 
  row, minElementCol);
  return * minElementPtr;
}
