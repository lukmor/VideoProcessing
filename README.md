# VideoProcessing
A Visual Studio solution for Windows that returns a csv with median values describing the input video.

!Dependencies!:
Install OpenCV.
To Windows System variables, add variable OPENCV3_DIR that points to OpenCV build folder.
To Windows Path System variable, add %OPENCV3_DIR%\x64\vc14\bin

With a prepared video file, run VideoProcessing.exe with 3 arguments: name of the video file, x dimension, y dimension.
For example: VideoProcessing video.mp4 128 64
