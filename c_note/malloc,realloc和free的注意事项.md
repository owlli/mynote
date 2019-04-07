# malloc,realloc和free的注意事项



## void *malloc(size_t size);

1. malloc返回值有可能为NULL,所以调用后必须要判断返回值;
2. malloc返回值指向进程的堆中的地址,不是指向栈中的地址,如果未释放,堆中的那块内存就一直不能被再次分配,所以使用完后要及时free释放.



## void *realloc(void *ptr, size_t size);

1. ptr必须为NULL,或是malloc,realloc,calloc获得的指向堆中的指针,否则编译时发生"relloc invaild pointer"错误;
2. size如果小于ptr之前指向的空间大小,只有size大小的数据会保存,很可能发生数据丢失,慎重使用;
3. 如果size大于ptr之前所指向的空间大小,返回值可能会指向一块新的内存空间(比如ptr之前指向的地址没有足够的连续空间容纳size字节,系统会在堆中重新找一块可以容纳连续size字节的空间,并将此空间的起始地址返回),此时ptr所指向的内存会被释放,ptr成为野指针,再次使用时会发生错误;
4. 不要将返回值结果再次赋值给ptr,即`ptr = realloc(ptr,new_size)`是不建议使用的.因为如果内存分配失败,ptr会变为NULL,之前没有将ptr的值赋值给其他值的话,会丢失ptr之前的值,并且由于没有free(ptr),还会造成内存泄露.所以建议新定义指针变量temp,`temp = realloc(ptr,new_size)`;
5. 如果size的值为0,realloc返回的值则为一个已经free的地址,再free就会引起"double free"的错误(下文有介绍).而malloc(0)的返回地址则为合法指针,可以free.



## void free(void *ptr);

1. 不要对一个指针连续free两次.free(p)代表告诉进程,已经将malloc得到的p释放了,p所指向的堆空间可以继续被malloc分配了,但是p的值没有发生变化(即p为野指针,**所以建议free(p)后,执行`p = NULL`**).如果继续free(p),可能在free(p)前p所指的那块内存被malloc分配了,再次free会造成内存管理的混乱.(其实malloc返回值p所指向的地址的前一个地址存着malloc分配的大小,所以free(p)时,系统能知道释放p所指向内存后的多大空间)

   gcc7.3.0在编译存在连续free的代码时不会报错(据说在老版本中会报错),执行编译后的可执行程序时也不会报错.free(p)后,*p的值为0,推测是将malloc得到的那段堆空间所有数据全部初始化为0了.

   mac中的clang编译器在编译存在连续free的代码时也不会报错,但在执行编译后的可执行程序时会报错.free(p)后,*p的值仍为原来的值.