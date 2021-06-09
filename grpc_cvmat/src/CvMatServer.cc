#include "CvMatServer.h"
#include <opencv2/highgui.hpp>
using namespace std;
using namespace grpc;

::grpc::Status CvMatImpl::transferMat(::grpc::ServerContext *context,
                                      const ::MatInput *request, ::Response *response)
{

  cout << "Got the call from Client Transfer Mat" << endl;
  int datalen = request->data().length();
  cout << "Received Data " << datalen << endl;
  cout << "Expected data " << request->datalen() << endl;
  if (datalen != request->datalen())
  {
    cout << "Data Size Mismatch" << endl;
    return ::grpc::Status::OK;
  }
  //Get the Mat back
  int height = request->height();
  int width = request->width();
  
  std::cout << "Width : " << width << std::endl;
  std::cout << "Height: " << height << std::endl;
  if ((height > 0) && (width > 0))
  {
    cv::Mat image = cv::Mat(height, width, CV_8UC3, 
    const_cast<char *>(request->data().c_str())).clone();
    cv::imwrite("output.png", image);
  }
  response->set_response(0);
  response->set_datalen(datalen);
  return ::grpc::Status::OK;
}
