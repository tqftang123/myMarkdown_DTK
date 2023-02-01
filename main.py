import requests
import time
import json
import pandas as pd
import pygal
from pyecharts.charts import Map
from pyecharts import options as opts
import jieba
from wordcloud import WordCloud
import matplotlib.pyplot as plt

# 请求头部信息
headers = {
    'referer': 'https://careers.tencent.com/search.html?keyword=Python',
    'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36'
}
query = {
    'timestamp': '',
    'countryId': '',
    'cityId': '',
    'bgIds': '',
    'productId': '',
    'categoryId': '',
    'parentCategoryId': '',
    'attrId': '',
    'keyword': 'c++',
    'pageIndex': '',
    'pageSize': '10',
    'language': 'zh-cn',
    'area': 'cn'
}
# 动态链接路径
raw_url = 'https://careers.tencent.com/tencentcareer/api/post/Query?timestamp={}&countryId=&cityId=&bgIds=&productId=&categoryId=&parentCategoryId=&attrId=&keyword={}&pageIndex={}&pageSize={}&language={}&area={}'
post_url = 'https://careers.tencent.com/tencentcareer/api/post/ByPostId?timestamp=%d&postId=%s&language=zh-cn'

#获取PostId列表
#由于网页中每一个职位都有一个PostId，而爬取职位的详情信息时，需要用到PostId来构造请求的URL，因此，我先将职位的PostId存放到列表中，为下一步爬取职位的详情信息做准备。
def get_url():
    # 爬取的页数，这里爬取前7页
    endIndex = 7
    postId = []
    for pageIndex in range(1, endIndex+1):
        query['timestamp'] = int(time.time())
        query['pageIndex'] = pageIndex
        url = raw_url.format(query['timestamp'], query['keyword'], query['pageIndex'], query['pageSize'],
                             query['language'], query['area'])
        try:
            r = requests.get(url, headers=headers)
        except :
            print("请求失败！")
            continue
        data_list = json.loads(r.text)['Data']['Posts']
        # print(r.text)
        for item in data_list:
            postId.append(item['PostId'])
            #print(item['PostId'])
    return postId


def get_detail(post_id):
    data_list = []
    for it in post_id:
        url = post_url % (int(time.time()), it)
        try:
            response = requests.get(url, headers)
        except :
            print("请求失败！")
            continue
        if response.status_code == 200:
            data = json.loads(response.text)['Data']
            data_list += [data]
            #print(data)
    return data_list


def save_to_file(data_list):
    df = pd.DataFrame(data_list)
    df.to_csv('tencent.csv', index=False, encoding='utf_8_sig')


def locationName_pygal (date_list):
    #   统计岗位工作地点，并计数  locationName中是
    #   所有工作的地点列表，然后将其set转换为元组去重，再转换为lsit方便后续使用
    locationName = []
    locationName_conut = []
    for i in date_list:
        locationName.append(i['LocationName'])

    # print(locationName)

    locationName_2 = list(set(locationName))
    for name in locationName_2:
        # print(name+":  "+str(locationName.count(name) ))
        locationName_conut.append(locationName.count(name))
    # print(locationName_2)
    # print(locationName_conut)

    # 绘制直方图
    hist = pygal.Bar()
    hist.title = "腾讯招聘c++工作在某地的岗位数量"
    hist.x_labels = locationName_2
    hist.x_title = "城市"
    hist.y_title = "招聘岗位数量"

    hist.add('招聘岗位数量', locationName_conut)
    hist.render_to_file('腾讯招聘c++工作在某地的岗位数量.svg')


