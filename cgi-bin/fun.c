#include "fun.h"
#include <stdio.h>
#include <mysql/mysql.h>
#include <string.h>

// 将查询结果转换为 JSON 字符串
char *query_result_to_json(QueryResult *result)
{
    json_object *root = json_object_new_object(); // 使用 json_object_new_object 创建 JSON 对象
    json_object *rows = json_object_new_array();  // 使用 json_object_new_array 创建 JSON 数组
    for (int i = 0; i < result->rows; i++)
    {
        json_object *row = json_object_new_array(); // 使用 json_object_new_array 创建 JSON 数组
        for (int j = 0; j < result->cols; j++)
        {
            json_object_array_add(row, json_object_new_string(result->data[i][j])); // 使用 json_object_array_add 添加元素到数组中
        }
        json_object_array_add(rows, row); // 使用 json_object_array_add 添加数组到行中
    }
    json_object_object_add(root, "rows", json_object_new_int(result->rows)); // 使用 json_object_object_add 添加键值对到对象中
    json_object_object_add(root, "cols", json_object_new_int(result->cols));
    json_object_object_add(root, "data", rows);

    const char *json_str = json_object_to_json_string(root); // 使用 json_object_to_json_string 将 JSON 对象转换为字符串

    // 将字符串拷贝到新分配的内存中，并返回
    char *result_str = strdup(json_str);
    json_object_put(root); // 释放 JSON 对象

    return result_str;
}

// 创建数据库连接
MYSQL *init_db_connection()
{
    MYSQL *sql = mysql_init(NULL);
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return NULL;
    }

    MYSQL *sqlconnect = mysql_real_connect(sql, "10.9.29.135", "root", "123456", "attendance", 3306, NULL, 0);
    if (sqlconnect == NULL)
    {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(sql));
        mysql_close(sql);
        return NULL;
    }

    return sqlconnect;
}
// 关闭数据库连接
void close_db_connection(MYSQL *sqlconnect)
{
    mysql_close(sqlconnect);
}

// 插入操作
void db_insert(MYSQL *sqlconnect, const char *sql_syntax)
{
    int ret = mysql_real_query(sqlconnect, sql_syntax, strlen(sql_syntax));
    if (ret != 0)
    {
        fprintf(stderr, "插入失败\n");
    }
}

// 删除操作
void db_delete(MYSQL *sqlconnect, const char *sql_syntax)
{
    int ret = mysql_real_query(sqlconnect, sql_syntax, strlen(sql_syntax));
    if (ret != 0)
    {
        fprintf(stderr, "删除失败\n");
    }
}

// 修改操纵
void db_update(MYSQL *sqlconnect, const char *sql_syntax)
{
    int ret = mysql_real_query(sqlconnect, sql_syntax, strlen(sql_syntax));
    if (ret != 0)
    {
        fprintf(stderr, "修改失败\n");
    }
}

// 查询操作 返回查询结果
QueryResult *db_select(MYSQL *sqlconnect, const char *sql_syntax)
{
    QueryResult *result = malloc(sizeof(QueryResult)); // 分配内存以保存结果
    if (result == NULL)
    {
        fprintf(stderr, "内存分配失败\n");
        return NULL;
    }

    int ret = mysql_real_query(sqlconnect, sql_syntax, strlen(sql_syntax));
    if (ret == 0)
    {
        MYSQL_RES *res = mysql_store_result(sqlconnect); // 存储结果
        if (res != NULL)
        {
            int col = mysql_num_fields(res); // 获取列数
            int row = mysql_num_rows(res);   // 获取行数
            result->rows = row;
            result->cols = col;

            result->data = malloc(row * sizeof(char **)); // 分配内存以保存行
            if (result->data == NULL)
            {
                fprintf(stderr, "内存分配失败\n");
                mysql_free_result(res);
                free(result);
                return NULL;
            }

            for (int i = 0; i < row; i++)
            {
                MYSQL_ROW rowdate = mysql_fetch_row(res);       // 获取一行数据
                result->data[i] = malloc(col * sizeof(char *)); // 分配内存以保存列
                if (result->data[i] == NULL)
                {
                    fprintf(stderr, "内存分配失败\n");
                    mysql_free_result(res);
                    free(result);
                    return NULL;
                }
                for (int j = 0; j < col; j++)
                {
                    result->data[i][j] = malloc(strlen(rowdate[j]) + 1); // 分配内存以保存单元格内容
                    if (result->data[i][j] == NULL)
                    {
                        fprintf(stderr, "内存分配失败\n");
                        mysql_free_result(res);
                        free(result);
                        return NULL;
                    }
                    strcpy(result->data[i][j], rowdate[j]); // 复制单元格内容
                }
            }
            mysql_free_result(res);
            return result;
        }
        else
        {
            fprintf(stderr, "无法存储结果\n");
            free(result);
            return NULL;
        }
    }
    else
    {
        fprintf(stderr, "查询失败: %s\n", mysql_error(sqlconnect));
        free(result);
        return NULL;
    }
}
