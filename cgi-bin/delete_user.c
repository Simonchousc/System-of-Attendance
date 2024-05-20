#include <stdio.h>
#include <stdlib.h>
#include "fun.h"

int main(int argc, char const *argv[])
{
    printf("Content-Type: application/json\n\n");
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

    // 解析 JSON 数据
    cJSON *root = cJSON_Parse(post_data);
    if (root == NULL)
    {
        printf("{\"success\": false, \"message\": \"解析 JSON 失败\"}");
        free(post_data);
        return 1;
    }

    // 从 JSON 中获取ic
    cJSON *icCard_json = cJSON_GetObjectItem(root, "icCard");
    if (icCard_json == NULL)
    {
        printf("{\"success\": false, \"message\": \"缺少 IC 卡号\"}");
        cJSON_Delete(root);
        free(post_data);
        return 1;
    }

    // 从 cJSON 对象中获取 IC 卡号的值
    const char *icCard = icCard_json->valuestring;

    // 构建 SQL 查询语句
    char sql_syntax[100]; // 根据需要调整数组大小
    snprintf(sql_syntax, sizeof(sql_syntax), "DELETE FROM user WHERE ic='%s'", icCard);

    // fprintf(stderr, "%s\n", sql_syntax);
    //  初始化数据库连接并执行删除操作
    MYSQL *sql = init_db_connection();

    if (sql == NULL)
    {
        printf("{\"success\": false, \"message\": \"数据库连接失败\"}");
        cJSON_Delete(root);
        free(post_data);
        return 1;
    }

    // 执行删除操作
    db_delete(sql, sql_syntax);

    // 关闭数据库连接
    close_db_connection(sql);

    // 构建并输出 JSON 响应
    printf("{\"success\": true, \"message\": \"删除成功\"}");

    // 释放内存
    cJSON_Delete(root);
    free(post_data);

    return 0;
}
