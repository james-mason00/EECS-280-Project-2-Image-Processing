#include "Matrix.hpp"
#include "Matrix_test_helpers.hpp"
#include "unit_test_framework.hpp"
#include <cassert>
#include <iostream>

using namespace std;

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Fills a 3x5 Matrix with a value and checks
// that Matrix_at returns that value for each element.
TEST(test_fill_basic) {
  Matrix mat;
  const int width = 3;
  const int height = 5;
  const int value = 42;
  Matrix_init(&mat, 3, 5);
  Matrix_fill(&mat, value);

  for(int r = 0; r < height; ++r){
    for(int c = 0; c < width; ++c){
      ASSERT_EQUAL(*Matrix_at(&mat, r, c), value);
    }
  }
}

TEST(test_matrix_print) {
  Matrix mat;
  Matrix_init(&mat, 1, 1);
  *Matrix_at(&mat, 0, 0) = 777;
  ostringstream expected;
  expected << "1 1\n" << "777 \n";
  ostringstream actual;
  Matrix_print(&mat, actual);
  ASSERT_EQUAL(expected.str(), actual.str());
}

TEST(test_matrix_width_and_height) {
  Matrix mat;
  Matrix_init(&mat, 3, 6);
  assert(Matrix_width(&mat) == 3 && Matrix_height(&mat) == 6);
}

TEST(test_matrix_at) {
  Matrix mat;
  const int value = 777;
  Matrix_init(&mat, 10, 10);
  Matrix_fill(&mat, value);
  *Matrix_at(&mat, 2, 2) = 15;
  assert(*Matrix_at(&mat, 5, 5) == value && *Matrix_at(&mat, 2, 2) == 15);
}

TEST(test_matrix_fill_border) {
  Matrix mat;
  Matrix_init(&mat, 20, 20);
  const int middleValue = 10;
  const int borderValue = 11;
  Matrix_fill(&mat, middleValue);
  Matrix_fill_border(&mat, borderValue);
  assert(*Matrix_at(&mat, 19, 0) == borderValue && *Matrix_at(&mat, 0, 0) == borderValue);
  assert(*Matrix_at(&mat, 19, 0) == borderValue && *Matrix_at(&mat, 19, 19) == borderValue);
}

TEST(test_matrix_max) {
  Matrix mat;
  Matrix_init(&mat, 5, 5);
  const int min = 0;
  const int max = 1;
  Matrix_fill(&mat, min);
  *Matrix_at(&mat, 2, 2) = max;
  assert(Matrix_max(&mat) == max);
}

TEST(test_matrix_max_upperR) {
  Matrix mat;
  Matrix_init(&mat, 250, 250);
  const int min = 0;
  const int max = 1;
  Matrix_fill(&mat, min);
  *Matrix_at(&mat, 0, 249) = max;
  assert(Matrix_max(&mat) == max);
}

TEST(test_matrix_max_upperL) {
  Matrix mat;
  Matrix_init(&mat, 250, 250);
  const int min = 0;
  const int max = 1;
  Matrix_fill(&mat, min);
  *Matrix_at(&mat, 0, 0) = max;
  assert(Matrix_max(&mat) == max);
}

TEST(test_matrix_max_lowerR) {
  Matrix mat;
  Matrix_init(&mat, 250, 250);
  const int min = 0;
  const int max = 1;
  Matrix_fill(&mat, min);
  *Matrix_at(&mat, 249, 249) = max;
  assert(Matrix_max(&mat) == max);
}

TEST(test_matrix_max_lowerL) {   
  Matrix mat;
  Matrix_init(&mat, 250, 250);
  const int min = 0;
  const int max = 1;
  Matrix_fill(&mat, min);
  *Matrix_at(&mat, 249, 0) = max;
  assert(Matrix_max(&mat) == max);
}

TEST(test_col_of_min_value_in_row) {
  Matrix mat;
  Matrix_init(&mat, 50, 50);
  Matrix_fill(&mat, 30);
  *Matrix_at(&mat, 5, 5) = 25;
  assert(Matrix_column_of_min_value_in_row(&mat, 5, 1, 10) == 5);
}

