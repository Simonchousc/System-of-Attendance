#include <stdio.h>

#include "fun.h"
int main(int argc, char const *argv[])
{

    MYSQL *sql = init_db_connection();

    QueryResult *result = malloc(sizeof(QueryResult));
    result = db_select(sql, "SELECT ic FROM newcard ORDER BY id DESC LIMIT 1;");

    // 将查询结果转换为 JSON 字符串
    char *json_str = query_result_to_json(result);

    // 打印 Content-Type 头
    printf("Content-Type: application/json\n\n");
    fprintf(stderr, "%s\n", json_str);
    // 打印 JSON 字符串
    printf("%s\n", json_str);

    // 释放分配的内存
    free(json_str);
    close_db_connection(sql); // 关闭数据库连接
}
