#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <mysql/mysql.h>
char *query_data(const char *sql);
int sql_operation(const char *sql);
char *get_current_time();
int main(int argc, char const *argv[])
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    inet_pton(AF_INET, "10.9.29.135", (void *)&(addr.sin_addr));
    bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));

    char buf[1500] = "";
    char sql_syntx[256] = "";

    while (1)
    {

        bzero(buf, sizeof(buf));
        recvfrom(sockfd, buf, sizeof(buf), 0, NULL, NULL);
        char hex_str[9];
        for (int i = 0; i < 4; i++)
        {
            sprintf(&hex_str[i * 2], "%02X", (unsigned char)buf[i]);
        }
        if (hex_str != NULL)
        {
            // 操作数据库(查询)
            sprintf(sql_syntx, "SELECT rname FROM user where ic='%s'", hex_str);

            if (query_data(sql_syntx) != NULL)
            {
                char realname[20] = "";
                strcpy(realname, query_data(sql_syntx));
                bzero(sql_syntx, sizeof(sql_syntx));
                sprintf(sql_syntx, "INSERT INTO records (rname, time, ic) VALUES ('%s', '%s', '%s');", realname, get_current_time(), hex_str);
                sql_operation(sql_syntx);
            }
            else
            {
                bzero(sql_syntx, sizeof(sql_syntx));
                sprintf(sql_syntx, "INSERT INTO newcard (ic) VALUES ('%s');", hex_str);
                if (sql_operation(sql_syntx) == 1)
                {
                    perror("sql_operation");
                    return -1;
                }
            }
        }
    }
    close(sockfd);
    return 0;
}

// 查询并返回结果
char *query_data(const char *sql)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char *server = "10.9.29.135";
    char *user = "root";           // 数据库用户名
    char *password = "123456";     // 数据库密码
    char *database = "attendance"; // 数据库名
    conn = mysql_init(NULL);

    // 连接到 MySQL 服务器
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(conn));
        return NULL;
    }

    // 执行 SQL 查询
    if (mysql_query(conn, sql))
    {
        fprintf(stderr, "Failed to execute SQL query: Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }

    // 获取查询结果
    res = mysql_store_result(conn);
    if (!res)
    {
        fprintf(stderr, "Failed to get result set: Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }

    // 读取查询结果
    row = mysql_fetch_row(res);
    if (!row)
    {
        // fprintf(stderr, "Empty result set\n");
        mysql_free_result(res);
        mysql_close(conn);
        return NULL;
    }

    // 将查询结果转换为字符串
    char *result_str = strdup(row[0]); // strdup 分配内存并复制字符串
    if (!result_str)
    {
        perror("strdup");
        mysql_free_result(res);
        mysql_close(conn);
        return NULL;
    }

    // 释放资源
    mysql_free_result(res);
    mysql_close(conn);

    return result_str;
}
// 非查询语句
int sql_operation(const char *sql)
{
    MYSQL *conn;
    char *server = "10.9.29.135";
    char *user = "root";           // 数据库用户名
    char *password = "123456";     // 数据库密码
    char *database = "attendance"; // 数据库名
    conn = mysql_init(NULL);

    // 连接到 MySQL 服务器
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(conn));
        return 1;
    }

    // 执行 SQL 查询
    if (mysql_query(conn, sql))
    {
        fprintf(stderr, "Failed to execute SQL query: Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    // 释放资源
    mysql_close(conn);
    return 0;
}

// 获取当前时间，并格式化成字符串
char *get_current_time()
{
    time_t current_time = time(NULL);
    struct tm *tm_info = localtime(&current_time);
    static char current_time_str[20]; // 使用 static 保证变量在函数结束后不被释放
    strftime(current_time_str, sizeof(current_time_str), "%Y-%m-%d %H:%M:%S", tm_info);
    return current_time_str;
}