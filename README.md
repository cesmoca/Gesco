# Gesco

Gesco is an old C++/OpenCV student project for recognizing simple hand gestures
from a webcam stream. It was originally written for a University of Minnesota
course project, with the recognition code intended to be reusable as a small
library that other applications could call.

The repository contains two parts:

- `Gesco/`: the gesture recognition library.
- `src/`: a small SDL/OpenCV test application that opens a camera, feeds frames
  into the recognizer, and displays debug windows.

## What It Does

Gesco tries to detect a moving hand in front of a camera and compare its contour
against stored gesture templates.

The pipeline is roughly:

1. Capture a frame from the webcam.
2. Resize and blur the frame.
3. Build a motion mask from frame differences.
4. Build a skin mask using histogram backprojection.
5. Estimate a hand seed point from movement.
6. Flood fill a likely hand region.
7. Extract contours and compare them against saved gesture contours.

The implementation is intentionally simple and experimental. It was built as a
course project, not as a production computer vision library.

## Repository Layout

```text
.
|-- Makefile                 # Builds the test application
|-- src/                     # Test application code
|-- data/
|   |-- gestures/            # Stored gesture templates
|   `-- histograms/          # Stored skin histograms
`-- Gesco/
    |-- Makefile             # Builds the recognizer objects/library
    |-- include/             # Public and internal headers
    |-- source/              # Recognizer implementation
    `-- tool/                # Helper tool for generating histograms/gestures
```

## Dependencies

The current Makefiles expect an MSYS2/MinGW-style environment with:

- `g++`
- `make`
- `pkg-config`
- SDL 1.2 development files
- OpenCV 4 development files

On MSYS2 UCRT64, that usually means installing packages equivalent to:

```sh
pacman -S make mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-pkgconf
pacman -S mingw-w64-ucrt-x86_64-SDL mingw-w64-ucrt-x86_64-opencv
```

## Building

From the project directory:

```sh
make
```

The test application is built as:

```text
bin/test.exe
```

The recognizer library build lives under:

```text
Gesco/lib/
```

## Running The Test App

Run the test app from the project directory so it can find the `data/` folder:

```sh
./bin/test.exe
```

Press `Escape` or close the SDL/OpenCV window to exit.

The app expects a webcam at device index `0`. It also expects the histogram and
gesture YAML files under `data/histograms/` and `data/gestures/`.

## Using The Recognizer

The intended library entry point is `Recognizer`:

```cpp
#include <Recognizer.h>

Recognizer recognizer(width, height, 0.5f, false);

while (running) {
    cv::Mat frame;
    camera >> frame;
    recognizer.detect(frame);
}
```

Constructor arguments:

- `width`, `height`: camera frame dimensions.
- `scale`: internal processing scale. Smaller values are faster.
- `debug`: enables debug OpenCV windows.

The public API is still incomplete. `Recognizer::detect()` runs the detection
pipeline, while result reporting was planned but not fully finished.

## Helper Tool

`Gesco/tool/calcBackProject.cpp` is a small interactive helper used to generate
skin histograms and gesture contours. It opens a camera frame, lets the user
pick a point, flood-fills a mask, and saves YAML files.

Build it from the tool directory:

```sh
cd Gesco/tool
make
```

## Modernization Notes

This code was originally written against older OpenCV APIs. The current tree has
been partially updated for OpenCV 4:

- old color conversion constants were replaced with `cv::COLOR_*`
- old contour constants were replaced with `cv::RETR_*` and `cv::CHAIN_*`
- old camera property constants were replaced with `cv::CAP_PROP_*`
- old `cvMoveWindow` calls were replaced with `cv::moveWindow`
- Makefiles now use `pkg-config ... opencv4`

There are still historical rough edges:

- SDL is SDL 1.2, not SDL2.
- The recognizer API is not fully polished.
- Some error handling still reflects the original student-project code style.
- The algorithm is sensitive to lighting, background motion, and camera quality.

## License

No license file is currently included. Treat the code as private/unlicensed
unless a license is added.
