import random

from pyecharts.charts import Page, Line, Bar, Map, Sunburst, ThemeRiver, Boxplot, Pie
from pyecharts import options as opts
from pyecharts.commons.utils import JsCode
import pandas as pd
from datetime import datetime


def add_jump(char_id, provinces):
    jsCode = '''
    <script>
        chart_''' + char_id + '''.on('click', function (param){
            var selected = param.name;
                if (selected) {
                    switch(selected){'''
    for province in provinces:
        jsCode = jsCode + f'''
                        case '{province}':
                            location.href = "./provs/{province}.html";
                            break;'''
    jsCode = jsCode + '''
                        default:
                            break;
                    }
                }
            });
    </script>    
    '''
    return jsCode


def format_column():
    _sheet = pd.read_excel(file, '销售订单表')
    # 处理不规整的数据，如'2019#3#11'
    for i, date in enumerate(_sheet['订单日期']):
        if isinstance(date, str):
            aa = date.split('#')
            _sheet.iloc[i, 1] = datetime(int(aa[0]), int(aa[1]), int(aa[2]))
    return _sheet


def drawLine():
    title = '2019年化妆品销售总览'
    time_sort_group = sheet.sort_values(by='订单日期').groupby('订单日期')

    x_data = [date.strftime('%m-%d') for date in time_sort_group['订单日期'].groups.keys()]
    y_data = list(time_sort_group['金额'].sum())

    line1 = (
        Line(opts.InitOpts(width=pic_width, height=pic_height))
        .set_global_opts(
            title_opts=opts.TitleOpts(title=title, subtitle='每日销售情况图'),
            datazoom_opts=opts.DataZoomOpts(type_='inside', range_start=0, range_end=100),
            xaxis_opts=opts.AxisOpts(name='时间'),
            yaxis_opts=opts.AxisOpts(name='金额/元')
        )
        .add_xaxis(x_data)
        .add_yaxis('单天订单金额', y_data, is_symbol_show=False)
    )

    month_money = {}
    month_count = {}
    for date in time_sort_group['订单日期'].groups.keys():
        if month_money.get(date.month) is None:
            month_money[date.month] = 0
            month_count[date.month] = 0
        for i in time_sort_group['订单日期'].groups[date]:
            month_money[date.month] += sheet['金额'].iloc[i]
            month_count[date.month] += sheet['订购数量'].iloc[i]

    bar = (
        Bar(opts.InitOpts(width=pic_width, height=pic_height))
        .add_xaxis(list(month_count.keys()))
        .add_yaxis('月度销售总量', [int(v) for v in month_count.values()], z_level=1)
        .set_global_opts(
            title_opts=opts.TitleOpts(title=title, subtitle='月度销售情况图'),
            xaxis_opts=opts.AxisOpts(name='月份', type_="category", name_location='middle', name_gap=25),
            yaxis_opts=opts.AxisOpts(name='件', max_=6000000),
            tooltip_opts=opts.TooltipOpts(is_show=True, trigger="axis", axis_pointer_type="cross"),
        )
        .extend_axis(yaxis=opts.AxisOpts(name='金额/万元', max_=90000))
    )

    line2 = (
        Line(opts.InitOpts(width=pic_width, height=pic_height))
        .add_xaxis(list(month_count.keys()))
        .add_yaxis('月订单金额', [opts.LineItem(value=round(v / 10000, 2)) for v in month_money.values()],
                   yaxis_index=1, z_level=2)
    )
    bar.overlap(line2)

    top = 15
    sale_cnt = [(i + 1, cnt) for i, cnt in enumerate(sheet.groupby('商品编号')['订购数量'].sum())]
    sale_cnt.sort(key=lambda x: x[1], reverse=True)
    sheet_info = pd.read_excel(file, sheet_name='商品信息表')
    prices = sheet_info['销售单价']
    top_10 = sale_cnt[:top]
    type = {}
    for i_, (id_, sale_cnt) in enumerate(top_10):
        index = info['start'][1]
        for i in index:
            if id_ in info[i][0]:
                top_10[i_] = (f'X{id_ + 1:03d}', int(prices.iloc[id_]), i)
                type[i] = [0] * top
                break

    bar1 = (
        Bar(opts.InitOpts(width=pic_width, height=pic_height))
        .add_xaxis([id_ for id_, _, _ in top_10])
        .set_global_opts(
            title_opts=opts.TitleOpts(title=f'最受欢迎的前{top}位化妆品'),
            xaxis_opts=opts.AxisOpts(name='商品编号', type_="category", name_location='middle', name_gap=25),
            yaxis_opts=opts.AxisOpts(name='定价', position='left'),
            tooltip_opts=opts.TooltipOpts(is_show=True, trigger="axis", axis_pointer_type="cross"),
        )
    )

    for i_, (_, cnt, t_) in enumerate(top_10):
        type[t_][i_] = cnt
    for k, v in type.items():
        bar1.add_yaxis(k, v, stack=1, label_opts=opts.LabelOpts(formatter=JsCode("""
            function(x) {
                    if (x.data != 0) return x.data;
                    else return '';
                }
        """)))

    page = (
            Page()
            .add(line1)
            .add(bar)
            .add(bar1)
        )

    page.render(f'{charts_path}{title}.html')
    print(f'{title} 生成成功')


