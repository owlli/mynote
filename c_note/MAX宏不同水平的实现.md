# MAX宏不同水平的实现



```c
#define MAX(a,b) a>b?a:b

#define MAX(a,b) (a>b?a:b)

#define MAX(a,b) ((a)>(b)?(a):(b))

#define MAX(a,b,type) ({type _a=(a),_b=(b);(_a>_b?_a:_b);})//必须用小括号括起来,否则编译报语法错误
```



