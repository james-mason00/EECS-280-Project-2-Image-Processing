# Image Processing with Seam Carving

A C++ image resizing program built for EECS 280 at the University of Michigan. It uses seam carving to resize images while trying to preserve the most important visual content.

Unlike standard resizing methods that scale the entire image, seam carving removes paths of pixels that have relatively low visual importance. This allows an image to change dimensions while keeping prominent features from being distorted as much as possible.

## How It Works

The program breaks the resizing process into a few steps:

- **Image representation:** Stores image data using separate RGB color channels, built on a matrix abstraction.
- **Energy calculation:** Calculates pixel energy from differences between neighboring pixels to estimate which areas are less important.
- **Cost calculation:** Uses dynamic programming to determine the lowest-cost path through the image.
- **Seam selection:** Traces the minimum-cost path through the image, choosing the leftmost option when costs are tied.
- **Seam removal:** Removes one pixel from each row along a vertical seam, then repeats the process until the target dimensions are reached.
- **Height resizing:** Rotates the image, applies the same seam-removal process, and rotates it back.

## Implementation

The project is written in C++ and is organized into separate modules:

- `Matrix.cpp` handles the underlying matrix data structure and element access.
- `Image.cpp` manages image dimensions, RGB pixel data, and PPM file input and output.
- `processing.cpp` implements the energy, cost, seam-finding, seam-removal, and resizing algorithms.
- `resize.cpp` handles command-line arguments and coordinates the resizing process.

The project also uses pointers, structs, streams, assertions, and unit tests. Keeping the matrix, image, and processing logic separate made it easier to test individual parts of the program and track down bugs.

## Building and Running

The project uses the provided Makefile and supports PPM image files.

Compile the program:

```bash
make resize.exe
```

Resize an image to a target width and height:

```bash
./resize.exe input.ppm output.ppm 400 250
```

To resize only the width while keeping the original height:

```bash
./resize.exe input.ppm output.ppm 400
```

The requested dimensions must be positive and no larger than the original image.

## Testing

The project includes unit tests for the matrix and image modules, along with tests for the image-processing functions. The output can be compared against expected results to check the correctness of the resizing algorithm.

Run the test suite with:

```bash
make test
```

## What I Learned

This project gave me practice with pointer-based programming, designing modules with well-defined interfaces, writing unit tests, and implementing dynamic programming. The most interesting part was connecting the energy and cost calculations to the actual seam-removal process to produce a resized image.