def drawProvince():
    for province, data in sheet.groupby('所在省份'):
        title = province

        if province == '内蒙古自治区' or province == '黑龙江省':
            province = province[:3]
        else:
            province = province[:2]

        cities_data = data.groupby('所在地市')
        consumer_sum = cities_data['金额'].sum()
        consumer_cnt = data.groupby('所在地市').count()['客户编码']

        prov1 = (
            Map(opts.InitOpts(width=pic_width, height=pic_height))
            .add(
                f'{title}各城市消费金额分布图',
                [i for i in zip(cities_data.groups, consumer_sum)],
                province,
                is_map_symbol_show=False
            )
            .set_global_opts(
                title_opts=opts.TitleOpts(title=f'{title}状况图'),
                legend_opts=opts.LegendOpts(pos_left="20%"),
                visualmap_opts=opts.VisualMapOpts(
                    is_calculable=True,
                    dimension=0,
                    pos_left="30",
                    pos_top="center",
                    range_text=["High", "Low"],
                    range_color=["lightskyblue", "yellow", "orangered"],
                    textstyle_opts=opts.TextStyleOpts(color="#ddd"),
                    min_=min(consumer_sum),
                    max_=max(consumer_sum),
                )
            )
        )

        prov2 = (
            Map(opts.InitOpts(width=pic_width, height=pic_height))
            .add(
                f'{title}各城市消费订单数分布图',
                [i for i in zip(cities_data.groups, consumer_cnt)],
                province,
                is_map_symbol_show=False,
                label_opts=opts.LabelOpts(position='top')
            )
            .set_global_opts(
                title_opts=opts.TitleOpts(title=f'{title}状况图', pos_right="5%"),
                legend_opts=opts.LegendOpts(pos_right="20%"),
                visualmap_opts=opts.VisualMapOpts(
                    is_calculable=True,
                    dimension=0,
                    pos_left="30",
                    pos_top="center",
                    range_text=["High", "Low"],
                    range_color=["lightskyblue", "yellow", "orangered"],
                    textstyle_opts=opts.TextStyleOpts(color="#ddd"),
                    min_=min(consumer_cnt),
                    max_=max(consumer_cnt),
                )
            )
        )

        bar = (
            Bar(opts.InitOpts(width=pic_width, height=pic_height))
            .add_xaxis(list(cities_data.groups))
            .add_yaxis('消费总金额', list(consumer_sum), stack='1')
            .add_yaxis('消费总订单数', list(consumer_cnt), stack='2')
            .add_yaxis('单均消费', [round(ave, 2) for ave in (consumer_sum / consumer_cnt)], stack='3')
            .set_global_opts(
                title_opts=opts.TitleOpts(title=f'{title}状况图'),
                legend_opts=opts.LegendOpts(selected_mode='single'),
                xaxis_opts=opts.AxisOpts(name='城市')
            )
        )

        time_sort_group = data.sort_values(by='订单日期').groupby('订单日期')

        month_money = {}
        month_count = {}
        for date in time_sort_group['订单日期'].groups.keys():
            if month_money.get(date.month) is None:
                month_money[date.month] = 0
                month_count[date.month] = 0
            for i in time_sort_group['订单日期'].groups[date]:
                month_money[date.month] += sheet['金额'].iloc[i]
                month_count[date.month] += sheet['订购数量'].iloc[i]

        bar1 = (
            Bar(opts.InitOpts(width=pic_width, height=pic_height))
            .add_xaxis(list(month_count.keys()))
            .add_yaxis('月度销售总量', [int(v) for v in month_count.values()], z_level=1)
            .set_global_opts(
                title_opts=opts.TitleOpts(title=title, subtitle='月度销售情况图'),
                xaxis_opts=opts.AxisOpts(name='月份', type_="category", name_location='middle', name_gap=25),
                yaxis_opts=opts.AxisOpts(name='件'),
                tooltip_opts=opts.TooltipOpts(is_show=True, trigger="axis", axis_pointer_type="cross"),
            )
            .extend_axis(yaxis=opts.AxisOpts(name='金额/万元'))
        )

        line2 = (
            Line(opts.InitOpts(width=pic_width, height=pic_height))
            .add_xaxis(list(month_count.keys()))
            .add_yaxis('月订单金额', [opts.LineItem(value=round(v / 10000, 2)) for v in month_money.values()],
                       yaxis_index=1, z_level=2)
        )
        bar1.overlap(line2)

        page = (
            Page()
            .add(prov1)
            .add(prov2)
            .add(bar)
            .add(bar1)
        )
        page.render(f'{charts_path}/provs/{title}.html')


