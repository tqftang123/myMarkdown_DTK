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