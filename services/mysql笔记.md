# mysql笔记

## 安装数据库

```shell
sudo apt install mariadb-server
```

### 配置数据库

```shell
sudo mysql_secure_installation
```

## 连接数据库

```shell
mysql -uroot -p
```

### 普通用户无法登录mysql的root用户的解决办法

1. linux的root用户登录mysql的root用户
2. `use mysql`
3. `update user set authentication_string=PASSWORD('000000'),plugin='mysql_native_password' where user='root';`(000000代表新设置的mysql的root密码)
4. 重启mysqld服务

## 创建数据库

`create database runoob;`

## 删除数据库

`drop database runoob;`

## 选择数据库

`use runoob`

## 创建数据表

```
CREATE TABLE IF NOT EXISTS `runoob_tbl`(
   `runoob_id` INT UNSIGNED AUTO_INCREMENT,
   `runoob_title` VARCHAR(100) NOT NULL,
   `runoob_author` VARCHAR(40) NOT NULL,
   `submission_date` DATE,
   PRIMARY KEY ( `runoob_id` )
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
```

> 实例解析：
>
> - 如果你不想字段为 **NULL** 可以设置字段的属性为 **NOT NULL**， 在操作数据库时如果输入该字段的数据为**NULL** ，就会报错。
> - AUTO_INCREMENT定义列为自增的属性，一般用于主键，数值会自动加1。
> - PRIMARY KEY关键字用于定义列为主键。 您可以使用多列来定义主键，列间以逗号分隔。
> - ENGINE 设置存储引擎，CHARSET 设置编码。

## 删除数据表

`drop table runoob_tbl`

## 插入数据

```
INSERT INTO table_name ( field1, field2,...fieldN )
                       VALUES
                       ( value1, value2,...valueN );
```

例如:

```
INSERT INTO runoob_tbl
(runoob_title, runoob_author, submission_date)
VALUES
("学习 PHP", "菜鸟教程", NOW());
```

## 查询数据

```
SELECT column_name,column_name
FROM table_name
[WHERE Clause]
[LIMIT N][ OFFSET M]
```

> - 查询语句中你可以使用一个或者多个表，表之间使用逗号(,)分割，并使用WHERE语句来设定查询条件。
> - SELECT 命令可以读取一条或者多条记录。
> - 你可以使用星号（*）来代替其他字段，SELECT语句会返回表的所有字段数据
> - 你可以使用 WHERE 语句来包含任何条件。
> - 你可以使用 LIMIT 属性来设定返回的记录数。
> - 你可以通过OFFSET指定SELECT语句开始查询的数据偏移量。默认情况下偏移量为0。

## 修改数据

```
UPDATE table_name SET field1=new-value1, field2=new-value2
[WHERE Clause]
```

> - 你可以同时更新一个或多个字段。
> - 你可以在 WHERE 子句中指定任何条件。
> - 你可以在一个单独表中同时更新数据。

```
 UPDATE runoob_tbl SET runoob_title='学习 C++' WHERE runoob_id=3;
```

## 删除数据

```
DELETE FROM table_name [WHERE Clause]
```

> - 如果没有指定 WHERE 子句，MySQL 表中的所有记录将被删除。
> - 你可以在 WHERE 子句中指定任何条件
> - 您可以在单个表中一次性删除记录。

## where子句

```
SELECT field1, field2,...fieldN FROM table_name1, table_name2...
[WHERE condition1 [AND [OR]] condition2.....
```

> - 查询语句中你可以使用一个或者多个表，表之间使用逗号, 分割，并使用WHERE语句来设定查询条件。
> - 你可以在 WHERE 子句中指定任何条件。
> - 你可以使用 AND 或者 OR 指定一个或多个条件。
> - WHERE 子句也可以运用于 SQL 的 DELETE 或者 UPDATE 命令。
> - WHERE 子句类似于程序语言中的 if 条件，根据 MySQL 表中的字段值来读取指定的数据。

以下为操作符列表，可用于 WHERE 子句中。

下表中实例假定 A 为 10, B 为 20