def drawMap():
    title = '销售额地理区间分布图'

    client_prov_cnt = sheet.groupby('所在省份')['金额'].sum() / 10000
    province_group = list(sheet.groupby('所在省份').groups)

    geo = (
        Map(opts.InitOpts(width=pic_width, height=pic_height))
        .add(
            "各省销售金额分布",
            [i for i in zip(province_group, client_prov_cnt)],
            zoom=2,
            center=[109.5, 34.5]
        )
        .set_global_opts(
            title_opts=opts.TitleOpts(title=title),
            visualmap_opts=opts.VisualMapOpts(
                is_calculable=True,
                dimension=0,
                pos_left="30",
                pos_top="center",
                range_text=["High", "Low"],
                range_color=["lightskyblue", "yellow", "orangered"],
                textstyle_opts=opts.TextStyleOpts(color="#ddd"),
                min_=min(client_prov_cnt),
                max_=max(client_prov_cnt),
            ),
            tooltip_opts=opts.TooltipOpts(
                trigger="item", formatter="{b}<br/>{c} 万元"
            ),
        )
    )

    geo.render(f'{charts_path}{title}.html')
    with open(f'{charts_path}{title}.html', 'a', encoding="utf-8") as f:
        f.write(add_jump(geo.chart_id, province_group))

    print(f'{title} 生成成功')


def get_types():
    sheet_info = pd.read_excel(file, sheet_name='商品信息表')
    _info_dict = {'start': ([], [])}
    j = 0

    for key, value in sheet_info.groupby('商品大类').groups.items():
        _info_dict['start'][0].append(key)
        _info_dict[key] = ([], j)
        j += 1

    for key, value in sheet_info.groupby('商品小类').groups.items():
        _info_dict['start'][1].append(key)
        _info_dict[key] = (frozenset(value), j)

        for parent_key, parent_value in sheet_info.groupby('商品大类').groups.items():
            if value[0] in parent_value:
                _info_dict[parent_key][0].append(key)
        j += 1
    return _info_dict


def drawTypeSunburst():
    title = '各类型商品销售情况图'
    colors = [['#A3D65C', '#A4DE7C', '#ABE8A1', '#BBE8A1', '#9AE189', '#9AE495'],
              ['#9279D2', '#8888D7', '#85A9E0', '#89C2E1', '#B6B6ED', '#9DAFE7', '#99B4E6', '#A1A7E8'],
              ['#85E0A3', '#8DDBE2', '#8591E0', '#E495C7', '#EAE4A9', '#E6F2CA', '#91E3D3', '#B1ECEA', '#A1DBE8',
               '#9E99E6', '#8DE2B3', '#D77460']]

    sale_sum = list(sheet.groupby('商品编号')['订购数量'].sum())

    grandpa_list = []
    for index, key in enumerate(info['start'][0]):
        father_list = []
        father_total = 0
        for index1, father in enumerate(info[key][0]):
            child_list = []
            child_total = 0
            for child in list(info[father][0]):
                child_list.append(opts.SunburstItem(name=f'X{child + 1:03d}', value=sale_sum[child],
                                                    itemstyle_opts=opts.ItemStyleOpts(color=random.choice(colors[2]))))
                child_total += sale_sum[child]
            father_list.append(opts.SunburstItem(name=father, value=child_total, children=child_list,
                                                 itemstyle_opts=opts.ItemStyleOpts(color=colors[index][index1 + 1])))
            father_total += child_total
        grandpa_list.append(opts.SunburstItem(name=key, value=father_total, children=father_list,
                                              itemstyle_opts=opts.ItemStyleOpts(color=colors[index][0])))

    (
        Sunburst(opts.InitOpts(width=pic_width, height='800px'))
        .add('各类化妆品销售情况', grandpa_list, radius=[0, '100%'],
             levels=[
                 {},
                 {
                     "r0": "15%",
                     "r": "35%",
                     "itemStyle": {"borderWidth": 2},
                     "label": {"rotate": "tangential"},
                 },
                 {"r0": "35%", "r": "70%", "label": {"align": "right"}},
                 {
                     "r0": "70%",
                     "r": "72%",
                     "label": {"position": "outside", "padding": 3, "silent": False},
                     "itemStyle": {"borderWidth": 3},
                 },
             ]
             )
        .set_global_opts(
            title_opts=opts.TitleOpts(title=title, subtitle='销售对比旭日图'),
            legend_opts=opts.LegendOpts(is_show=False)
        )
        .render(f'{charts_path}{title}.html')
    )
    print(f'{title} 生成成功')


