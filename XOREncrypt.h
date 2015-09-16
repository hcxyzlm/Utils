#ifndef __CXOREncrypt_h__
#define __CXOREncrypt_h__

#pragma once


class CXOREncrypt
{
	//函数定义
private:
	//构造函数
	CXOREncrypt(void);

	//功能函数
public:
	//生成密文
	static unsigned short EncryptData(const char *pSrcData, char *pEncrypData, unsigned short wSize);
	//解开密文
	static unsigned short CrevasseData(const char * pEncrypData, char *pSrcData, unsigned short wSize);
};

#endif