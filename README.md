# Introduction  
Monitoring the specified local directory and send the created or modified files to web server automatically.  

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
 
   AutoFileSender needs 4 params.  
   1) server ip 
   2) server path
   3) server port  
   4) directory path you want to watch  
 ```c++
 AutoFileSender con("127.0.0.1", "/upload/","8000","/home/horoyoii/Desktop");
 ```
 
 
    #include"AutoFileSender.h"

    int main(int argc, char* argv[]){

        AutoFileSender con("127.0.0.1", "/upload/","8000","/home/horoyoii/Desktop");
        con.run();

        cout<<"not printed here"<<endl;

        return 0;
    }
 

server side may look like (In python Django)  

      @csrf_exempt
      def upload_file(req):
        if req.method == 'POST':                       
             if 'file' in req.FILES:
                  file = req.FILES['file']
                  filename = file._name
                  data = file.read()
                  decoded = base64.b64decode(data)
                  
                  f = open("./upload/"+filename, "wb")
                  f.write(decoded)
                  f.close()
                  return HttpResponse('File Uploaded')
             else:
                  return HttpResponse("no file field")
# Features 

 * Simple C++ API  
 * Runs on Linux platform  
 * **Binary data are encoded by base64**  
 * does not detect .swp or .swx files 


# Stuff it doesn't do  

* It's Linux focused and probably won't work on ohter systmes.  
* the file size is limited to 10MB.  
* it doesn't detect the directory recursively.  





