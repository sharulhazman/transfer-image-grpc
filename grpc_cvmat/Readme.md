This is a GRPC Server to resceive OpenCV Mat image from the NW
The test client is added as  test. 
Not tuned to reduce the bytes transferred now

To build this, create a folder name build in same level as src
and do the following steps

cd build
cmake ..
make

this will generate the server as well as &Tests  to run the tests

Note:
If under proxy add the following steps before cmake as it uses google test

export HTTP_PROXY=http://abc:aaaa%25@inban1b-proxy.apac.nsn-net.net:8080
export HTTPS_PROXY=http://abc:aaaa%25%@inban1b-proxy.apac.nsn-net.net:8080

Where abc is user name
and aaaa% is your password (url encoded https://www.w3schools.com/tags/ref_urlencode.asp)