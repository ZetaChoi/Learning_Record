# 排行榜接口文档

1. 获取排行榜列表
    - 请求地址： `/rank/rankList`
    - 请求方式：`GET` `POST`
    - 请求参数：
        | 参数名 | 数据类型 | 说明 |
        |-------|----------|-----|
        |channelId| int | channelId|

    - 返回值： json
        - 基本参数
            | 参数名 | 数据类型 | 说明 |
            |-------|----------|-----|
            |state| int| 通用状态码|
            |data| json | 数组+对象|
        - data
            | 参数名 | 数据类型 | 说明 |
            |-------|----------|-----|
            | rankId | int | 榜单id |
            | channelId | int | 榜单所属渠道id |
            | rankName | String | 榜单名 |
            | rankRang | int | 排名范围(前x人) |
    - 请求示例：`GET:` `/rank/rankList?channelId=1007`
    - 返回示例：`json:` `{"state":1,"data":[{"rankId":1,"channelId":1007,"rankName":"等级榜","rankRang":20},{"rankId":2,"channelId":1007,"rankName":"消费榜","rankRang":10}]}`

2. 获取排行榜数据
    - 请求地址： `/rank/query`
    - 请求方式：`GET` `POST`
    - 请求参数：
        | 参数名 | 数据类型 | 说明 |
        |-------|----------|-----|
        | channelId | int | channelId|
        | rankId | int | 榜单id |
        | rangStart  | int | 范围起始位 |
        | rangStop | int | 范围结束位 |

    - 返回值： json
        - 基本参数
            | 参数名 | 数据类型 | 说明 |
            |-------|----------|-----|
            |state| int| 通用状态码|
            |data| json | 数组+对象|
        - data 
            | 参数名 | 数据类型 | 说明 |
            |-------|----------|-----|
            | roleId | String | 角色id |
            | score | int | 分数 |
    - 请求示例：`GET:` `/rank/query?channelId=1007&rankId=1&rangStart=0&rangStop=1`
    - 返回示例：`json:` `{"state":1,"data":[{"roleId":"role1","score":999},{"roleId":"role2","score":998}]}`

2. 提交到排行榜
    - 请求地址： `/rank/submit`
    - 请求方式：`GET` `POST`
    - 注意： roleId传游戏角色，应与提交数据接口提交的roleId一致。
    - 优化： （若已经获取排行榜最后一名记录）调用接口前与榜单最后一名分数比较，小于则不必提交，缓解服务器压力。
    - 请求参数：
        | 参数名 | 数据类型 | 说明 |
        |-------|----------|-----|
        | channelId | int | channelId|
        | rankId | int | 榜单id |
        | roleId | String | 角色id |
        | score | int | 分数 |

    - 返回值： json
        - 基本参数
            | 参数名 | 数据类型 | 说明 |
            |-------|----------|-----|
            |state| int| 通用状态码 |
            |data| String | 结果说明 |
    - 请求示例：`GET:` `/rank/submit?channelId=1007&rankId=1&roleId=role1&score=999`
    - 返回示例：`json:` `{"state":1,"data":"Success"}`
