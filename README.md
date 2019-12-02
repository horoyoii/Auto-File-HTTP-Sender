# Introduction  
Monitoring the specified local directory and send the created or modified file to web server automatically.  

* Monitoring the specified directory with a Linux Kernel subsystem [inotify](https://en.wikipedia.org/wiki/Inotify#References) utility.  
* [Boost Asio C++](https://en.wikipedia.org/wiki/Asio_C%2B%2B_library) is used in networking.  
* HTTP request format 

      POST [ given path ] HTTP/1.1
      Host: [ given host ]
      Connection: close
      Content-type: multipart/form-data; boundary=--------287032381131322
      Content-length: 514

      ----------287032381131322
      Content-Disposition: form-data; name="file"; filename="[ given file name]"
      Content-Type: image/gif
        
      [ binary data encoded in base64 ]
      
      ----------287032381131322--
 
 
 
 
 # Usage 
 
    #include"AutoFileSender.h"

    int main(int argc, char* argv[]){

        AutoFileSender con("127.0.0.1", "/upload/","8000","/home/horoyoii/Desktop");
        con.run();

        cout<<"not printed here"<<endl;

        return 0;
    }
 

# Features 

 * Simple C++ API  
 * Runs on Linux platform  
 * **Binary data are encoded by base64**  
 * does not detect .swp or .swx files 




