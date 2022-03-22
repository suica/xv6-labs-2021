#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char error_message[] = "Usage: sleep <seconds>\n";

int main(int argc,char *argv[]){
   if(argc <= 1) {
        write(2, error_message, sizeof error_message);
        exit(1);
   }
   int seconds = atoi(argv[1]);
   sleep(seconds);
   exit(0);
}