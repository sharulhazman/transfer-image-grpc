#include <grpc++/grpc++.h>
#include <gtest/gtest.h>
#include <thread>
#include "../src/CvMatServer.h"
//Open CV includes
#include <opencv2/highgui.hpp>
#include <iostream>
//Opencv and GRPC namespaces
using namespace grpc;
using namespace std;
using namespace cv;
typedef unsigned char byte;

const string target = "127.0.0.1:17007";
namespace
{

class CvMatServerTest : public ::testing::Test
{

  protected:
    //::grpc::string target = "0.0.0.0:17007";
    CvMatServerTest() {}

    virtual ~CvMatServerTest() {}

    virtual void SetUp() {}

    virtual void TearDown() {}

    // Objects declared here can be used by all tests in the test case for Foo.
};

// Test Basic Server Test
TEST_F(CvMatServerTest, Inttegration_TestServerBasic)
{
    std::unique_ptr<ByteTransferSvc::Stub> stub_;

    std::shared_ptr<grpc::Channel> channel =
        grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
    stub_ = ByteTransferSvc::NewStub(channel);

    Response response;
    ClientContext context;
    MatInput parameter;
    std::string somebytes = "hello";
    parameter.set_data(somebytes);
    parameter.set_datalen(5);
    Status status = stub_->transferMat(&context, parameter, &response);
    ASSERT_TRUE(status.ok());
    cout << "detectObject OK" << endl;
}

TEST_F(CvMatServerTest, Inttegration_TestServerBytes)
{
    std::unique_ptr<ByteTransferSvc::Stub> stub_;

    std::shared_ptr<grpc::Channel> channel =
        grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
    stub_ = ByteTransferSvc::NewStub(channel);

    Response response;
    ClientContext context;
    cv::Mat image;
    image = cv::imread("../test/image.png", CV_LOAD_IMAGE_COLOR);

    if (!image.data)
    {
        std::cout << "Could not open or find the image" << std::endl;
        return;
    }
    int width = image.cols;
    int height = image.rows;

    std::cout << "Test Width : " << width << std::endl;
    std::cout << "Test Height: " << height << std::endl;

    int dataSize = image.total() * image.elemSize();
    std::cout << "Image dataSize: " << dataSize << std::endl;
    imwrite("in1.png", image);

    //convert to bytes
    std::vector<char> vec(dataSize);
    memcpy(&vec[0], reinterpret_cast<char *>(image.data), dataSize);
    std::string test2(vec.begin(), vec.end());
    std::cout << "Encoded String length " << test2.length() << std::endl;

    //test to check if the string encoding is proper before sending
    // over the network
    cv::Mat data_mat(height,width,CV_8UC3,const_cast<char*>(test2.c_str()));
    imwrite("out2.png", data_mat);

    //todo - send a compressed string out  - for later
    std::vector<uchar> buff;//buffer for coding
    std::vector<int> param(2);
    param[0] = cv::IMWRITE_PNG_COMPRESSION;
    param[1] = 5;//default 0-9
    cv::imencode(".png", image, buff, param);
    std::string compData(buff.begin(), buff.end());
    std::cout << "Compressed String length " << compData.length() << std::endl;
    //Encoded String length 953388
    //Compressed String length 955414 --compresion 0
    //Compressed String length 489649 --compresion 5

    // send bytes over NW via GRPC
    MatInput parameter;
    parameter.set_data(test2);
    parameter.set_datalen(dataSize);
    parameter.set_width(width);
    parameter.set_height(height);
    Status status = stub_->transferMat(&context, parameter, &response);
    ASSERT_EQ(response.datalen(), dataSize);
}

void RunServer(string server_address)
{

    cout << "Going to Start the Server" << endl;
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    CvMatImpl cvMatgrpc;
    builder.RegisterService(&cvMatgrpc);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    cout << "Server listening on {}" << server_address << endl;
    server->Wait();
}

} // end namespace

int main(int argc, char **argv)
{
    cout << "Going to Run all tests " << endl;
    auto startGRPCServer = [](string k) { RunServer(k); };
    string server_address(target);
    thread t1(startGRPCServer, server_address);
    this_thread::sleep_for(chrono::milliseconds(100));
    ::testing::InitGoogleTest(&argc, argv);
    int out = RUN_ALL_TESTS();
    t1.detach();
    return out;
}