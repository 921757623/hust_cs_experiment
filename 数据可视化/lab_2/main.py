from pyecharts import options as opts
from pyecharts.charts import Graph, WordCloud, Bar, Pie, Page, Line
import json

import requests

import pandas as pd
from pyecharts.commons.utils import JsCode


def relationship():
    json_content = json.load(open('weibo.json', 'r', encoding='utf-8'))
    # 创建节点
    nodes = [opts.GraphNode(
        name=_node['name'],
        symbol_size=_node['symbolSize'],
        value=_node['value'],
        category=_node['category']
    ) for _node in json_content[0]]
    # 创建节点边关系
    links = [opts.GraphLink(
        source=_link['source'],
        target=_link['target']
    ) for _link in json_content[1]]

    categories = [opts.GraphCategory(name=category)
                  for category in {_node['category'] for _node in json_content[0]}]

    graph = (
        Graph(init_opts=opts.InitOpts(width='1260px', height='560px'))
        .add("", nodes, links, categories=categories, repulsion=50,
             layout='circular', is_rotate_label=True,
             linestyle_opts=opts.LineStyleOpts(curve=0.2))  # 图显示的大小（两结点间的距离）
        .set_global_opts(legend_opts=opts.LegendOpts(is_show=False),
                         title_opts=opts.TitleOpts(title="Graph-微博转发关系图"))
    )
    graph.render('和弦关系图.html')
    graph = (
        Graph(init_opts=opts.InitOpts(width='1260px', height='560px'))
        .add("", nodes, links, categories=categories, repulsion=50,
             linestyle_opts=opts.LineStyleOpts(curve=0.2))  # 图显示的大小（两结点间的距离）
        .set_global_opts(legend_opts=opts.LegendOpts(is_show=False),
                         title_opts=opts.TitleOpts(title="Graph-微博转发关系图"))
    )
    graph.render('关系图.html')


def wordCloud():
    header = {
        'Accept': 'application / json, text / plain, * / *',
        'Accept-Encoding': 'gzip, deflate, br',
        'Accept-Language': 'zh-CN,zh;q=0.9,en;q=0.8',
        'Connection': 'keep-alive',
        'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) '
                      'Chrome/99.0.4844.51 Safari/537.36'
    }
    BASE_URL = 'https://top.baidu.com/api/board?platform=wise&tab=realtime&tag=%7B%7D'
    resp = requests.get(BASE_URL, headers=header)

    content = json.loads(resp.text)['data']['cards'][0]['content']
    titles = [item['query'] for item in content][:20]

    words = [(title, len(titles) - index) for index, title in enumerate(titles)]

    wordcloud = (WordCloud(init_opts=opts.InitOpts(width='1260px', height='560px'))
                 .add("", words, word_size_range=[3, 28])
                 .set_global_opts(title_opts=opts.TitleOpts(title="WordCloud-百度热搜榜前20")))
    wordcloud.render('百度热搜.html')


def drawBar1(sheet):
    subjects = ['英语', '体育', '军训', '数分', '高代', '解几']

    bar = Bar(init_opts=opts.InitOpts(width='1260px', height='560px')) \
        .add_xaxis(sheet['姓名'].to_list())
    for subject in subjects:
        bar.add_yaxis(subject, [score for score in sheet[subject]], stack='same')
    bar.set_series_opts(
        label_opts=opts.LabelOpts(
            position="inside",
            formatter=JsCode("""
                function(x) {
                    if (x.data > 0) return x.data;
                    else if (x.data == -50) return '未知';
                    else if (x.data == -51) return '作弊';
                    else return '缺考';
                }
            """)
        )
    ).set_global_opts(
        title_opts={'text': '学生总分'},
        xaxis_opts=opts.AxisOpts(name="姓名"),
        yaxis_opts=opts.AxisOpts(name="分数")
    )
    bar.render('总分——堆叠.html')


def drawBar2(sheet):
    subjects = ['英语', '体育', '军训', '数分', '高代', '解几']
    sheet[sheet[subjects] < 0] = 0
    aa = [int(sheet.iloc[i, 4:].sum()) for i in range(sheet.shape[0])]
    bar = (
        Bar(init_opts=opts.InitOpts(width='1260px', height='560px'))
        .add_xaxis(sheet['姓名'].to_list())
        .add_yaxis('总分', aa)
        .set_global_opts(
            title_opts={'text': '学生总分'},
            xaxis_opts=opts.AxisOpts(name="姓名"),
            yaxis_opts=opts.AxisOpts(name="分数")
        )
    )

    bar.render('总分——不堆叠.html')


def drawPie(sheet):
    subjects = ['英语', '体育', '军训', '数分', '高代', '解几']
    total_sorted_top_three = sorted(
        enumerate(sheet[subjects].sum(axis=1)),
        key=lambda x: x[1],
        reverse=True
    )[:3]
    page = Page()
    for i in range(3):
        scores = sheet[subjects].iloc[total_sorted_top_three[i][0],].to_list()
        data = [opts.PieItem(name=subject, value=score) for subject, score in zip(subjects, scores)]
        pie = (
            Pie(init_opts=opts.InitOpts(width='1260px', height='560px')).add("", data)
            .set_series_opts(label_opts=opts.LabelOpts(position='inside', formatter='{b}:{c}'))
            .set_global_opts(
                title_opts={"text": "学生分数构成", "subtext": f"第{i + 1}名 总分：{total_sorted_top_three[i][1]}"})
        )
        page.add(pie)
    page.render("学生前三成绩构成.html")


def drawLine(sheet):
    subjects = ['英语', '数分', '高代', '解几']
    line = (
        Line(init_opts=opts.InitOpts(width='1260px', height='560px'))
        .add_xaxis(list(range(0, 101, 10)))
        .set_global_opts(
            title_opts=opts.TitleOpts(title="成绩分布图"),
            legend_opts=opts.LegendOpts(is_show=True)
        )
    )
    for subject in subjects:
        data = [0] * 11
        for score in sheet[subject]:
            data[score // 10] += 1
        line.add_yaxis(subject, [opts.LineItem(name="人数", value=num) for num in data])
    line.render("四科分布折线图.html")


def drawAverage(sheet):
    subjects = ['英语', '体育', '军训', '数分', '高代', '解几']
    male = []
    female = []
    for index, gender in enumerate(sheet['性别']):
        if gender == '男':
            male.append(index)
        else:
            female.append(index)

    subject_ave_male = []
    subject_ave_female = []
    for subject in subjects:
        subject_ave_male.append(round(sheet[subject][male].mean(), 2))
        subject_ave_female.append(round(sheet[subject][female].mean(), 2))

    bar = (
        Bar(init_opts=opts.InitOpts(width='1260px', height='560px'))
        .add_xaxis(subjects)
        .add_yaxis('男生', subject_ave_male)
        .add_yaxis('女生', subject_ave_female)
        .set_global_opts(
            title_opts={'text': '男女各科平均分对比'},
            xaxis_opts=opts.AxisOpts(name="科目"),
            yaxis_opts=opts.AxisOpts(name="分数")
        )
    )

    bar.render('男女各科平均成绩.html')


def draw():
    situation = {'未知': -50, '作弊': -51, '缺考': -52}
    # 处理异常的数据
    s = pd.read_excel(pd.ExcelFile('student.xls'), sheet_name='Sheet1') \
        .fillna(situation['未知']).replace(situation)

    drawBar1(s)
    drawBar2(s)
    drawPie(s)
    drawLine(s)
    drawAverage(s)


if __name__ == '__main__':
    relationship()
    wordCloud()
    draw()
