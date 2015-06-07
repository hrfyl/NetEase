#ifndef URLINFO
#define URLINFO

#define NULL_REQUEST -1																//		不明操作..
#define LOGIN QString("http://music.163.com/api/login/")							//		登录操作..
#define GETMUSICLIST QString("http://music.163.com/api/user/playlist/?offset=")		//		获取歌单...
#define GETLISTINFO QString("http://music.163.com/api/playlist/detail?id=")			//		获取歌单详细信息...

#define SEARCH QString("http://music.163.com/api/search/get/web")					//		搜索...
#define GETMP3URL QString("http://music.163.com/api/song/detail?ids=")				//		通过songs id 获取mp3url..
#define LYRIC QString("http://music.163.com/api/song/lyric?id=")					//		获取歌词

#endif // URLINFO

