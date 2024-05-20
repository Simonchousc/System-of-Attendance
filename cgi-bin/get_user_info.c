#include <stdio.h>
#include "fun.h"
int main(int argc, char const *argv[])
{
    // 打印 Content-Type 头
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
    cJSON *username_json = cJSON_GetObjectItem(root, "username");
    cJSON *password_json = cJSON_GetObjectItem(root, "password");

    // 从 cJSON 对象中获取值

    const char *username = username_json->valuestring;
    const char *password = password_json->valuestring;

    // 构建 SQL 查询语句
    char sql_syntax[200];
    snprintf(sql_syntax, sizeof(sql_syntax), "SELECT * FROM  user WHERE uname='%s' AND pwd='%s'", username, password);

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

    QueryResult *result = malloc(sizeof(QueryResult));

    // 执行操作
    result = db_select(sql, sql_syntax);

    // 将查询结果转换为 JSON 字符串
    char *json_str = query_result_to_json(result);

    // 打印 JSON 字符串
    printf("%s\n", json_str);

    // 关闭数据库连接
    close_db_connection(sql);

    // 释放内存
    cJSON_Delete(root);
    free(post_data);

    return 0;
}