TEST(test_matrix_max_neg) {
  Matrix mat;
  Matrix_init(&mat, 5, 5);
  Matrix_fill(&mat, -10);
  *Matrix_at(&mat, 2, 2) = -5;
  assert(Matrix_max(&mat) == -5);
}

TEST(test_col_of_min_value_in_row_left_bound) {
  Matrix mat;
  Matrix_init(&mat, 50, 50);
  Matrix_fill(&mat, 25);
  *Matrix_at(&mat, 5, 1) = 10;
  assert(Matrix_column_of_min_value_in_row(&mat, 5, 1, 10) == 1);
}

TEST(test_col_of_min_value_in_row_right_boundary) {
  Matrix mat;
  Matrix_init(&mat, 50, 50);
  Matrix_fill(&mat, 30);
  *Matrix_at(&mat, 5, 9) = 10;

  assert(Matrix_column_of_min_value_in_row(&mat, 5, 1, 10) == 9);
}

TEST(test_col_of_min_value_in_row_outside_region) {
  Matrix mat;
  Matrix_init(&mat, 50, 50);
  Matrix_fill(&mat, 30);
  *Matrix_at(&mat, 5, 0) = 1;
  *Matrix_at(&mat, 5, 10) = 2;
  assert(Matrix_column_of_min_value_in_row(&mat, 5, 1, 10) == 1);
}

TEST(test_col_of_min_value_in_row_tie) {
  Matrix mat;
  Matrix_init(&mat, 50, 50);
  Matrix_fill(&mat, 30);
  *Matrix_at(&mat, 5, 3) = 10;
  *Matrix_at(&mat, 5, 7) = 10;
  assert(Matrix_column_of_min_value_in_row(&mat, 5, 1, 10) == 3);
}

TEST(test_col_of_min_value_in_row_negative) {
  Matrix mat;
  Matrix_init(&mat, 50, 50);
  Matrix_fill(&mat, -5);
  *Matrix_at(&mat, 5, 6) = -20;
  assert(Matrix_column_of_min_value_in_row(&mat, 5, 1, 10) == 6);
}

TEST(test_matrix_min_val_in_row) {
  Matrix mat;
  Matrix_init(&mat, 100, 100);
  Matrix_fill(&mat, 50);
  *Matrix_at(&mat, 10, 10) = 0;
  assert(Matrix_min_value_in_row(&mat, 10, 5, 15) == 0);
}

TEST(test_matrix_fill_border_2) {
  Matrix mat;
  Matrix_init(&mat, 100, 100);
  Matrix_fill_border(&mat, 50);

  //loop through left border
  for(int row = 0; row != Matrix_height(&mat); ++row) {
    assert(*Matrix_at(&mat, row, 0) == 50);
  }

  //loop through right border
  for(int row = 0; row != Matrix_height(&mat); ++row) {
    assert(*Matrix_at(&mat, row, Matrix_width(&mat) - 1) == 50);
  }

  //loop through top border
  for(int col = 0; col != Matrix_width(&mat); ++col) {
    assert(*Matrix_at(&mat, 0, col) == 50);
  }

  //loop through bottom border
  for(int col = 0; col != Matrix_width(&mat); ++col) {
    assert(*Matrix_at(&mat, Matrix_height(&mat) - 1, col) == 50);
  }

  //check interior is the same
  assert(*Matrix_at(&mat, 50, 50) == 0);
}

TEST(test_matrix_min_value_in_row_tie) {
  Matrix mat;
  Matrix_init(&mat, 100, 100);
  Matrix_fill(&mat, 50);
  *Matrix_at(&mat, 10, 5) = 0;
  *Matrix_at(&mat, 10, 15) = 0;
  assert(Matrix_min_value_in_row(&mat, 10, 5, 15) == 0);
}

TEST(test_matrix_min_value_in_row_outside_region) {
  Matrix mat;
  Matrix_init(&mat, 100, 100);
  Matrix_fill(&mat, 50);
  *Matrix_at(&mat, 10, 4) = -100;
  *Matrix_at(&mat, 10, 16) = -100;
  *Matrix_at(&mat, 10, 10) = 5;
  assert(Matrix_min_value_in_row(&mat, 10, 5, 15) == 5);
}


TEST_MAIN() // Do NOT put a semicolon here
