#include <stdio.h>
#include <json-c/json.h>
#include <stdbool.h>

int main()
{
    // 创建一个 JSON 字符串
    const char *json_str = "{\"name\": \"John\", \"age\": 30, \"married\": true}";

    // 解析 JSON 字符串
    json_object *json_obj = json_tokener_parse(json_str);

    // 获取姓名
    json_object *name_obj;
    if (json_object_object_get_ex(json_obj, "name", &name_obj))
    {
        const char *name = json_object_get_string(name_obj);
        printf("Name: %s\n", name);
    }

    // 获取年龄
    json_object *age_obj;
    if (json_object_object_get_ex(json_obj, "age", &age_obj))
    {
        int age = json_object_get_int(age_obj);
        printf("Age: %d\n", age);
    }

    // 获取婚姻状态
    json_object *married_obj;
    if (json_object_object_get_ex(json_obj, "married", &married_obj))
    {
        bool married = json_object_get_boolean(married_obj);
        printf("Married: %s\n", married ? "true" : "false");
    }

    // 释放 JSON 对象
    json_object_put(json_obj);

    return 0;
}
