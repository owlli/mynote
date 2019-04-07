# cJSON的简单使用



## 1.cjson结构体及部分宏定义介绍

cJSON是一个结构体,它的成员变量type标志了它的类型,类型见下方代码中的宏

一般使用方法是通过cJSON_CreateObject函数建一个object类型的cJSON,再在里面添加其他类型的cJSON

cJSON能识别的字符串中,字符串都要加双引号,数组都要用[]括起来,在cJSON_Print生成的字符串中,一行就代表一个cJSON,冒号之前的是此行cJSON的string成员变量的值,之后是它type类型代表的值

```c
/* cJSON Types: */
#define cJSON_Invalid (0)
#define cJSON_False  (1 << 0)
#define cJSON_True   (1 << 1)
#define cJSON_NULL   (1 << 2)
#define cJSON_Number (1 << 3)
#define cJSON_String (1 << 4)
#define cJSON_Array  (1 << 5)
#define cJSON_Object (1 << 6)
#define cJSON_Raw    (1 << 7) /* raw json */


/* The cJSON structure: */
typedef struct cJSON
{
    /* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem ,cJSON结构体包含一个双向链表*/
    struct cJSON *next;
    struct cJSON *prev;
    /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */
    struct cJSON *child;
    /* The type of the item, as above. ,类型就是上文提供的宏*/
    int type;
    /* The item's string, if type==cJSON_String  and type == cJSON_Raw */
    char *valuestring;
    /* writing to valueint is DEPRECATED, use cJSON_SetNumberValue instead */
    int valueint;
    /* The item's number, if type==cJSON_Number */
    double valuedouble;
    /* The item's name string, if this item is the child of, or is in the list of subitems of an object. ,cJSON结构体变量的名字,只有通过cJSON_AddItemToObject函数添加的cJSON此变量才有值*/
    char *string;
} cJSON;
```

## 2.常用函数介绍

1. 解析函数

```c
extern cJSON *cJSON_Parse(const char *value);//从给定的json字符串中得到cjson对象

extern char  *cJSON_Print(cJSON *item);//从cjson对象中获取有格式的json对象

extern char  *cJSON_PrintUnformatted(cJSON *item);//从cjson对象中获取无格式的json对象

extern void   cJSON_Delete(cJSON *c);//删除cjson对象，释放链表占用的内存空间

extern int    cJSON_GetArraySize(cJSON *array);//获取cjson对象数组成员的个数
/* Retrieve item number "item" from array "array". Returns NULL if unsuccessful. */
extern cJSON *cJSON_GetArrayItem(cJSON *array,int item);//根据下标获取cjosn对象数组中的对象
/* Get item "string" from object. Case insensitive. */
extern cJSON *cJSON_GetObjectItem(cJSON *object,const char *string);//根据键获取对应的值（cjson对象）
/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when cJSON_Parse() returns 0. 0 when cJSON_Parse() succeeds. */
extern const char *cJSON_GetErrorPtr(void);//获取错误字符串
```

程序退出时记得释放cJSON_Print的返回值

2. 构造函数

```c
extern void cJSON_AddItemToObject(cJSON *object,const char *string,cJSON *item);//在object下创建一个cJSON--item,item的string成员变量为string
extern void cJSON_AddItemToArray(cJSON *array, cJSON *item);//在array数组中添加一个cJSON成员item

extern cJSON *cJSON_CreateObject(void);//创建一个cJSON结构体指针
extern cJSON *cJSON_CreateNull(void);
extern cJSON *cJSON_CreateTrue(void);
extern cJSON *cJSON_CreateFalse(void);
extern cJSON *cJSON_CreateBool(cJSON_bool boolean);
extern cJSON *cJSON_CreateNumber(double num);
extern cJSON *cJSON_CreateString(const char *string);
extern cJSON *cJSON_CreateArray(void);//创建一个数组
extern cJSON *cJSON_CreateObject(void);

//下面的函数实现的功能和cJSON_AddItemToObject()类似,都是创建一个新的cJSON,这些函数也是通过调用add_item_to_object()函数实现的
extern cJSON *cJSON_AddNullToObject(cJSON * const object, const char * const name);
extern cJSON *cJSON_AddTrueToObject(cJSON * const object, const char * const name);
extern cJSON *cJSON_AddFalseToObject(cJSON * const object, const char * const name);
extern cJSON *cJSON_AddBoolToObject(cJSON * const object, const char * const name, const cJSON_bool boolean);
extern cJSON *cJSON_AddNumberToObject(cJSON * const object, const char * const name, const double number);
extern cJSON *cJSON_AddStringToObject(cJSON * const object, const char * const name, const char * const string);
extern cJSON *cJSON_AddRawToObject(cJSON * const object, const char * const name, const char * const raw);
extern cJSON *cJSON_AddObjectToObject(cJSON * const object, const char * const name);
extern cJSON *cJSON_AddArrayToObject(cJSON * const object, const char * const name);

extern void cJSON_Delete(cJSON *c);//释放c及c包含的*cJSON
```

程序退出时记得要释放cJSON_CreateObject创建的结构体

