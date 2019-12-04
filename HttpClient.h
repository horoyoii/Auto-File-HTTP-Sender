#ifndef HTTPCLIENT
#define HTTPCLIENT

#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>

#include "base64.h"
#include "MimeTypes.h"
using boost::asio::ip::tcp;
using namespace std;


/**
* HttpClient : it sends POST request to specified server with a file encoded in base64.
*
* name Field : for "Content-Disposition: form-data; name=[ ** HERE ** ]; filename=aa"
* boudary Field : for multipart/form-data's boundary...   
*                 there is a default boundary value but can be set with "setBoundary()"
*/
class HttpClient{
private:
    string servIp;
    string path;
    string port;    
    
    string boundary;
    string name;    

public:
    HttpClient(string servIp, string path, string port);
    void sendFile(string filePath, string filename);  


private:
    /**
    * make header 
    */
    void buildHeader(char** header, int bodySize, int* headerSize);



    /**
    *  Make request body. file will be attached to multipart/form-data.
    *  binary files will be encoded into base64. 
    *  getEncodedBinaryFileBuffer() wiil do encode the file.
    *
    * [meta_body + fileBuf + post_body] will make a whole request body. 
    *
    *  return -1 if file does not exist
    */
    int buildPayload(char** req_body, string filePath, string filename, int* bodySize);



    /**
    * Aggregate the request header and body into packet
    *
    */
    void buildPacket(char** packet, char* header, char* body, int headerSize, int bodySize);



    /**
    * 
    */
    int getEncodedBinaryFileBuffer(char** fileBuf, string filename);   


    
    /**
    * send request to specified server and get response 
    */
    void sendRequest(char* packet);
    
    void setName(string _name);
    void setBoundary(string _boundary);    
};

#endif