def drawTypeRiver():
    title = '同种类化妆品销售量比对'

    time_sort_group = sheet.sort_values(by='订单日期').groupby('订单日期')
    data = {date.strftime('%Y/%m'): [0] * len(info['start'][1]) for date in time_sort_group['订单日期'].groups.keys()}

    sale_count = sheet.groupby(['商品编号', '订单日期'])['订购数量'].sum()
    for (name, time), v in sale_count.items():
        _id = int(name[1:]) - 1
        for key, value in info.items():
            if _id in value[0]:
                data[time.strftime('%Y/%m')][value[1] - 2] += v

    y_data = []
    for key, value in data.items():
        for i, v in enumerate(value):
            y_data.append([key, v, info['start'][1][i]])

    (
        ThemeRiver(opts.InitOpts(width=pic_width, height=pic_height))
        .add(
            series_name=info['start'][1],
            data=y_data,
            singleaxis_opts=opts.SingleAxisOpts(
                pos_top="50", pos_bottom="50", type_='time'
            )
        )
        .set_global_opts(
            title_opts=opts.TitleOpts(title=title),
            tooltip_opts=opts.TooltipOpts(trigger="axis", axis_pointer_type="line")
        )
        .render(f'{charts_path}{title}.html')
    )
    print(f'{title} 生成成功')


def drawTypeBar():
    title = '定价与销量关系比较图'

    sheet_info = pd.read_excel(file, sheet_name='商品信息表')
    prices = [(i, price) for i, price in enumerate(sheet_info['销售单价'])]
    prices.sort(key=lambda x: x[1], reverse=True)

    x_data = [f'X{i:03d}' for i, _ in prices]

    sale_cnt = list(sheet.groupby('商品编号')['订购数量'].sum())

    bar1 = (
        Bar(opts.InitOpts(width=pic_width, height=pic_height))
        .add_xaxis(x_data)
        .add_yaxis('销量', [sale_cnt[i] for i, _ in prices], z_level=1, label_opts=opts.LabelOpts(is_show=False))
        .extend_axis(yaxis=opts.AxisOpts(name='元'))
        .set_global_opts(title_opts=opts.TitleOpts(title=title),
                         datazoom_opts=opts.DataZoomOpts(type_='inside', range_start=0, range_end=20),
                         )
    )

    line1 = (
        Line(opts.InitOpts(width=pic_width, height=pic_height))
        .add_xaxis(x_data)
        .add_yaxis('定价', [price for _, price in prices], yaxis_index=1, z_level=2,
                   label_opts=opts.LabelOpts(is_show=False))
    )
    bar1.overlap(line1)

    prices = [(i, tot) for i, tot in enumerate(sale_cnt)]
    prices.sort(key=lambda x: x[1], reverse=True)
    bar2 = (
        Bar(opts.InitOpts(width=pic_width, height=pic_height))
        .add_xaxis(x_data)
        .add_yaxis('销量', [tot for _, tot in prices], z_level=1, label_opts=opts.LabelOpts(is_show=False))
        .extend_axis(yaxis=opts.AxisOpts(name='金额/万元'))
        .set_global_opts(datazoom_opts=opts.DataZoomOpts(type_='inside', range_start=0, range_end=20))
    )

    line2 = (
        Line(opts.InitOpts(width=pic_width, height=pic_height))
        .add_xaxis(x_data)
        .add_yaxis('定价', [int(sheet_info['销售单价'].iloc[i]) for i, _ in prices], yaxis_index=1, z_level=2,
                   label_opts=opts.LabelOpts(is_show=False))
    )
    bar2.overlap(line2)

    (
        Page()
        .add(bar1)
        .add(bar2)
        .render(f'{charts_path}{title}.html')
    )
    print(f'{title} 生成成功')


