#ifndef __CXOREncrypt_h__
#define __CXOREncrypt_h__

#pragma once


class CXOREncrypt
{
	//��������
private:
	//���캯��
	CXOREncrypt(void);

	//���ܺ���
public:
	//��������
	static unsigned short EncryptData(const char *pSrcData, char *pEncrypData, unsigned short wSize);
	//�⿪����
	static unsigned short CrevasseData(const char * pEncrypData, char *pSrcData, unsigned short wSize);
};

#endif