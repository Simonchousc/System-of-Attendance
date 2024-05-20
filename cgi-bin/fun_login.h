#ifndef FUN_LOGIN_H
#define FUN_LOGIN_H
#include <json-c/json.h>
#include <mysql/mysql.h>
#include "cJSON.h"
typedef struct
{
    char ***data; // 指向字符串数组的指针（查询结果）
    int rows;     // 结果集中的行数
    int cols;     // 结果集中的列数
} QueryResult;

// 初始化数据库连接
MYSQL *init_db_connection();

// 关闭数据库连接
void close_db_connection(MYSQL *sqlconnect);

// 从数据库中查询数据
QueryResult *db_select(MYSQL *sqlconnect, const char *sql_syntax);

// 将结果转换为json
char *query_result_to_json(QueryResult *result);

// 验证登录
void login_in(const char *post_data);
#endif /* FUN_LOGIN_H */
