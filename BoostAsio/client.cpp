#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
#include "base64.h"

using boost::asio::ip::tcp;
using namespace std;


#define DATA_SIZE 1024*1024*10 //10MB
#define MAXLINE 409600


/*
* Files will be encoded into base64 then attached to the request body. 
* 
*/

void buildPayload(std::string *payload, std::string boundary);


int main(int argc, char* argv[]){
  try{
    if (argc != 3){
      std::cout << "Usage: sync_client <server> <path>\n";
      std::cout << "Example:\n";
      std::cout << "  sync_client www.boost.org /LICENSE_1_0.txt\n";
      return 1;
    }

    boost::asio::io_service io_service;

    // Get a list of endpoints corresponding to the server name.
    tcp::resolver resolver(io_service);
   
    tcp::resolver::query query(argv[1], "8000");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;

    // Try each endpoint until we successfully establish a connection.
    tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end){
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
   
    
    //std::string boundary ="------WebKitFormBoundaryM7dIRpv0f6aFMeu3";
    
    boost::asio::streambuf request;
    
    std::ostream request_stream(&request);

    std::string payload = ""; 

    //buildPayload(&payload, boundary);

    // == Make HTTP Header
    

    const char* filename = argv[2];
    const char* fileDir = "pixel.jpeg";

    char *packet;
    char pre_body[1024], post_body[1024];
    char sendline[MAXLINE + 1];
    
    char boundary[] ="----WebKitFormBoundaryu8FzpUGNDgydoA4z";

    char *bodyline = (char*)calloc(sizeof(char), DATA_SIZE);
    char *fileBuf = (char*)calloc(sizeof(char), DATA_SIZE);
    
    printf("send file name : %s\n", filename);

 
      
    FILE *img = fopen(filename, "rb");
    fseek(img, 0, SEEK_END);
    
    unsigned long filesize2 = ftell(img);
    char *buffer2 = (char*)malloc(sizeof(char)*filesize2);
    rewind(img);

    fread(buffer2, sizeof(char), filesize2, img);
    
    std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(buffer2), filesize2);

   
    

    buffer2 = new char[encoded.size() +1];
    copy(encoded.begin(), encoded.end(), buffer2);
    buffer2[encoded.size()] = '\0';


// ================================================================


    // === make body ======================================
     sprintf(pre_body, "--%s\r\nContent-Disposition: form-data; name=\"pdata\"\r\n\r\n"
        "%s\r\n--%s\r\nContent-Disposition: form-data; name=\"desc\"\r\n\r\n"
        "%s\r\n--%s\r\nContent-Disposition: form-data; name=\"file\";filename=\"%s\"\r\n"
        "Content-Type:image/png\r\n\r\n"
        ,  boundary, "postdata", boundary,"no", boundary, filename);

    sprintf(post_body, "\r\n--%s--\r\n", boundary);

    int pre_len = strlen(pre_body);
    
    int post_len = strlen(post_body);

    int body_len = pre_len + filesize2 + post_len;

    memcpy(bodyline, pre_body, pre_len);
    memcpy(bodyline+pre_len, buffer2, filesize2);
    memcpy(bodyline+pre_len+filesize2, post_body, post_len);

    //make header
    sprintf(sendline, "POST /upload/ HTTP/1.1\r\n"
        "Host: %s\r\nConnection: close\r\n"
        "Content-Length: %d\r\n"
        "Content-Type: multipart/form-data; boundary=%s\r\n\r\n", argv[1], body_len, boundary);
    
    int head_len = strlen(sendline);
    int packet_len = head_len+body_len;

    //join header + body
    packet=(char*)calloc(1, head_len+body_len+1);
    memcpy(packet, sendline, head_len);
    memcpy(packet+head_len, bodyline, body_len);

    request_stream<<packet;


    // Send the request.
    boost::asio::write(socket, request);



    // Read the response status line.
    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n");




    // Check that response is OK.
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    
    std::cout<<http_version<<std::endl;

    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
      std::cout << "Invalid response\n";
      return 1;
    }
    if (status_code != 200)
    {
      std::cout << "Response returned with status code " << status_code << "\n";
      return 1;
    }

    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response, "\r\n\r\n");

    // Process the response headers.
    std::string header;
    while (std::getline(response_stream, header) && header != "\r")
      std::cout << header << "\n";
    std::cout << "\n";

    // Write whatever content we already have to output.
    if (response.size() > 0)
      std::cout << &response;

    // Read until EOF, writing data to output as we go.
    while (boost::asio::read(socket, response,
          boost::asio::transfer_at_least(1), error))
      std::cout << &response;
    if (error != boost::asio::error::eof)
      throw boost::system::system_error(error);
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }

  return 0;
}


void buildPayload(std::string *payload, std::string boundary){
    (*payload).append(boundary+"\r\n");
    (*payload).append("Content-Disposition: form-data; name=\"myFile\"; filename=\"test.txt\"\r\n");
    (*payload).append("Content-Type: text/plain\r\n\r\n");
    (*payload).append("aaaaaaaaaabbbbbbbbbb\n\r\n"); 
    
    
    (*payload).append(boundary+"--\r\n");




}
