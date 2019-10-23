#include "Executive.h"
// #include "Testing.h"
#include <string>
#include <iostream>

int main(int argc, char **argv){
    // bool isTesting = false;
    // if(argc > 1){
    //     for(int i=0; i < argc; i++){
    //         if(argv[i][0] == '-'){
    //             if(argv[i][1] == 't'){
    //                 isTesting = true;
    //             }
    //         }   
    //     }
    // }

    // if(isTesting == true){
    //     Testing test;
    //     test.run();
    // }else{
        Executive exec;
        exec.run();
    // }
    return 0;
}