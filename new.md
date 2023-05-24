# 欢迎使用 TeamNote 编辑阅读器
 
------
 
在学习工作中我们常常需要更便捷更高效的工具记录思想，整理笔记、知识，并将其中承载的价值传播给他人，**TeamNote** 是我们给出的答案 —— 我们为记录思想和分享知识提供更专业的工具。<br> 
您可以使用 TeamNote：
 
> * 整理知识，学习笔记
> * 编写简历、演示文稿
> * 发布日记，杂文，所见所想
> * 撰写发布技术文稿（代码支持）
 
 
------
 
## 什么是 Markdown
 
Markdown 是一种方便记忆、书写的纯文本标记语言，用户可以使用这些标记符号以最小的输入代价生成极富表现力的文档：譬如您正在阅读的这份文档。它使用简单的符号标记不同的标题，分割不同的段落，**粗体** 或者 *斜体* 某些文字，更棒的是，它还可以
 
### 1. 制作一份待办事宜
如何制作待办事宜，参考官方链接：[Todo 列表](https://markdown.com.cn/basic-syntax/lists.html#13-待办事宜-todo-列表)
> - [ ] 待办事项1
> - [ ] 待办事项2
> - [x] 待办事项3
> - [x] 代办事项4
 
### 2. 高亮一段代码
 
``` c++
//一段C++代码
#include <iostream>
using namespace std;
int main()
{
    cout << “Hello World！” << endl;; 
    return 0;
}
```

``` java
//一段Java代码
public class HelloWord {
    public static void main(String[] args){
        System.out.println("Hello World！");
    }
}
```

``` python
//一段Python代码
def print_seconds():
    hours = 24
    minutes = hours *60
    seconds = minutes *60
    print(seconds)
print_seconds() 

```
 
### 3. 绘制一张表格
 
| &nbsp;&nbsp;&nbsp;&nbsp;名称  &nbsp;&nbsp; &nbsp;&nbsp;     | &nbsp;&nbsp;&nbsp;&nbsp;价格 &nbsp;&nbsp; &nbsp;&nbsp; | &nbsp;&nbsp; &nbsp;&nbsp;数量 &nbsp;&nbsp; |
| --------   | -----:  | :----:  |
| &nbsp;&nbsp;&nbsp;&nbsp;计算机  &nbsp;&nbsp;   | &nbsp;&nbsp;&nbsp;&nbsp;¥1600 &nbsp;&nbsp;|  &nbsp;&nbsp; 5 &nbsp;&nbsp;    |
| &nbsp;&nbsp;&nbsp;&nbsp;手机   &nbsp;&nbsp;     |   &nbsp;&nbsp;&nbsp;&nbsp;¥12 &nbsp;&nbsp;  |  &nbsp;&nbsp; 12 &nbsp;&nbsp;  |
| &nbsp;&nbsp;&nbsp;&nbsp;平板   &nbsp;&nbsp;     |    &nbsp;&nbsp;&nbsp;&nbsp;¥1  &nbsp;&nbsp;  |  &nbsp;&nbsp;234 &nbsp;&nbsp; |

### 4. 编写一份个人简历
<br>

 <center>
     <h1>XXX</h1>
 </center>

#### 个人信息

* 性 别：男&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;&emsp;&ensp;年 龄：25  
* 手 机：134XXXX3216&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;邮 箱：XXXXX@163.com    
* 专 业：软件工程 &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;岗 位：C++工程师

#### 工作及教育经历

* 前公司&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;2019.8~至今&emsp;&emsp;&emsp;&emsp;&emsp; 事业群名字-部门名字
* XXXX大学&emsp;&emsp;&emsp;&emsp;&emsp;2017.9~2019.7&emsp;&emsp;&emsp;&emsp; 计算机科学与技术专业-研究生
* XXXX大学&emsp;&emsp;&emsp;&emsp;&emsp;2013.9~2017.7&emsp;&emsp;&emsp;&emsp; 计算机科学与技术专业-本科  

#### 专业技能

* 熟练使用 C++，掌握Python，了解 Java、JavaScript 等编程语言
* 掌握基础数据结构和算法的基本原理
* 等等

#### 项目经历

1. 公司/学校 - XXweb服务器 - 独立开发 - 202108- 202112 
    * 具体功能 
    * 运用了那些技术，技术难点是
    * 效果如何
    * demo演示地址，github地址 

2. 公司/学校 - XX游戏 - 负责后端开发 - 202209- 202212
    * 具体功能
    * 运用了那些技术，技术难点是
    * 效果如何
    * demo演示地址，github地址

#### 获奖经历

* XXX 优秀新人
* XXX 学生社团优秀干部
* 竞赛 XXX 奖

#### 个人账号

* blog 地址 (附加自己博客特色，写了哪些技术文章)
* github 地址 (附加自己github特色，突出的项目)

#### 其他信息

* 喜欢钻研技术 等等
* 性格开朗，喜欢跳舞，做个主持人 等等
<br>

### 5. 编写一份技术文档

### 展示效果
**简要描述：**

| 参数名 | 必选 | 类型 | 说明 |举例
|:--- |:--- |:--- |:---  |:---  |
| acceptNo | 是 | string |  受理号 | 6120200302000148 |

* 该接口不返回保单号 和 投保人姓名数据, 需要前端从上个页面携带

**cURL 描述：**
```sh
curl --location --request POST 'http://aaa.bbb.ccc/ddd/product/qualityInspection/recordDetail' \
--header 'Content-Type: application/json' \
--data-raw '{   
	"acceptNo": "111"}'
```

**请求方式：**
- POST

**返回参数：**

|参数名|类型|说明|
|:----  |:----- |-----   |
|resultCode  | number | 码值，200：成功；500：失败|
|resultMsg | string | 提示话术  |
|resultBody |object |  依情况而定 |
 **返回 resultBody 说明** 

|参数名|类型|说明|
|:-----  |:-----|----- 
| acceptNo | string | 受理号  | 
| inspector | string | 质检员  | 
|  inspectionDate | string | 质检日期 | 
|    status | string |  状态 | 
|    reason | string | 原因|
|   histories | array | 历史轨迹| 
| ├──  dateTime | string | 操作时间| 
| ├──  description  | string | 操作后状态 | 


 **返回示例**
```json
{
    "resultCode": 200,
    "resultMsg": "OK",
    "resultBody": {
        "acceptNo": "111",
        "inspector": "秦灵",
        "inspectionDate": "2020-01-09 06:06:16",
        "status": "不通过",
        "reason": "杨柳丝丝弄轻柔，烟缕织成愁。海棠未雨，梨花先雪，一半春休。而今往事难重省，归梦绕秦楼。相思只在，丁香枝上，豆蔻梢头。",
        "histories": [
            {
                "dateTime": "2020-12-19 10:32:06",
                "description": "质检通过"
            },
            {
                "dateTime": "2020-12-18 10:01:12",
                "description": "被抽取待人工质检"
            },
            {
                "dateTime": "2020-12-17 10:24:16",
                "description": "被抽取待人工质检"
            },
            {
                "dateTime": "2020-12-16 10:25:21",
                "description": "被抽取待人工质检"
            }
        ]
    }
}
```

 **备注**


### 6. 使用Git仓库进行文件协同
首先设置您的**仓库地址**：点击右上角的*菜单栏*->*同步*->*设置github仓库地址*<br>
仓库地址格式应使用以下格式的地址：
![](file:///home/polaris/cdc/test/mk_images/Markdown_2023_04_24_22_14_12_github.png)
而后即可将本地文件上传至远程仓库或者将远程仓库文件下载至本地。通过此功能，可进行多用户的文本协同。


### 7. 更详细语法说明
 
想要查看更详细的语法说明，可以参考我们准备的用户手册-右上角的的**菜单栏**，进阶用户可以参考[markdown官方扩展语法] https://markdown.com.cn/extended-syntax/ 了解更多高级功能。
 
总而言之，不同于其它 **所见即所得** 的编辑器：在TeamNote中，你只需使用键盘专注于书写文本内容，就可以生成印刷级的排版格式，省却在键盘和工具栏之间来回切换，调整内容和格式的麻烦。**Markdown 在流畅的书写和印刷级的阅读体验之间找到了平衡。** 目前它已经成为世界上最大的技术分享网站 GitHub 和 技术问答网站 StackOverFlow 的御用书写格式。
 
---