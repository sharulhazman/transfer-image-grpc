A Python server to check if OpenCV Mat can be transferred as bytes
over the network using GRPC
Python 2.7 (if you have OpenCV compiled with Python 3 you can use that too)

To compile the Protobuffers

python2 -m grpc_tools.protoc -I../Interfaces --python_out=generated  --grpc_python_out=generated ../Interfaces/cvmat.proto
:~/coding/Prototypes/python_cvmatpython2 cvmat_server.py

Pyton Server
python2 cvmat_server.py

  CVMat Python GRPC Server
  Going to Start Cloud Server on  127.0.0.1 2001

C++ Client 
:~/coding/Prototypes/grpc_cvmat/build$ ./cvMatTests