| 操作符 | 描述                                                         | 实例                 |
| ------ | ------------------------------------------------------------ | -------------------- |
| =      | 等号，检测两个值是否相等，如果相等返回true                   | (A = B) 返回false。  |
| <>, != | 不等于，检测两个值是否相等，如果不相等返回true               | (A != B) 返回 true。 |
| >      | 大于号，检测左边的值是否大于右边的值, 如果左边的值大于右边的值返回true | (A > B) 返回false。  |
| <      | 小于号，检测左边的值是否小于右边的值, 如果左边的值小于右边的值返回true | (A < B) 返回 true。  |
| >=     | 大于等于号，检测左边的值是否大于或等于右边的值, 如果左边的值大于或等于右边的值返回true | (A >= B) 返回false。 |
| <=     | 小于等于号，检测左边的值是否小于于或等于右边的值, 如果左边的值小于或等于右边的值返回true | (A <= B) 返回 true。 |

如果我们想在 MySQL 数据表中读取指定的数据，WHERE 子句是非常有用的。

使用主键来作为 WHERE 子句的条件查询是非常快速的。

如果给定的条件在表中没有任何匹配的记录，那么查询不会返回任何数据。

```
SELECT * from runoob_tbl WHERE runoob_author='菜鸟教程';
```

### like

```
SELECT field1, field2,...fieldN 
FROM table_name
WHERE field1 LIKE condition1 [AND [OR]] filed2 = 'somevalue'
```

> - 你可以在 WHERE 子句中指定任何条件。
> - 你可以在 WHERE 子句中使用LIKE子句。
> - 你可以使用LIKE子句代替等号 =。
> - LIKE 通常与 % 一同使用，类似于一个元字符的搜索。
> - 你可以使用 AND 或者 OR 指定一个或多个条件。
> - 你可以在 DELETE 或 UPDATE 命令中使用 WHERE...LIKE 子句来指定条件。

like 匹配/模糊匹配，会与 % 和 _ 结合使用。

```
'%a'     //以a结尾的数据
'a%'     //以a开头的数据
'%a%'    //含有a的数据
'_a_'    //三位且中间字母是a的
'_a'     //两位且结尾字母是a的
'a_'     //两位且开头字母是a的
```

## union

```
SELECT expression1, expression2, ... expression_n
FROM tables
[WHERE conditions]
UNION [ALL | DISTINCT]
SELECT expression1, expression2, ... expression_n
FROM tables
[WHERE conditions];
```

> ### 参数
>
> - **expression1, expression2, ... expression_n**: 要检索的列。
> - **tables:** 要检索的数据表。
> - **WHERE conditions:** 可选， 检索条件。
> - **DISTINCT:** 可选，删除结果集中重复的数据。默认情况下 UNION 操作符已经删除了重复数据，所以 DISTINCT 修饰符对结果没啥影响。
> - **ALL:** 可选，返回所有结果集，包含重复数据。

## order排序

```
SELECT field1, field2,...fieldN table_name1, table_name2...
ORDER BY field1, [field2...] [ASC [DESC]]
```

> - 你可以使用任何字段来作为排序的条件，从而返回排序后的查询结果。
> - 你可以设定多个字段来排序。
> - 你可以使用 ASC 或 DESC 关键字来设置查询结果是按升序或降序排列。 默认情况下，它是按升序排列。
> - 你可以添加 WHERE...LIKE 子句来设置条件。

## group by分组

```
SELECT column_name, function(column_name)
FROM table_name
WHERE column_name operator value
GROUP BY column_name;
```

例子:

```
SET NAMES utf8;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
--  Table structure for `employee_tbl`
-- ----------------------------
DROP TABLE IF EXISTS `employee_tbl`;
CREATE TABLE `employee_tbl` (
  `id` int(11) NOT NULL,
  `name` char(10) NOT NULL DEFAULT '',
  `date` datetime NOT NULL,
  `singin` tinyint(4) NOT NULL DEFAULT '0' COMMENT '登录次数',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Records of `employee_tbl`
-- ----------------------------
BEGIN;
INSERT INTO `employee_tbl` VALUES ('1', '小明', '2016-04-22 15:25:33', '1'), ('2', '小王', '2016-04-20 15:25:47', '3'), ('3', '小丽', '2016-04-19 15:26:02', '2'), ('4', '小王', '2016-04-07 15:26:14', '4'), ('5', '小明', '2016-04-11 15:26:40', '4'), ('6', '小明', '2016-04-04 15:26:54', '2');
COMMIT;

SET FOREIGN_KEY_CHECKS = 1;
```