def drawSpecificTypeBar():
    title = '同种类下销量与定价关系图'

    sheet_info = pd.read_excel(file, sheet_name='商品信息表')
    sale_cnt = list(sheet.groupby('商品编号')['订购数量'].sum())
    types = info['start'][1]
    page = Page()
    for type_ in types:
        index = list(info[type_][0])
        prices = sheet_info['销售单价'].iloc[index]
        data = [(i, price) for i, price in zip(index, prices)]
        data.sort(key=lambda x: x[1], reverse=True)
        x_data = [f'X{i:03d}' for i, _ in data]
        bar = (
            Bar(opts.InitOpts(width=pic_width, height=pic_height))
            .add_xaxis(x_data)
            .add_yaxis('销量', [int(sale_cnt[i]) for i, _ in data], z_level=1)
            .extend_axis(yaxis=opts.AxisOpts(name='元'))
            .set_global_opts(title_opts=opts.TitleOpts(title=type_))
        )

        line = (
            Line(opts.InitOpts(width=pic_width, height=pic_height))
            .add_xaxis(x_data)
            .add_yaxis('定价', [price for _, price in data], z_level=2, yaxis_index=1)
        )
        bar.overlap(line)
        page.add(bar)
    page.render(f'{charts_path}{title}.html')
    print(f'{title} 生成成功')


def drawPrice():
    title = '不同价位区间销量与定价关系图'
    sheet_info = pd.read_excel(file, sheet_name='商品信息表')
    n = int(max(sheet_info['销售单价']) / 100) + 1
    prices = [(i, price) for i, price in enumerate(sheet_info['销售单价'])]
    prices.sort(key=lambda x: x[1], reverse=True)
    sale_cnt = list(sheet.groupby('商品编号')['订购数量'].sum())
    data = [[], [], []]

    for i, price in prices:
        data[int(price / 100)].append((i, price))
    page = Page()
    for n_ in range(n):
        x_data = [f'X{i:03d}' for i, _ in data[n_]]
        bar = (
            Bar(opts.InitOpts(width=pic_width, height=pic_height))
            .add_xaxis(x_data)
            .add_yaxis('销量', [int(sale_cnt[i]) for i, _ in data[n_]], z_level=1)
            .extend_axis(yaxis=opts.AxisOpts(name='元'))
            .set_global_opts(title_opts=opts.TitleOpts(title=f'{n_ * 100}-{(n_ + 1) * 100}价位区间'))
        )

        line = (
            Line(opts.InitOpts(width=pic_width, height=pic_height))
            .add_xaxis(x_data)
            .add_yaxis('定价', [price for _, price in data[n_]], z_level=2, yaxis_index=1)
        )
        bar.overlap(line)
        page.add(bar)
    page.render(f'{charts_path}{title}.html')
    print(f'{title} 生成成功')


def drawArea():
    title = '各区域销售分布'
    area_name = sheet.groupby('所在区域').groups.keys()
    area_sum = sheet.groupby('所在区域')['金额'].sum()
    area_cnt = sheet.groupby('所在区域').count()['订单编码']
    page = Page()
    pie1 = (
        Pie(opts.InitOpts(width=pic_width, height=pic_height))
        .add('各区域销售金额分布', [(name, sum_) for name, sum_ in zip(area_name, area_sum)])
        .set_global_opts(title_opts=opts.TitleOpts(title='各区域销售金额分布'))
        .set_series_opts(label_opts=opts.LabelOpts(formatter="{b}: {d}%"),
                         tooltip_opts=opts.TooltipOpts(
                             trigger="item", formatter="{a} <br/>{b}: {c} ({d}%)"
                         )
                         )
    )

    pie2 = (
        Pie(opts.InitOpts(width=pic_width, height=pic_height))
        .add('各区域销售量分布', [(name, cnt) for name, cnt in zip(area_name, area_cnt)])
        .set_global_opts(title_opts=opts.TitleOpts(title='各区域销售量分布'))
        .set_series_opts(label_opts=opts.LabelOpts(formatter="{b}: {c} ({d}%)"),
                         tooltip_opts=opts.TooltipOpts(
                             trigger="item", formatter="{a} <br/>{b}: {c} ({d}%)"
                         )
                         )
    )
    page.add(pie1).add(pie2).render(f'{charts_path}{title}.html')
    print(f'{title} 生成成功')


if __name__ == '__main__':
    pic_width = '1260px'
    pic_height = '560px'
    charts_path = './charts/'
    file = pd.ExcelFile("./DataSet/cosmetics.xlsx")

    sheet = format_column()
    info = get_types()

    drawLine()
    drawMap()
    drawProvince()
    drawArea()
    drawPrice()

    drawTypeSunburst()
    drawTypeRiver()
    drawTypeBar()
    drawSpecificTypeBar()
