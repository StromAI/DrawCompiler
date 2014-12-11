DrawCompiler
============

A compiler with bug(For a function draw language)

**DrawCompiler**是XDU编译原理的实验内容，其功能是编译一种由XDU自定的函数绘图语言，其内容简单的描述大概如下（被我简化过的）：
* FOR T FROM A TO B STEP C DRAW（FUNCA（T）,FUNCB（T））
* //从A到B每隔C取值给T通过DRAW中的FUNCA和FUNCB确定点的位置循环绘图
* SCALE IS (X,Y)
* //之后接FOR语句进行绘制，将图形的横纵坐标分别缩放X，Y
* ROT IS α
* //之后接FOR语句进行绘制，顺时针旋转图形α
* ORIGIN IS (X,Y)
* //之后接FOR语句进行绘制，将图形横向纵向移动X，Y
* 最后该语言支持"//"和"--"形式的注释符
* 上文中A、B、C、X、Y、α为需要解析的参数，FUNC为需解析的函数

---
**DrawCompiler**使用C++的boost.python将词法及语法（LL）分析器封装成cppcompilers.so通过外部接口与Python交互并在Python中将语义重新解释为MATLAB命令，通过MATLAB进行图像绘制。

目前基本功能已经实现，存在的问题包括:
* 错误语句的处理：由于时间比较紧没有做异常的处理，所以错误的输入只会显示在输出信息中但仍会使用错误的参数启动MATLAB导致MATLAB绘图出现错误
* 无法连续进行绘制：编写之初没有想到这个问题，可能需要修改C++外部接口来解决
* 绘制之后不能制动退出MATLAB：由于没有具体的延迟时间暂时不考虑这一问题，其次感觉用其他方式完成图像的绘制似乎是更好的选择

---
总之自己用了最简单暴力的方法实现了**DrawCompiler**的基本内容，较好的选择应该是LR加QT或者其他什么的吧，不过这种方案的优势大概在于比较快吧（毕竟赶时间Orz）
