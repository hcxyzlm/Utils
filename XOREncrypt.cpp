#include<stdio.h>
#include <cstdlib>
#include <string>
#include "XOREncrypt.h"

#define  CountArray(array)  sizeof(array)/sizeof(array[0])

//数据大小
#define ENCRYPT_KEY_LEN		6		//密钥长度
//生成密文
unsigned short CXOREncrypt::EncryptData(const char * pSrcData, char * pEncrypData, unsigned short wSize)
{
	//效验参数
	if(wSize<=0) return 0;
	if(pSrcData==NULL) return 0;
	if(pEncrypData==NULL) return 0;

	//初始化参数
	pEncrypData[0]=0;

	//生成密钥
	unsigned char szRandKey[ENCRYPT_KEY_LEN];
	szRandKey[0]=strlen(pSrcData);
	for (int i=1;i<CountArray(szRandKey);i++) szRandKey[i]=rand()%255;

	//生成密文
	unsigned char bTempCode=0;
	unsigned short wTimes=(szRandKey[0]+ENCRYPT_KEY_LEN-1)/ENCRYPT_KEY_LEN*ENCRYPT_KEY_LEN;
	if ((wTimes*4+1)>wSize) return wTimes*4+1;
	for (int i=0;i<wTimes;i++)
	{
		if (i<szRandKey[0]) bTempCode=pSrcData[i]^szRandKey[i%ENCRYPT_KEY_LEN];
		else bTempCode=szRandKey[i%ENCRYPT_KEY_LEN]^(rand()%255);
		sprintf(pEncrypData+i*4,"%02x%02x",szRandKey[i%ENCRYPT_KEY_LEN],bTempCode);
	}

	return wTimes*4;
}

//解开密文
unsigned short CXOREncrypt::CrevasseData(const char * pEncrypData, char *pSrcData, unsigned short wSize)
{
	//效验参数
	if(wSize<=0) return 0;
	if(pSrcData==NULL) return 0;
	if(pEncrypData==NULL) return 0;

	//初始化参数
	pSrcData[0]=0;

	//效验长度
	unsigned short wEncrypPassLen=strlen(pEncrypData);
	if (wEncrypPassLen<ENCRYPT_KEY_LEN*4) return 0;

	//效验长度
	char * pEnd=NULL;
	char szTempBuffer[3]={pEncrypData[0],pEncrypData[1],0};
	unsigned short wSoureLength=(unsigned short)strtol(szTempBuffer,&pEnd,16);
	if (wEncrypPassLen!=((wSoureLength+ENCRYPT_KEY_LEN-1)/ENCRYPT_KEY_LEN*ENCRYPT_KEY_LEN*4)) return 0;
	if (wSoureLength>(wSize-1)) return wSoureLength;

	//解开密码
	unsigned char szKeyBuffer[3];
	for (int i=0;i<wSoureLength;i++)
	{
		szKeyBuffer[0]=pEncrypData[i*4];
		szKeyBuffer[1]=pEncrypData[i*4+1];
		szKeyBuffer[2]=0;
		szTempBuffer[0]=pEncrypData[i*4+2];
		szTempBuffer[1]=pEncrypData[i*4+3];
		szTempBuffer[2]=0;
		unsigned int uKey=strtol((char *)szKeyBuffer,&pEnd,16);
		unsigned int uEncrypt=strtol(szTempBuffer,&pEnd,16);
		pSrcData[i]=(char)((unsigned char)uKey^(unsigned char)uEncrypt);
	}
	pSrcData[wSoureLength]=0;

	return wSoureLength;
}
