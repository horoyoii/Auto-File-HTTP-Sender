#include "HttpClient.h"


HttpClient::HttpClient(string servIp, string path, string port)
    :servIp(servIp), path(path), port(port), name("file"){
    
    boundary = "----WebKitFormBoundaryu8FzpUGNDgydoA4z";
}

void HttpClient::sendFile(string filePath){
    char* packet;   // request header + body 
    char* header;   // = (char*)calloc(sizeof(char), 
    char* body;     //         
    
    int headerSize;
    int bodySize;

    int res = buildPayload(&body, filePath, &bodySize);
            
    if(res == -1) // if file does not exist
        return;

    buildHeader(&header, bodySize, &headerSize);
    
    buildPacket(&packet, header, body, headerSize, bodySize);

    
    sendRequest(packet);
       
    printf("Send done\n");
}




void HttpClient::buildHeader(char** header, int bodySize, int* headerSize){
    *header = (char*)calloc(sizeof(char), 1024<<2);
     
    sprintf(*header, "POST %s HTTP/1.1\r\n"
        "Host: %s\r\nConnection: close\r\n"
        "Content-Length: %d\r\n"
        "Content-Type: multipart/form-data; boundary=%s\r\n\r\n", path.c_str(), servIp.c_str(), bodySize, boundary.c_str());
    
    *headerSize = strlen(*header); 
}


int HttpClient::buildPayload(char** req_body, string filename, int* bodySize){
    
    *req_body = (char*)calloc(sizeof(char), 1024*1024*10);

    char    meta_body[1024]; 
    char*   fileBuf;
    char    post_body[1024];
    int     filesize; 


    //TODO : to customize the name field and automatically set the Content-type 
    // === make body ======================================
    sprintf(meta_body, "--%s\r\nContent-Disposition: form-data; name=\"%s\";filename=\"%s\"\r\n"
    "Content-Type:image/png\r\n\r\n"
    ,  boundary.c_str(), name.c_str(), filename.c_str());


    sprintf(post_body, "\r\n--%s--\r\n", boundary.c_str());

    filesize = getEncodedBinaryFileBuffer(&fileBuf, filename);
    
    if(filesize == -1){ // file does not exist
        return -1;
    }


    int meta_len = strlen(meta_body);
    int post_len = strlen(post_body);
    int body_len = meta_len + filesize + post_len; 
    
    memcpy((*req_body), meta_body, meta_len);
    memcpy((*req_body) + meta_len, fileBuf, filesize);
    memcpy((*req_body) + meta_len + filesize, post_body, post_len);
    
    *bodySize = body_len;
}



void HttpClient::buildPacket(char** packet, char* header, char* body, int headerSize, int bodySize){
   
    *packet =(char*)calloc(1, headerSize + bodySize + 1);
    
    memcpy(*packet, header, headerSize);
    memcpy((*packet)+headerSize, body, bodySize);

}

int HttpClient::getEncodedBinaryFileBuffer(char** fileBuf, string filename){
    
    FILE *fp = fopen(filename.c_str(), "rb");
    if(fp == NULL){
        printf("File does not exist\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);

    unsigned long filesize = ftell(fp);

    char *buffer = (char*)malloc(sizeof(char)*filesize);
    rewind(fp);

    fread(buffer, sizeof(char), filesize, fp);
    
    std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(buffer), filesize);

    filesize = encoded.length();
    
    *fileBuf = new char[encoded.size() +1];

    copy(encoded.begin(), encoded.end(), *fileBuf);
    (*fileBuf)[encoded.size()] = '\0';


    delete buffer;
    fclose(fp);
    
    return filesize;    
}





void HttpClient::sendRequest(char* packet){
    
    boost::asio::io_service io_service;
    
    tcp::resolver resolver(io_service);

    tcp::resolver::query query(servIp, port);

    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;

    tcp::socket socket(io_service);
    boost::system::error_code error = boost::asio::error::host_not_found;
    
    while (error && endpoint_iterator != end){
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }       
    
    boost::asio::streambuf request;
    std::ostream request_stream(&request);

    request_stream << packet;

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
    
    if (!response_stream || http_version.substr(0, 5) != "HTTP/"){
      std::cout << "Invalid response\n";
      return;
    }
    
    if (status_code != 200){
      std::cout << "Response returned with status code " << status_code << "\n";
      return ;
    }

    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response, "\r\n\r\n");

    // Process the response headers.
    std::string header2;
    while (std::getline(response_stream, header2) && header2 != "\r")
      std::cout << header2 << "\n";
    std::cout << "\n";

    // Write whatever content we already have to output.
    if (response.size() > 0)
      std::cout << &response;


}


void HttpClient::setName(string _name){
    this->name = _name;
}

void HttpClient::setBoundary(string _boundary){
    this->boundary = _boundary;
}