def   dtu(date_list):
    locationName = []
    locationName_conut = []
    for i in date_list:
        locationName.append(i['LocationName'])

    # print(locationName)

    locationName_2 = list(set(locationName))
    for name in locationName_2:
        # print(name+":  "+str(locationName.count(name) ))
        locationName_conut.append(locationName.count(name))
    # print(locationName_2)
    # print(locationName_conut)

    area_data = {
        '北京': ['北京市', '朝阳区', '海淀区', '通州区', '房山区', '丰台区', '昌平区', '大兴区', '顺义区', '西城区',
                 '延庆县', '石景山区', '宣武区', '怀柔区', '崇文区', '密云县',
                 '东城区', '门头沟区', '平谷区'],
        '广东': ['广东省', '东莞市', '广州市', '中山市', '深圳市', '惠州市', '江门市', '珠海市', '汕头市', '佛山市',
                 '湛江市', '河源市', '肇庆市', '潮州市', '清远市', '韶关市', '揭阳市', '阳江市', '云浮市', '茂名市',
                 '梅州市', '汕尾市'],
        '山东': ['山东省', '济南市', '青岛市', '临沂市', '济宁市', '菏泽市', '烟台市', '泰安市', '淄博市', '潍坊市',
                 '日照市', '威海市', '滨州市', '东营市', '聊城市', '德州市', '莱芜市', '枣庄市'],
        '江苏': ['江苏省', '苏州市', '徐州市', '盐城市', '无锡市', '南京市', '南通市', '连云港市', '常州市', '扬州市',
                 '镇江市', '淮安市', '泰州市', '宿迁市'],
        '河南': ['河南省', '郑州市', '南阳市', '新乡市', '安阳市', '洛阳市', '信阳市', '平顶山市', '周口市', '商丘市',
                 '开封市', '焦作市', '驻马店市', '濮阳市', '三门峡市', '漯河市', '许昌市', '鹤壁市', '济源市'],
        '上海': ['上海市', '松江区', '宝山区', '金山区', '嘉定区', '南汇区', '青浦区', '浦东新区', '奉贤区', '闵行区',
                 '徐汇区', '静安区', '黄浦区', '普陀区', '杨浦区', '虹口区', '闸北区', '长宁区', '崇明县', '卢湾区'],
        '河北': ['河北省', '石家庄市', '唐山市', '保定市', '邯郸市', '邢台市', '河北区', '沧州市', '秦皇岛市',
                 '张家口市', '衡水市', '廊坊市', '承德市'],
        '浙江': ['浙江省', '温州市', '宁波市', '杭州市', '台州市', '嘉兴市', '金华市', '湖州市', '绍兴市', '舟山市',
                 '丽水市', '衢州市'],
        '陕西': ['陕西省', '西安市', '咸阳市', '宝鸡市', '汉中市', '渭南市', '安康市', '榆林市', '商洛市', '延安市',
                 '铜川市'],
        '湖南': ['湖南省', '长沙市', '邵阳市', '常德市', '衡阳市', '株洲市', '湘潭市', '永州市', '岳阳市', '怀化市',
                 '郴州市', '娄底市', '益阳市', '张家界市', '湘西州'],
        '重庆': ['重庆市', '江北区', '渝北区', '沙坪坝区', '九龙坡区', '万州区', '永川市', '南岸区', '酉阳县', '北碚区',
                 '涪陵区', '秀山县', '巴南区', '渝中区', '石柱县', '忠县', '合川市', '大渡口区', '开县', '长寿区',
                 '荣昌县', '云阳县', '梁平县', '潼南县', '江津市', '彭水县', '璧山县', '綦江县',
                 '大足县', '黔江区', '巫溪县', '巫山县', '垫江县', '丰都县', '武隆县', '万盛区', '铜梁县', '南川市',
                 '奉节县', '双桥区', '城口县'],
        '福建': ['福建省', '漳州市', '泉州市', '厦门市', '福州市', '莆田市', '宁德市', '三明市', '南平市', '龙岩市'],
        '天津': ['天津市', '和平区', '北辰区', '河北区', '河西区', '西青区', '津南区', '东丽区', '武清区', '宝坻区',
                 '红桥区', '大港区', '汉沽区', '静海县', '宁河县', '塘沽区', '蓟县', '南开区', '河东区'],
        '云南': ['云南省', '昆明市', '红河州', '大理州', '文山州', '德宏州', '曲靖市', '昭通市', '楚雄州', '保山市',
                 '玉溪市', '丽江地区', '临沧地区', '思茅地区', '西双版纳州', '怒江州', '迪庆州'],
        '四川': ['四川省', '成都市', '绵阳市', '广元市', '达州市', '南充市', '德阳市', '广安市', '阿坝州', '巴中市',
                 '遂宁市', '内江市', '凉山州', '攀枝花市', '乐山市', '自贡市', '泸州市', '雅安市', '宜宾市', '资阳市',
                 '眉山市', '甘孜州'],
        '广西': ['广西壮族自治区', '贵港市', '玉林市', '北海市', '南宁市', '柳州市', '桂林市', '梧州市', '钦州市',
                 '来宾市', '河池市', '百色市', '贺州市', '崇左市', '防城港市'],
        '安徽': ['安徽省', '芜湖市', '合肥市', '六安市', '宿州市', '阜阳市', '安庆市', '马鞍山市', '蚌埠市', '淮北市',
                 '淮南市', '宣城市', '黄山市', '铜陵市', '亳州市', '池州市', '巢湖市', '滁州市'],
        '海南': ['海南省', '三亚市', '海口市', '琼海市', '文昌市', '东方市', '昌江县', '陵水县', '乐东县', '五指山市',
                 '保亭县', '澄迈县', '万宁市', '儋州市', '临高县', '白沙县', '定安县', '琼中县', '屯昌县'],
        '江西': ['江西省', '南昌市', '赣州市', '上饶市', '吉安市', '九江市', '新余市', '抚州市', '宜春市', '景德镇市',
                 '萍乡市', '鹰潭市'],
        '湖北': ['湖北省', '武汉市', '宜昌市', '襄樊市', '荆州市', '恩施州', '孝感市', '黄冈市', '十堰市', '咸宁市',
                 '黄石市', '仙桃市', '随州市', '天门市', '荆门市', '潜江市', '鄂州市', '神农架林区'],
        '山西': ['山西省', '太原市', '大同市', '运城市', '长治市', '晋城市', '忻州市', '临汾市', '吕梁市', '晋中市',
                 '阳泉市', '朔州市'],
        '辽宁': ['辽宁省', '大连市', '沈阳市', '丹东市', '辽阳市', '葫芦岛市', '锦州市', '朝阳市', '营口市', '鞍山市',
                 '抚顺市', '阜新市', '本溪市', '盘锦市', '铁岭市'],
        '台湾': ['台湾省', '台北市', '高雄市', '台中市', '新竹市', '基隆市', '台南市', '嘉义市'],
        '黑龙江': ['黑龙江', '齐齐哈尔市', '哈尔滨市', '大庆市', '佳木斯市', '双鸭山市', '牡丹江市', '鸡西市', '黑河市',
                   '绥化市', '鹤岗市', '伊春市', '大兴安岭地区', '七台河市'],
        '内蒙古': ['内蒙古自治区', '赤峰市', '包头市', '通辽市', '呼和浩特市', '乌海市', '鄂尔多斯市', '呼伦贝尔市',
                   '兴安盟', '巴彦淖尔盟', '乌兰察布盟', '锡林郭勒盟', '阿拉善盟'],
        '香港': ["香港", "香港特别行政区"],
        '澳门': ['澳门', '澳门特别行政区'],
        '贵州': ['贵州省', '贵阳市', '黔东南州', '黔南州', '遵义市', '黔西南州', '毕节地区', '铜仁地区', '安顺市',
                 '六盘水市'],
        '甘肃': ['甘肃省', '兰州市', '天水市', '庆阳市', '武威市', '酒泉市', '张掖市', '陇南地区', '白银市', '定西地区',
                 '平凉市', '嘉峪关市', '临夏回族自治州', '金昌市', '甘南州'],
        '青海': ['青海省', '西宁市', '海西州', '海东地区', '海北州', '果洛州', '玉树州', '黄南藏族自治州'],
        '新疆': ['新疆', '新疆维吾尔自治区', '乌鲁木齐市', '伊犁州', '昌吉州', '石河子市', '哈密地区', '阿克苏地区',
                 '巴音郭楞州', '喀什地区', '塔城地区', '克拉玛依市', '和田地区', '阿勒泰州', '吐鲁番地区', '阿拉尔市',
                 '博尔塔拉州', '五家渠市',
                 '克孜勒苏州', '图木舒克市'],
        '西藏': ['西藏区', '拉萨市', '山南地区', '林芝地区', '日喀则地区', '阿里地区', '昌都地区', '那曲地区'],
        '吉林': ['吉林省', '吉林市', '长春市', '白山市', '白城市', '延边州', '松原市', '辽源市', '通化市', '四平市'],
        '宁夏': ['宁夏回族自治区', '银川市', '吴忠市', '中卫市', '石嘴山市', '固原市']
    }

    # for k, v in area_data.items():
    #     for i in v:
    #         if '韶关' in i:
    #             print(k)

    def chaozhao(ct):
        for k, v in area_data.items():
            for i in v:
                if ct in i:
                    return k

    ct1 = locationName_2
    ct2_conut = locationName_conut
    ct2 = []

    for i in ct1:
        ct2.append(chaozhao(i))
    #print(ct2)
    ct3 = list(set(ct2))
    ct3_conut = [0] * len(ct3)
    #print(ct3)
    i = 0
    while i < len(ct2):
        j = 0
        while j < len(ct3):
            if ct3[j] == ct2[i]:
                ct3_conut[j] = ct3_conut[j] + ct2_conut[i]
            j = j + 1
        i = i + 1
    #print(ct3_conut)

    province_list = [list(z) for z in zip(ct3, ct3_conut)]
    # 将省份和数量输出
    #print(province_list)

    #
    c = (
        Map(init_opts=opts.InitOpts(width="1000px", height="600px"))  # 可切换主题
        .set_global_opts(
            title_opts=opts.TitleOpts(title="腾讯招聘c++岗位在全国的分布情况"),
            visualmap_opts=opts.VisualMapOpts(
                min_=0,
                max_=30,
                range_text=['岗位数量区间:', ''],  # 分区间
                is_piecewise=True,  # 定义图例为分段型，默认为连续的图例
                pos_top="middle",  # 分段位置
                pos_left="left",
                orient="vertical",
                split_number=5  # 分成10个区间
            )

        )
        .add("腾讯招聘c++岗位在全国的分布情况", province_list, maptype="china")
        .render("腾讯招聘c++岗位在全国的分布情况.html")
    )



