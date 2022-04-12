# Emehcs - 一个大约的 Scheme 解释器/就叫这个名字的语言

[English](./readme.md) | 简体中文

基于纯 C++20 和它的 STL。

- C++17 for `std::varient`（变体），折叠表达式，结构化绑定，`if` 的初始化语句，等等；
- C++20 for `std::unordered_set` 的 `contains`、`std::string` 的 `starts_with` 和 `ends_with`。

## 如何编译

```shell
mkdir -p _build && cd _build
cmake ..
make
```

## CLI

```shell
emehcs    # 启动 REPL
emehcs filename.scm   # 解释运行 Scheme/Emehcs 源文件
```

## 简易文档

### 目录

- [Emehcs - 一个大约的 Scheme 解释器/就叫这个名字的语言](#emehcs---一个大约的-scheme-解释器就叫这个名字的语言)
  - [如何编译](#如何编译)
  - [CLI](#cli)
  - [简易文档](#简易文档)
    - [目录](#目录)
    - [基本语法](#基本语法)
      - [数据类型](#数据类型)
        - [原子（Atom）](#原子atom)
        - [列表（List）](#列表list)
        - [点列表（DottedList）](#点列表dottedlist)
        - [数（Number）](#数number)
        - [字符（Char）](#字符char)
        - [字符串（String）](#字符串string)
        - [布尔值（Bool）](#布尔值bool)
        - [函数（Function）](#函数function)
      - [定义变量和函数](#定义变量和函数)
      - [分支](#分支)
      - [循环](#循环)
    - [内置函数（Built-in Functions）](#内置函数built-in-functions)
      - [`load` 函数](#load-函数)
    - [文件头](#文件头)
    - [结语](#结语)

### 基本语法

Emehcs 它的所有语句都由列表（List）构成，组成语言的基本单位是原子（Atom），所有的求值均为函数调用。

举例一个最基本的加法运算：

```scheme
(+ 1 2)
; => 3
```

举例嵌套的基本运算：

```scheme
(+ 2 (* 3 2))
; => 8
```

Emehcs 的单行注释由 `;` 开头。

#### 数据类型

Emehcs 包含以下基本数据类型，以及与它们对应的 C++ 数据结构：

| Emehcs 数据类型 | C++ 数据类型                                                 |
| --------------- | ------------------------------------------------------------ |
| Atom            | struct {std::string}                                         |
| List            | std::deque\<ValueP\>                                         |
| DottedList      | std::pair\<std::deque\<ValueP\>, ValueP\>                    |
| Number          | std::int64_t                                                 |
| Char            | char                                                         |
| String          | std::string                                                  |
| Bool            | bool                                                         |
| Function        | struct {std::deque\<std::string\>, std::deque\<ValueP\>, EnvironmentP} |

因为 Lisp 的主旨是用函数表示数据，所以暂时不支持复杂型数据结构。

列举各种类型的字面量：

##### 原子（Atom）

以字母或符号开头的、包含若干字母或符号或数字的串，如 `atom`、`number42`、`--perhaps-an-atom`。

##### 列表（List）

用于调用函数、求值用的列表，使用一对圆括号内含元素表示，元素之间使用若干空白字符隔开。如：

```scheme
; 不可以求值的用于存储数据的列表要使用单引号引用
'(1 2 3)

; 函数调用
(+ 1 2 3)

; 这下面两行也是一个列表
'(#\a   3
        #\b      #\c)
```

##### 点列表（DottedList）

在 Lisp 中列表是由点列表组成的，但是在 Emehcs 中，点列表的意义不明，可以自行测试 `car`、`cdr`、`cons` 等函数。

内部由一个列表和单个元素组成，与列表同样使用圆括号内含元素表示，但是在尾元素的前面需要额外添加一个 Atom `.` 的元素，表示这是一个点列表。列表可以为空，但是尾部元素一定不能为空。如

```scheme
'(1 . 3)
'(1 2 3 . 4)
'( . 1)
```

##### 数（Number）

Emehcs 的数字面量目前只可以表示非负整数，但是数可以存储负整数。分别有十进制、~~二进制、~~八进制、十六进制的字面量。比如：

```scheme
; 十进制整数 42
42

; 使用 `-0` 函数计算得到的负数 -42
(-0 42)

; 二进制数（论文里写了这个，但是并不支持）
;;;#b10100010

; 八进制数
#o12345670

; 十六进制数
#xABCD
#xabcd
```

##### 字符（Char）

字符字面量以 `#\` 开头，后接一个字符，该字符可以为转义字符：

```scheme
; 字符 a
#\a

; 字符 1
#\1

; 换行符
#\\n

; Scheme 的空格表示暂时不支持
;;;#\space
```

##### 字符串（String）

使用一对双引号包括，可以转义：

```scheme
"hello \\ \"world\"\n"
```

##### 布尔值（Bool）

二择，`#t` 和 `#f`，分别用来表示布尔真值和布尔假值。

##### 函数（Function）

函数使用 lambda 表达式表示。lambda 表达式由 `lambda` 关键字、参数列表和函数体构成，函数体的最后一个部分为其返回值。举例：

```scheme
; 普通的加法函数
(lambda (x y)
  (+ x y))

; 函数变量作为形式参数
(lambda (foo x)
  (foo (* 2 x)))

; 高阶函数
(lambda (binary-func p1)
  (lambda (p2)
    (binary-func p1 p2)))

; 顺序式过程（不推荐在面向函数的语言中这么写代码
; 参数列表可以为空
(define s "aaa")
(lambda ()
  (string-replace! s #\a #\b)
  (string-replace! s #\b #\c))
; => "ccc"
```

#### 定义变量和函数

目前只可以定义在全局环境中的全局变量（在 Emehcs 中函数也是变量或者值）。

使用 `define` 关键字：

```scheme
; 定义一个整数
(define god 42)

; 定义一个字符串
(define str "hello world")

; 定义一个函数（使用 lambda 表达式）
(define foo (lambda (x y)
               (+ x y)))

; 定义一个函数（使用 define 提供的特殊形式）
(define (foo2 x y)      ; 将函数名 foo2 放到参数列表中，将 lambda 关键字替换成 define
  (+ x y))
```

#### 分支

Emehcs 提供两种特殊函数作为分支语句，`if` 和 `cond`。使用方法如下：

```scheme
(if condition true-clause false-clause)

(cond (condition1 clause1)
      (condition2 clause2)
      ...
      (else else-clause))
```

举例如：

```scheme
(if (/= 1 42) "Normal" "World ends")
; => "Normal"

(cond ((> 3 4) 1)
      ((< 4 3) 2)
      (#t 3))       ; 使用 #t 或 else 均可
; => 3
```

#### 循环

Emehcs 暂时不提供循环语句。可以用尾递归实现循环，比如：

```scheme
(define (iota-sum n)
  (if (= n 0)
      0
      (+ n (iota-sum (- n 1)))))
(iota-sum 1000)
; => 500500
```

但是要注意现阶段 Emehcs 还没有尾递归优化，所以上面的递归参数再大点程序就会崩溃了。

### 内置函数（Built-in Functions）

暂时没有时间为每一个函数写文档，我写点注释，函数功能基本跟 Scheme 等同：

```scheme
const ::std::unordered_map<::std::string, ::std::function<ValueP(ValueP, EnvironmentP)>> BuiltInFunctor{
        {"quote",  funcQuote},
        {"if",     funcIf},
        {"cond",   funcCond},
        {"define", funcDefine},
        {"string", funcString},
};

const ::std::unordered_map<::std::string,
                           ::std::function<ValueP(ValueP, EnvironmentP)>> UnaryOps {
        {"-0", numericUnopMinus},
        {"not", boolBoolUnopNot},
        {"car", listCar},
        {"cdr", listCdr},
        {"load", loadFromFile},
        {"list->string", strList2String},
        {"string->list", strString2List},
        {"string-copy", strStringCopy},
        {"string?", strIsString},
        {"string-length", strStringLength},
        {"string-null?", strIsStringNull},
        {"string-hash", strStringHash},
        {"string-capitalized?", strIsStringCapitalized},
        {"string-upper-case?", strIsStringUpperCase},
        {"string-lower-case?", strIsStringLowerCase},
        {"string-capitalize", strStringCapitalize},
        {"string-capitalize!", strStringCapitalizeBang},
        {"string-downcase", strStringDowncase},
        {"string-downcase!", strStringDowncaseBang},
        {"string-upcase", strStringUpcase},
        {"string-upcase!", strStringUpcaseBang},
        {"string-trim", strStringTrim},
        {"string-trim-left", strStringTrimLeft},
        {"string-trim-right", strStringTrimRight},
};

const ::std::unordered_map<::std::string, ::std::function<ValueP(ValueP, ValueP, EnvironmentP)>> BinaryOps {
        {"=", numBoolBinopEq},
        {"<", numBoolBinopL},
        {"<=", numBoolBinopLe},
        {">", numBoolBinopG},
        {">=", numBoolBinopGe},
        {"/=", numBoolBinopNeq},
        {"string=?", strBoolBinopEq},
        {"string<?", strBoolBinopL},
        {"string<=?", strBoolBinopLe},
        {"string>?", strBoolBinopG},
        {"string>=?", strBoolBinopGe},
        {"cons", listCons},
        {"eq?", eqv},
        {"eqv?", eqv},
        {"make-string", strMakeString},
        {"assert", EqAssert},
        {"string-ref", strStringRef},
        {"string-head", strStringHead},
        {"string-tail", strStringTail},
        {"string-pad-left", strStringPadLeft},
        {"string-pad-right", strStringPadRight},
        {"substring?", strIsSubstring},
        {"string-prefix?", strIsStringPrefix},
        {"string-suffix?", strIsStringSuffix},
        {"vector-8b-ref", strVector8bRef},
};

const ::std::unordered_map<::std::string,
                           ::std::function<ValueP(ValueP, ValueP, EnvironmentP)>> FoldOps {
        {"+", numericBinopPlus},
        {"-", numericBinopMinus},
        {"*", numericBinopTimes},
        {"/", numericBinopDivide},
        {"mod", numericBinopMod},
        {"quotient", numericBinopQuot},
        {"remainder", numericBinopRem},
        {"&&", boolBoolBinopAnd},
        {"||", boolBoolBinopOr},
        {"string-append", strStringAppend}
};

const ::std::unordered_map<::std::string,
                           ::std::function<ValueP(ValueP, ValueP, ValueP, EnvironmentP)>> TernaryOps {
        {"substring", strSubstring},
        {"string-set!", strStringSetBang},
        {"string-replace", strStringReplace},
        {"string-replace!", strStringReplaceBang},
};
```

#### `load` 函数

```scheme
(load filename)
```

这是一个比较特殊的函数，可以导入已经写好的 Emehcs/Scheme 源代码。

比如在 REPL 环境中直接运行测试代码：

```
>>> (load "string.scm")
Test (assert (make-string 1 #\a) "a")
Test (assert (make-string 3 #\a) "aaa")
Test (assert (string) "")
Test (assert (string #\a #\b #\c) "abc")
Test (assert (list->string '(#\a #\b #\c)) "abc")
...
All test cases were correct and finished.
eval[0]: #t
>>> %
```

### 文件头

暂时只支持一个特殊的文件头，`*test`，可以写成 `* test`。放在 Emehcs/Scheme 源代码的第一行才能识别。配合 `assert` 函数使用，具体可以参考 `string.scm` 提供的样例。

### 结语

好了，你现在已经完全学会怎么使用 Emehcs 了，那么尝试用它帮我写一些测试代码吧！可以使用 GitHub 的 Pull Request 功能提交新的 Emehcs/Scheme 测试脚本帮助我完善我的 Emehcs 程序。
