#include <stdio.h>
#include "fun_login.h"
int main(int argc, char const *argv[])
{

    // 读取从客户端发送过来的 JSON 数据
    char *content_length_str = getenv("CONTENT_LENGTH");
    if (content_length_str == NULL)
    {
        printf("{\"success\": false, \"message\": \"CONTENT_LENGTH not found\"}");
        return 1;
    }

    int content_length = atoi(content_length_str);
    char *post_data = malloc(content_length + 1);
    fread(post_data, 1, content_length, stdin);
    post_data[content_length] = '\0';

    // 调用登录验证函数
    login_in(post_data);

    // 释放内存
    free(post_data);

    return 0;
}