def ciYun(data_list):
    recruitPostName_1 = []
    for recruitPostName in date_list:
        recruitPostName_1.append(recruitPostName['RecruitPostName'])

    f = open("recruitPostName.txt", "w", encoding="utf-8")
    for line in recruitPostName_1:
        f.write(line + '\n')
    f.close()

    responsibility_1 = []
    for responsibility in date_list:
        responsibility_1.append(responsibility['Responsibility'])

    f = open("responsibility.txt", "w", encoding="utf-8")
    for line in responsibility_1:
        f.write(line + '\n')
    f.close()

    requirement_1 = []
    for requirement in date_list:
        requirement_1.append(requirement['Requirement'])

    f = open("requirement.txt", "w", encoding="utf-8")
    for line in requirement_1:
        f.write(line + '\n')
    f.close()

    ciYun_1('recruitPostName')
    ciYun_1('responsibility')
    ciYun_1('requirement')



def ciYun_1(str_name):
    # 导入文本数据并进行简单的文本处理
    # 去掉换行符和空格以及标点符号

    str_name_1=str_name+".txt"
    str_name_2=str_name+".jpg"
    text = open(str_name_1, encoding="utf-8").read()
    text = text.replace('\n', "").replace("，", "").replace("。", "").replace("、", "").replace("；", "").replace("！",
                                                                                                              "").replace(
        "：", "")
    # 分词，返回结果为词的列表
    text_cut = jieba.lcut(text)
    # 停用词库
    stop_words = open("Stop Words.txt", encoding="utf8").read().split("\n")

    # 将分好的词用某个符号分割开连成字符串
    text_cut = ' '.join(text_cut)
    # print(text_cut)

    # 绘制词云
    word_cloud = WordCloud(font_path="simsun.ttc",
                           background_color="white",
                           stopwords=stop_words
                           )
    word_cloud.generate(text_cut)

    # 运用matplotlib展现结果
    plt.subplots(figsize=(12, 8))
    plt.imshow(word_cloud)
    plt.axis("off")
    # plt.show()
    plt.savefig(str_name_2)


if __name__ == '__main__':
    postId = get_url()
    date_list=get_detail(postId)
    save_to_file(date_list)
    locationName_pygal(date_list)
    dtu(date_list)
    ciYun(date_list)
    #print(date_list)














