#include "DownLoadFile.h"

#include "GameBaseInfo.h"
#include "cocos2d.h"
USING_NS_CC;

#include<stdio.h>
#ifdef _WIN32
#include <io.h>
#else
#include<unistd.h>
#endif


DownLoadFile::DownLoadFile()
{
}

DownLoadFile::~DownLoadFile()
{
}

DownLoadFile* DownLoadFile::instance()
{
	static DownLoadFile *pInstance = NULL;
	if (pInstance==NULL)
	{
		pInstance = new DownLoadFile();
	}
	return pInstance;
}

void DownLoadFile::downLoadFile(string url, string fileFullName)
{
	m_fileFullName = fileFullName;
	if (!dlFile(url))
	{
		//下载失败
		remove(getDownFileTempName(m_fileFullName).c_str());
		remove(m_fileFullName.c_str());
		fireEvent(0, gameDownLoadFailed, url.c_str());
	}
	else
	{
		fireEvent(0, gameDownLoadFinish, url.c_str());			
	}
}

int saveFile(void *buffer, size_t size, size_t nmemb, void *stream);
int onPercent(void* ptr, double fileSize, double downloadSize, double rUlTotal, double rUlNow);

bool DownLoadFile::dlFile(string url)
{	
	CURLcode return_code;
	return_code = curl_global_init(CURL_GLOBAL_ALL);
	if (CURLE_OK != return_code)
	{
		printf("init libcurl failed.");
		return false;
	}
	// 获取easy handle
	CURL *easy_handle = curl_easy_init(); 
	if (NULL == easy_handle) 
	{ 
		printf( "get a easy handle failed.") ;   
		curl_global_cleanup(); 
		return false;
	}

	FILE *fp = fopen(getDownFileTempName(m_fileFullName).c_str(), "wb");

	//设置下载URL
	return_code = curl_easy_setopt(easy_handle,CURLOPT_URL, url.c_str());
	//设置重定向URL
	return_code = curl_easy_setopt(easy_handle,CURLOPT_FOLLOWLOCATION, url.c_str());
	//设置回调函数
	return_code = curl_easy_setopt(easy_handle,CURLOPT_WRITEFUNCTION, saveFile);
	//设置回调数据
	return_code = curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, fp);
	if (return_code != CURLE_OK) 
	{
		return false;
	}

	//curl的进度条声明      
	curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0);
	//回调进度条函数
	curl_easy_setopt(easy_handle, CURLOPT_PROGRESSFUNCTION, onPercent);

	// 执行数据请求
	if (curl_easy_perform(easy_handle) != CURLE_OK) {
		return false;
	}
	// 释放资源
	fclose(fp);
	curl_easy_cleanup(easy_handle);
	curl_global_cleanup();

	rename(getDownFileTempName(m_fileFullName).c_str(), m_fileFullName.c_str());

	return true;
}

void DownLoadFile::onProgressbar( double maxsize, double downloadsize )
{
	structDownLoad pDownLoad;
	pDownLoad.cur = downloadsize;
	pDownLoad.max = maxsize;
	pDownLoad.fileFullName = m_fileFullName;
	fireEvent(0, gameDownLoading, (void *)&pDownLoad);
}

int saveFile(void *buffer, size_t size, size_t nmemb, void *stream)
{
	FILE *fp = (FILE *)stream;
	size_t return_size = fwrite(buffer, size, nmemb, fp);
	return return_size;
}

int onPercent(void* ptr, double fileSize, double downloadSize, double rUlTotal, double rUlNow)
{
	DownLoadFile::instance()->onProgressbar(fileSize , downloadSize);
	return 0;
}

string DownLoadFile::getDownFileTempName(string fileName)
{
	return fileName+".tmp";
}


