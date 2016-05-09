# NetEaseMusic-Qt for Linux

[![](https://github.com/fengleyl/NetEaseMusic/blob/master/images/NetEase.png)]()
[![](https://github.com/fengleyl/NetEaseMusic/blob/master/images/NetEase1.png)]()

---

# 概述

1. `NetEaseMusic-Qt`是基于WEB网易云音乐为`Linux`用户开发的一款音乐播放器

2. `NetEaseMusic-Qt`使用`C++/Qt`(UI)和`Python`(登陆接口)开发

3. `NetEaseMusic-Qt`目前实现的功能有:

	1. 登录网易云音乐(使用Python基于[nw_musicbox](https://github.com/stkevintan/nw_musicbox)脚本实现).

	2. 获取个人歌单

	3. 播放网易云歌曲

	4. 私人FM

	5. 添加收藏歌曲和取消收藏功能(仅限我喜欢的歌单..)
	
	6. ~~每日推荐歌曲(可能会有歌曲标题和歌手等信息匹配不上的情况)~~

---

# 问题

## python

>由于项目使用Python脚本实现登录.所以编译之前,请解决相应的依赖.<br>
具体依赖请看: `resource/py_api/login.py`文件<br>
如果是windows用户,请自行解决python问题.<br>
**注意: NetEase.pr文件也需要做相应的更改** <br>

---

## 解决媒体播放

本项目使用mplayer作为后端播放资源.<br>

+ windows下mplayer下载: http://www.mplayerhq.hu/design7/dload.html

+ Linux下mplayer
		
		ArchLinux: sudo pacman -S mplayer

		debain: sudo apt-get install mplayer
		
		openSUSE: sudo zypper in mplayer

---

## 登录问题

如果一直无法登录,可以尝试把你的用户信息写到sqlite数据库`user_info`表,以下是`user_info`的表结构:

	id|user_name|user_id|user_nickname|user_autologin|user_cookies

键说明: 

1. id: 做更新数据用的; 数据类型: int [主键]

2. user_name: 用户名.一般形式`0_email`; 数据类型: text

3. user_id: 网易云唯一标识; 数据类型: text [非空]

4. user_nickname: 网易的昵称;  数据类型: text

5. user_autologin: 是否自动获取云音乐信息,其值可以是; `0` 表示不自动获取, `1` 表示自动获取; 数据类型: int [非空]

6. user_cookies: 你的Cookies; 数据类型: text

---

# 配置文件路径

1. 数据库的配置文件: `~/.config/NetEase/resource/sql/music.db`

2. qss配置文件: `~/.config/NetEase/resource/qss/default.qss`

3. python login脚本: `~/.config/NetEase/resource/py_api/login.py`

**注:** 如果是`Windows`用户,配置文件在当前目录下`NetEase/resource/*`

---

# 更新日志

## 2016-05-09

1. 修复登录问题

---

## 2015-11-19

1. 添加私人FM

2. 添加移除指定歌曲([红心歌单]和[私人FM]会永远的移除)

3. 修复搜索歌曲和播放的歌曲不匹配的问题

---

# License

1. 本项目基于 [MIT License](https://github.com/fengleyl/NetEaseMusic/blob/master/LICENSE) 发布

2. 本项目所有网络资源归网易公司所有.任何人不得将该项目用于商业活动.
