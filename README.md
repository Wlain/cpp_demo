# cpp_demo
##一些开发中遇到的实践经验

1.有override就不要写virtual了

2.注意命名

3.类里面bool写在最后，注意顺序

4.注意传引用还是传值，不能传一个局部变量给一个引用

5.不是简单构造的， 都写到构造函数里面去

6.简单的类变量头文件直接初始化

7.bool变量的命名：如

```C++
enableXXX(bool flag)：表示一个方法
shouldXXX ：表示一个变量
```

8.有时候报奇奇怪怪的语法错误，检查下是否有引头文件

9.shared_ptr get指针，如果对象不为空，get到的是对象，如果对象为空，get到一个nullptr

10.更多地去考虑性能，尽量别传临时对象，传引用

11.有些代码需要适当加注释。

12.注意代码对齐。

13.C++返回引用给lua的话，lua持有的是一个临时变量的指针，可能会有问题

14.C++的void* ， 在lua里面可能无法表示， 需要用到 CGE::ByteBuffer 

15.std::string类型的构造不需要初始化

16."&&" 的优先级是比 "||" 更高的

17.基础类型不要传引用（比如枚举，int，float）

18.~test() override = default;这样的写法是没必要的可以忽略

19.share_ptr 的resize需要注意内部的resize，需要补齐数据

```C++
if (shared_ptrA->size() < size)
{
	shared_ptrA->resize(size);
	for (auto& f : *shared_ptrA)
	{
		if (!f)
		{
			f = MAKE_SHARED(f);
		}
	}
}
```

20.子类定义变量不要和父类同名，注意父类

21.多次调用使用内联函数

22.C++临时变量的生命周期（const 引用后延长临时变量周期)，详细见提交-（1263658b485d1a69ee84513497d50318e71d7257）

23.静态成员函数不能具有“ const”限定符

24.类中定义成员变量：

```C++
class Test
{
    Test()
    {
        constructorCount++;
    }
  	virtual ~Test() = default;
    static inline static int getConstructorCount()
    {
        return constructorCount;
    }
private:
    static int constructorCount;
};

int Test::constructorCount = 0;
// 必须在定义类的文件中对静态成员变量进行一次说明,或初始化。否则编译能通过，链接不能通过。
int main()
{
  return 0;
  Test *t = new Test[10];
  delete[] t;
}
```

25.宏定义：宏定义，本质上就是替换，参数需要加括号

```C++
#define COM(A,B) (A)*(B)
// 那么COM(6+5,3)它会换成这样: (6+5)*(3)
// 显然这是和COM宏的意图一致的，但是如是去掉了定义中括号，即写成这样：
#define COM(A,B) A*B
// 那么COM(6+5,3)它就会换成这样: 6+5*3, 这样显然就和宏的意图不符合了。
```

26.虚函数派生类的函数不能带有缺省参数，因为缺省参数是静态编译，而虚函数机制是动态编译

```C++
virtual void A(int a = 0); // error
virtual void B(int a); // ok
```

27.使用'= default'定义一个简单的默认构造函数

28.尽可能得使用const auto&，从而减少拷贝

29.%lu表示输出无符号长整型整数 (long unsigned)

30.遍历整个array并且打印：
```C++
std::reverse_copy(arr.begin(), arr.end(),
                  std::ostream_iterator<int>(std::cout, " "));
```

##C++ 编程规范 101条规则、准则与最佳实践
001.了解哪些东西不应该标准化

002.在搞警告级别干净利落地进行编译

003.使用自动构建系统

004.使用版本控制系统

005.一个实体应该只有一个紧凑的职责

006.正确、简单和清晰第一

007.编程中应该知道如何考虑可伸缩性

008.不要进行不成熟的优化

009.不要进行不成熟的劣化

010.尽量减少全局和共享数据





