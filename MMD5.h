/*
 * myMD5.h
 *
 *  Created on: 2009-10-2
 *      Author: Administrator
 */

#ifndef MYMD5_H_
#define MYMD5_H_
//#include"datatype.h"
//数据大小

#include <iostream>
using namespace std;

#define ENCRYPT_KEY_LEN					5									//密钥长度

//////////////////////////////////////////////////////////////////////////

#define S11		7
#define S12		12
#define S13		17
#define S14		22
#define S21		5
#define S22		9
#define S23 	14
#define S24 	20
#define S31 	4
#define S32 	11
#define S33 	16
#define S34 	23
#define S41 	6
#define S42 	10
#define S43 	15
#define S44 	21

#define F(x,y,z) (((x)&(y))|((~x)&(z)))
#define G(x,y,z) (((x)&(z))|((y)&(~z)))
#define H(x,y,z) ((x)^(y)^(z))
#define I(x,y,z) ((y)^((x)|(~z)))

#define ROTATE_LEFT(x,n) (((x)<<(n))|((x)>>(32-(n))))

#define FF(a,b,c,d,x,s,ac)													\
{																			\
	(a)+=F((b),(c),(d))+(x)+(unsigned int)(ac);						\
	(a)=ROTATE_LEFT((a),(s));												\
	(a)+=(b);																\
}

#define GG(a,b,c,d,x,s,ac)													\
{																			\
	(a)+=G((b),(c),(d))+(x)+(unsigned int)(ac);						\
	(a)=ROTATE_LEFT ((a),(s));												\
	(a)+=(b);																\
}

#define HH(a,b,c,d,x,s,ac)													\
{																			\
	(a)+=H((b),(c),(d))+(x)+(unsigned int)(ac);						\
	(a)=ROTATE_LEFT((a),(s));												\
	(a)+=(b);																\
}

#define II(a,b,c,d,x,s,ac)													\
{																			\
	(a)+=I((b),(c),(d))+(x)+(unsigned int)(ac);						\
	(a)=ROTATE_LEFT((a),(s));												\
	(a)+=(b);																\
}

//////////////////////////////////////////////////////////////////////////
/*
 * typedef TInt64				int64;
typedef TUint64				uint64;


typedef  wchar_t           wchar;
//typedef void              void
typedef TBool             BOOL;
typedef unsigned char		unsigned char;  //unsigned char
typedef char              int8;
typedef TInt16            int16;
typedef TInt              int;
typedef TUint8            uint8;
typedef TUint16           uint16;
typedef TUint32           unsigned int; //unsigned long int
//typedef TUint             unsigned int //unsigned int
 */

//MD5 加密类
class CMD5
{
	//变量定义
	private:
		unsigned int				state[4];
		unsigned int				count[2];
		unsigned char					buffer[64];
		unsigned char					PADDING[64];

		//函数定义
	public:
		//构造函数
		CMD5() { MD5Init(); }

		//功能函数
	public:
		//最终结果
		void MD5Final(unsigned char digest[16]);
		//设置数值
		void MD5Update(unsigned char * input, unsigned int inputLen);

		//内部函数
	private:
		//初始化
		void MD5Init();
		//置位函数
		void MD5_memset(unsigned char * output, int value, unsigned int len);
		//拷贝函数
		void MD5_memcpy(unsigned char * output, unsigned char * input, unsigned int len);
		//转换函数
		void MD5Transform(unsigned int state[4], unsigned char block[64]);
		//编码函数
		void Encode(unsigned char * output, unsigned int * input, unsigned int len);
		//解码函数
		void Decode(unsigned int *output, unsigned char * input, unsigned int len);

};

//MD5 加密类
class CMD5Encrypt
{
	//函数定义
public:
	//构造函数
	CMD5Encrypt() {}

	//功能函数
public:
	//生成密文
	static string EncryptData(unsigned char *pszSrcData);
};


#endif /* MYMD5_H_ */
