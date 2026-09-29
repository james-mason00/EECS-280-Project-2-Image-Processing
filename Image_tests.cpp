#include "Matrix.hpp"
#include "Image_test_helpers.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

using namespace std;

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Sets various pixels in a 2x2 Image and checks
// that Image_print produces the correct output.
TEST(test_print_basic) {
  Image img;
  const Pixel red = {255, 0, 0};
  const Pixel green = {0, 255, 0};
  const Pixel blue = {0, 0, 255};
  const Pixel white = {255, 255, 255};

  Image_init(&img, 2, 2);
  Image_set_pixel(&img, 0, 0, red);
  Image_set_pixel(&img, 0, 1, green);
  Image_set_pixel(&img, 1, 0, blue);
  Image_set_pixel(&img, 1, 1, white);

  // Capture our output
  ostringstream s;
  Image_print(&img, s);

  // Correct output
  ostringstream correct;
  correct << "P3\n2 2\n255\n";
  correct << "255 0 0 0 255 0 \n";
  correct << "0 0 255 255 255 255 \n";
  ASSERT_EQUAL(s.str(), correct.str());
}

TEST(test_img_init_istringstream_and_get_pixel) {
  Image img;

  std::istringstream is("P3\n2 2\n255\n255 0 0 0 255 0 \n0 0 255 255 255 255\n");
  
  const Pixel red = {255, 0, 0};
  const Pixel green = {0, 255, 0};
  const Pixel blue = {0, 0, 255};
  const Pixel white = {255, 255, 255};

  Image_init(&img, is);

  //checks each pixel is correct
  assert(Pixel_equal(red, Image_get_pixel(&img, 0, 0)));
  assert(Pixel_equal(green, Image_get_pixel(&img, 0, 1)));
  assert(Pixel_equal(blue, Image_get_pixel(&img, 1, 0)));
  assert(Pixel_equal(white, Image_get_pixel(&img, 1, 1)));
}

TEST(test_img_width) {
  Image img;
  Image_init(&img, 5, 5);
  assert(Image_width(&img) == 5);
}

TEST(test_img_height) {
  Image img;
  Image_init(&img, 5, 5);
  assert(Image_height(&img) == 5);
}

TEST(test_img_get_pixel_edge) {
  Image img;
  Image_init(&img, 250, 250);
  const Pixel pixel_default = {0, 0, 0};
  for(int row = 0; row != Image_height(&img); ++row) {
    for(int col = 0; col != Image_width(&img); ++col) {
      assert(Pixel_equal(pixel_default, Image_get_pixel(&img, row, col)));
    }
  }
}

TEST(test_img_set_pixel) {
  Image img;
  Image_init(&img, 250, 250);
  const Pixel lavender = {250, 250, 230};   //one of my favorite colors :)

  //set every pixel to lavender, check it's actually lavender
  for(int row = 0; row != Image_height(&img); ++row) {
    for(int col = 0; col != Image_width(&img); ++col) {
      Image_set_pixel(&img, row, col, lavender);
      assert(Pixel_equal(lavender, Image_get_pixel(&img, row, col)));
    }
  }
}

TEST(test_img_fill) {
  Image img;
  Image_init(&img, 250, 250);
  const Pixel pink = {235, 52, 180};

  Image_fill(&img, pink);

  //again, loop through all pixels, check they're all pink
  for(int row = 0; row != Image_height(&img); ++row) {
    for(int col = 0; col != Image_width(&img); ++col) {
      assert(Pixel_equal(pink, Image_get_pixel(&img, row, col)));
    }
  }
}

TEST(test_img_fill_small) {
  Image img;
  Image_init(&img, 3, 2);
  const Pixel pink = {235, 52, 180};
  Image_fill(&img, pink);
  for (int row = 0; row < Image_height(&img); ++row) {
    for (int col = 0; col < Image_width(&img); ++col) {
      assert(Pixel_equal(pink, Image_get_pixel(&img, row, col)));
    }
  }
}

TEST(test_img_fill_one_row) {
  Image img;
  Image_init(&img, 5, 1);
  const Pixel pink = {235, 52, 180};
  Image_fill(&img, pink);
  for (int col = 0; col < Image_width(&img); ++col) {
    assert(Pixel_equal(pink, Image_get_pixel(&img, 0, col)));
  }
}

TEST(test_img_fill_one_col) {
  Image img;
  Image_init(&img, 1, 5);
  const Pixel pink = {235, 52, 180};
  Image_fill(&img, pink);
  for (int row = 0; row < Image_height(&img); ++row) {
    assert(Pixel_equal(pink, Image_get_pixel(&img, row, 0)));
  }
}

TEST_MAIN() // Do NOT put a semicolon here
