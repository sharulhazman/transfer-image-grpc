# Server to implement GRPC Service that receives openCV Mat in serialized form
from __future__ import print_function
import grpc
from concurrent import futures
import time
import numpy as np
import sys
import generated.cvmat_pb2_grpc as server_generated
from generated.cvmat_pb2 import Response
#import OpenCV
import cv2 as cv


# The Class that implements the GRPC server

class CvMatServer(server_generated.ByteTransferSvcServicer):

    # GRPC Method
    def transferMat(self, request, context):
        response = Response()
        try:
            print("Received call from Client Data Size is ", request.datalen)
            nparr = np.fromstring(request.data, np.uint8)
            received_datalen = len(nparr)
            print("Received data", received_datalen)
            # reshape the bytes to string
            nparr = nparr.reshape(request.height, request.width, 3)
            ret = cv.imwrite("out.png", nparr)
            print("writing test image")

            response.datalen = received_datalen
            return response
        except:
            print("Unexpected error:", sys.exc_info()[0])
            response.datalen = -1
            return response

# the main program


if __name__ == '__main__':
    print("CVMat Python GRPC Server")
    # Configurations
    ip = "127.0.0.1"
    port = "2001"
    _ONE_DAY_IN_SECONDS = 60 * 60 * 24

    print("Going to Start Cloud Server on ", ip, port)
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    server_generated.add_ByteTransferSvcServicer_to_server(
        CvMatServer(), server)
    server.add_insecure_port('[::]:' + port)
    server.start()
    try:
        while True:
            time.sleep(_ONE_DAY_IN_SECONDS)
    except KeyboardInterrupt:
        server.stop(0)
