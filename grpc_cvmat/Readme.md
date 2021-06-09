This is a GRPC Server to resceive OpenCV Mat image from the NW
The test client is added as  test. 
Not tuned to reduce the bytes transferred now

To build this, create a folder name build in same level as src
and do the following steps

cd build
cmake ..
make

this will generate the server as well as &Tests  to run the tests
