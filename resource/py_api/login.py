# -*- coding: utf-8 -*-

import re
import os
import json
import requests
from Crypto.Cipher import AES
import hashlib
import random
import base64
import sqlite3

default_timeout = 10

modulus = '00e0b509f6259df8642dbc35662901477df22677ec152b5ff68ace615bb7b725152b3ab17a876aea8a5aa76d2e417629ec4ee341f56135fccf695280104e0312ecbda92557c93870114af6c9d05c4f7f0c3685b7a46bee255932575cce10b424d813cfe4875d3e82047b97ddef52741d546b8e289dc6935b3ece0462db0a22b8e7'
nonce = '0CoJUm6Qyw8W8jud'
pubKey = '010001'

header = {
	'Accept': '*/*',
	'Accept-Encoding': 'gzip,deflate,sdch',
	'Accept-Language': 'zh-CN,zh;q=0.8,gl;q=0.6,zh-TW;q=0.4',
	'Connection': 'keep-alive',
	'Content-Type': 'application/x-www-form-urlencoded',
	'Host': 'music.163.com',
	'Referer': 'http://music.163.com/search/',
	'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/33.0.1750.152 Safari/537.36'
}

# 登录加密算法, 基于https://github.com/stkevintan/nw_musicbox脚本实现
def encrypted_login(username, password):
	text = {
		'username': username,
		'password': hashlib.md5(str(password)).hexdigest(),
		'rememberLogin': 'true'
	}
	text = json.dumps(text)
	secKey = createSecretKey(16)
	encText = aesEncrypt(aesEncrypt(text, nonce), secKey)
	encSecKey = rsaEncrypt(secKey, pubKey, modulus)
	data = {
		'params': encText,
		'encSecKey': encSecKey
	}
	return data


def encrypted_phonelogin(username, password):
	text = {
		'phone': username,
		'password': password,
		'rememberLogin': 'true'
	}
	text = json.dumps(text)
	secKey = createSecretKey(16)
	encText = aesEncrypt(aesEncrypt(text, nonce), secKey)
	encSecKey = rsaEncrypt(secKey, pubKey, modulus)
	data = {
		'params': encText,
		'encSecKey': encSecKey
	}
	return data


def aesEncrypt(text, secKey):
	pad = 16 - len(text) % 16
	text = text + pad * chr(pad)
	encryptor = AES.new(secKey, 2, '0102030405060708')
	ciphertext = encryptor.encrypt(text)
	ciphertext = base64.b64encode(ciphertext)
	return ciphertext


def rsaEncrypt(text, pubKey, modulus):
	text = text[::-1]
	rs = int(text.encode('hex'), 16) ** int(pubKey, 16) % int(modulus, 16)
	return format(rs, 'x').zfill(256)


def createSecretKey(size):
	return (''.join(map(lambda xx: (hex(ord(xx))[2:]), os.urandom(size))))[0:16]

# 登录
def login(username, password, databasePath):
	pattern = re.compile(r'^0\d{2,3}\d{7,8}$|^1[34578]\d{9}$')
	if (pattern.match(username)):
		return phone_login(username, password)
	action = 'https://music.163.com/weapi/login/'
	data = encrypted_login(username, password)
	try:
		userInfo = httpRequest(action, data)
		sqlite_write(userInfo, databasePath)
		return 0
	except:
		return 1

# 手机登录
def phone_login(username, password, databasePath):
	action = 'http://music.163.com/weapi/login/cellphone'
	data = encrypted_phonelogin(username, password)
	try:
		userInfo = httpRequest(action, data)
		sqlite_write(userInfo, databasePath)
		return 0
	except:
		return 1

def httpRequest(action, query):
	connection = requests.post(
		action,
		data=query,
		headers=header,
		timeout=default_timeout
	)
	connection.encoding = "UTF-8"
	
	cookies_tuple = tuple(connection.cookies)
	cookies = getCookies(cookies_tuple)
	data = json.loads(connection.text)
	
	userId = data['account']['id']
	userName = data['account']['userName']
	userNickName = data['profile']['nickname']
	
	userInfo = [userId, userName, userNickName, cookies]
	return userInfo

def sqlite_write(user_info, databasePath) :
	conn = sqlite3.connect(databasePath)
	sql = """update user_info set user_id='%s', user_name='%s',
		user_nickname='%s', user_autologin='1', user_cookies='%s' where id='1';
		""" % (user_info[0], user_info[1], user_info[2], user_info[3])
	conn.execute(sql)
	conn.commit()
	conn.close()

def getCookies(_tuple):
	cookies = ""
	for i in _tuple :
		_str = str(i)
		cookies += _str.split(' ')[1] + "; "
	return cookies