```
mysql> SELECT * FROM employee_tbl;
+----+--------+---------------------+--------+
| id | name   | date                | singin |
+----+--------+---------------------+--------+
|  1 | 小明 | 2016-04-22 15:25:33 |      1 |
|  2 | 小王 | 2016-04-20 15:25:47 |      3 |
|  3 | 小丽 | 2016-04-19 15:26:02 |      2 |
|  4 | 小王 | 2016-04-07 15:26:14 |      4 |
|  5 | 小明 | 2016-04-11 15:26:40 |      4 |
|  6 | 小明 | 2016-04-04 15:26:54 |      2 |
+----+--------+---------------------+--------+
6 rows in set (0.00 sec)
```

###　count(*)统计

```
mysql> SELECT name, COUNT(*) FROM   employee_tbl GROUP BY name;
+--------+----------+
| name   | COUNT(*) |
+--------+----------+
| 小丽 |        1 |
| 小明 |        3 |
| 小王 |        2 |
+--------+----------+
3 rows in set (0.01 sec)
```

### sum(),with rollup统计

WITH ROLLUP 可以实现在分组统计数据基础上再进行相同的统计（SUM,AVG,COUNT…）。

例如我们将以上的数据表按名字进行分组，再统计每个人登录的次数：

```
mysql> SELECT name, SUM(singin) as singin_count FROM  employee_tbl GROUP BY name WITH ROLLUP;
+--------+--------------+
| name   | singin_count |
+--------+--------------+
| 小丽 |            2 |
| 小明 |            7 |
| 小王 |            7 |
| NULL   |           16 |
+--------+--------------+
4 rows in set (0.00 sec)
```

其中记录 NULL 表示所有人的登录次数。

### coalesce取代NULL字段

我们可以使用 coalesce 来设置一个可以取代 NUll 的名称，coalesce 语法：

```
select coalesce(a,b,c);
```

参数说明：如果a==null,则选择b；如果b==null,则选择c；如果a!=null,则选择a；如果a b c 都为null ，则返回为null（没意义）。

以下实例中如果名字为空我们使用总数代替：

```
mysql> SELECT coalesce(name, '总数'), SUM(singin) as singin_count FROM  employee_tbl GROUP BY name WITH ROLLUP;
+--------------------------+--------------+
| coalesce(name, '总数') | singin_count |
+--------------------------+--------------+
| 小丽                   |            2 |
| 小明                   |            7 |
| 小王                   |            7 |
| 总数                   |           16 |
+--------------------------+--------------+
4 rows in set (0.01 sec)
```

### 其他笔记

1、group by 可以实现一个最简单的去重查询，假设想看下有哪些员工，除了用 distinct,还可以用：

```
SELECT name FROM employee_tb1 GROUP BY name;
```

返回的结果集就是所有员工的名字。

2、group by 语句用法有一个注意点，在 select 语句中，所查询的字段除了聚合函数（SUM ,AVG,COUNT...)以外 必须只能是分组的字段，举例：

```
SELECT id,name,count(*) FROM employee_tb1 GROUP BY name;
```

运行该语句程序会报错，因为 id 字段并不包含在 GROUP BY 分组中，改为如下：

```
SELECT id,name ,count(*)FROM employee_tb1 GROUP BY  id,name;
```

则程序可以正常运行。

3、分组后的条件使用 HAVING 来限定，WHERE 是对原始数据进行条件限制。几个关键字的使用顺序为 where 、group by 、having、order by ，例如：

```
SELECT name ,sum(*)  FROM employee_tb1 WHERE id<>1 GROUP BY name  HAVING sum(*)>5 ORDER BY sum(*) DESC;
```

## NULL值处理

我们已经知道 MySQL 使用 SQL SELECT 命令及 WHERE 子句来读取数据表中的数据,但是当提供的查询条件字段为 NULL 时，该命令可能就无法正常工作。

为了处理这种情况，MySQL提供了三大运算符:

- **IS NULL:** 当列的值是 NULL,此运算符返回 true。
- **IS NOT NULL:** 当列的值不为 NULL, 运算符返回 true。
- **<=>:** 比较操作符（不同于=运算符），当比较的的两个值为 NULL 时返回 true。

关于 NULL 的条件比较运算是比较特殊的。你不能使用 = NULL 或 != NULL 在列中查找 NULL 值 。

在 MySQL 中，NULL 值与任何其它值的比较（即使是 NULL）永远返回 false，即 NULL = NULL 返回false 。

MySQL 中处理 NULL 使用 IS NULL 和 IS NOT NULL 运算符。









