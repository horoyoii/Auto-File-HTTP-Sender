#include"AutoFileSender.h"

int main(int argc, char* argv[]){
    
    AutoFileSender con("127.0.0.1", "/upload/","8000",argv[1]);
    con.run();
        
    cout<<"not printed here"<<endl;

    return 0;
}
