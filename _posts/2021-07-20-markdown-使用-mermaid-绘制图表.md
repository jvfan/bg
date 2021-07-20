---
layout:     post
title:      markdown 使用 mermaid 绘制图表
subtitle:   介绍一种易上手的 markdown 图表
date:       2021-07-20
author:     Jerry Chen
header-img: img/post-bg-debug.png
catalog: true
tags:
    - markdown
    - mermaid

---

> 文章中出现的图表代码在 web 页面中可能无法正常显示为图表，要查看效果请拷贝图表代码内容到 Windows Typora 应用程序中的代码框，并选择语言为 mermaid；

### mermaid 介绍

[官网](https://mermaid-js.github.io/) 

mermaid 是一个基于 Javascript 的图表工具，可以使用特定 markdown 标记动态创建和修改图表；

### 支持的图表

#### 饼图

以下是一个简单饼图代码：

```
pie title What Voldemort doesn't have?
         "FRIENDS" : 2
         "FAMILY" : 3
         "NOSE" : 45
```

显示效果如下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210720013853.png)

#### 序列图

以下是一个简单序列图代码：

```
sequenceDiagram
    Alice ->> Bob: Hello Bob, how are you?
    Bob-->>John: How about you John?
    Bob--x Alice: I am good thanks!
    Bob-x John: I am good thanks!
    Note right of John: Bob thinks a long<br/>long time, so long<br/>that the text does<br/>not fit on a row.

    Bob-->Alice: Checking with John...
    Alice->John: Yes... John, how are you?
```

显示效果如下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210720013930.png)

#### 流程图

以下是一个简单流程图代码：

> `graph TD;` 表示从上到下；
>
> `graph LR;` 表示从左到右；
>
> `graph RL;` 表示从右到左；

```
graph LR
    A[Square Rect] -- Link text --> B((Circle))
    A --> C(Round Rect)
    B --> D{Rhombus}
    C --> D
```

显示效果如下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210720014001.png)

#### 类图

以下是一个简单类图代码：

```
classDiagram
Class01 <|-- AveryLongClass : Cool
Class03 *-- Class04
Class05 o-- Class06
Class07 .. Class08
Class09 --> C2 : Where am i?
Class09 --* C3
Class09 --|> Class07
Class07 : equals()
Class07 : Object[] elementData
Class01 : size()
Class01 : int chimp
Class01 : int gorilla
Class08 <--> C2: Cool label
```

显示效果如下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210720014025.png)

#### 状态图

以下是一个简单状态图代码：

```
stateDiagram-v2
    [*] --> Still
    Still --> [*]

    Still --> Moving
    Moving --> Still
    Moving --> Crash
    Crash --> [*]
```

显示效果如下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210720014045.png)

#### 实体关系图

以下是一个简单实体关系图代码：

```
erDiagram
    CUSTOMER ||--o{ ORDER : places
    ORDER ||--|{ LINE-ITEM : contains
    CUSTOMER }|..|{ DELIVERY-ADDRESS : uses
```

显示效果如下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210720014109.png)

#### 用户旅程图

以下是一个简单用户旅程图代码：

```
journey
    title My working day
    section Go to work
      Make tea: 5: Me
      Go upstairs: 3: Me
      Do work: 1: Me, Cat
    section Go home
      Go downstairs: 5: Me
      Sit down: 5: Me
```

显示效果如下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210720014134.png)

#### 甘特图

以下是一个简单甘特图代码：

```
gantt
dateFormat  YYYY-MM-DD
title Adding GANTT diagram to mermaid
excludes weekdays 2014-01-10

section A section
Completed task            :done,    des1, 2014-01-06,2014-01-08
Active task               :active,  des2, 2014-01-09, 3d
Future task               :         des3, after des2, 5d
Future task2              :         des4, after des3, 5d
```

显示效果如下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210720014152.png)

#### Git 图

以下是一个简单 Git 图代码：

```
gitGraph:
options
{
    "nodeSpacing": 150,
    "nodeRadius": 10
}
end
commit
branch newbranch
checkout newbranch
commit
commit
checkout master
commit
commit
merge newbranch
```

显示效果如下：

![](https://raw.githubusercontent.com/jvfan/jvfan.github.io/master/img/post_img/20210720014211.png)

