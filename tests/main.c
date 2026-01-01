#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_init.h"

/* 有参数版main函数
./gitlet commit "Initial message"
argc = 4
argv[0],"""./gitlet""",程序名本身（永远是第一个）
argv[1],"""commit""",第一个参数（命令）
argv[2],"""Initial message""",第二个参数（具体内容）
argv[3],NULL,数组的末尾总是以空指针结束
*/
int main(int argc,char* argv[]) {
    if (argc < 2){
        printf("Please enter a command.\n");
        return EXIT_SUCCESS;
    }

    char* cmd = argv[1];
    
    if (strcmp(cmd,"init") == 0){
        cmd_init();
    }else if (strcmp(cmd,"add") == 0){
        
    }else{
        printf("No command with that name exists.\n");
    }
    
    

    return EXIT_SUCCESS;
}
