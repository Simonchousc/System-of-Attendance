#include <stdio.h>
#include "fun.h"
int main(int argc, char const *argv[])
{
    printf("Content-Type: application/json\n\n");
    // fprintf(stderr, "register.cgi执行***************************************************************\n");
    //  读取从客户端发送过来的 JSON 数据
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

    cJSON *root = cJSON_Parse(post_data);
    if (root == NULL)
    {
        printf("{\"success\": false, \"message\": \"解析 JSON 失败\"}");
        free(post_data);
        return 1;
    }

    // 从 JSON 中获取信息
    cJSON *icCard_json = cJSON_GetObjectItem(root, "icCard");
    cJSON *username_json = cJSON_GetObjectItem(root, "username");
    cJSON *password_json = cJSON_GetObjectItem(root, "password");
    cJSON *realname_json = cJSON_GetObjectItem(root, "realname");
    cJSON *gender_json = cJSON_GetObjectItem(root, "gender");
    // 从 cJSON 对象中获取值
    const char *icCard = icCard_json->valuestring;
    const char *username = username_json->valuestring;
    const char *password = password_json->valuestring;
    const char *realname = realname_json->valuestring;
    const char *gender = gender_json->valuestring;

    // 构建 SQL 查询语句
    char sql_syntax[200];
    snprintf(sql_syntax, sizeof(sql_syntax), "INSERT INTO user (uname, pwd, rname, gender, ic) VALUES ('%s', '%s', '%s', '%s', '%s')", username, password, realname, gender, icCard);

    // fprintf(stderr, "%s\n", sql_syntax);
    //   初始化数据库连接并执行操作
    MYSQL *sql = init_db_connection();

    if (sql == NULL)
    {
        printf("{\"success\": false, \"message\": \"数据库连接失败\"}");
        cJSON_Delete(root);
        free(post_data);
        return 1;
    }

    // 执行删除操作
    db_insert(sql, sql_syntax);

    // 关闭数据库连接
    close_db_connection(sql);

    // 构建并输出 JSON 响应
    printf("{\"success\": true, \"message\": \"注册成功\"}");

    // 释放内存
    cJSON_Delete(root);
    free(post_data);

    return 0;
}