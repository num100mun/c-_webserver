

#include<unistd.h> //uni std
#include<arpa/inet.h>
#include<string.h>
#include <iostream>
using namespace std;
int main(){

#ifdef _WIN32


#else
    cout << "hello word!" << endl;

#endif
    return 0;
};