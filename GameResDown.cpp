#include "GameResDown.h"
#include "DownLoadFile.h"
#include "cocos2d.h"
#include "support/zip_support/ZipUtils.h"
#include "Util.h"
USING_NS_CC;

#ifdef WIN32
#define snprintf _snprintf
#endif

#include <stdio.h> 
#ifdef _WIN32
#include <io.h>
#include <direct.h>  
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

static pthread_mutex_t  s_download_mutex;
GameResDown* GameResDown::m_pInstance = NULL;
GameResDown::GameResDown()
{
	m_isDownload = false;
	pthread_mutex_init(&s_download_mutex, NULL);
	DownLoadFile::instance()->addListener(this);
}

GameResDown::~GameResDown()
{
	pthread_mutex_destroy(&s_download_mutex);
	DownLoadFile::instance()->removeListener(this);
}

GameResDown* GameResDown::instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new GameResDown();
	}
	return m_pInstance;
}

void GameResDown::downResFile(string url, int gameid, int versionid, string module, string channid)
{
	char fileName[64] = {0};
	snprintf(fileName,sizeof(fileName) - 1,"R%d_%s_%d_%s.zip",gameid, module.c_str(), versionid, channid.c_str());
	m_savePath = getResPath(gameid);
	CCLog("savePath:%s, filename:%s", m_savePath.c_str(), fileName);
	DownRes downRes;
	downRes.gameid = gameid;
	downRes.versionid = versionid;
	downRes.module = module;
	downRes.channid = channid;
	downRes.downurl = url;

	//判断文件夹是否存在，不存在就创建文件夹
	if (!isFileExist(m_savePath))
	{
#ifdef _WIN32
		mkdir(m_savePath.c_str());
#else
		mkdir(m_savePath.c_str(), S_IRWXU);
#endif
	}

	pthread_mutex_lock(&s_download_mutex); //Get request task from queue
	m_downloadReses.push(downRes);
	pthread_mutex_unlock(&s_download_mutex);

	if (m_isDownload)
	{
		return;
	}
	SetRunning(true);
	onStart();
}

void GameResDown::onGameEvent(int handle, int eventID, void *param)
{
	switch (eventID)
	{
	case gameDownLoadFailed:
	case gameDownLoadCancle:
		fireEvent(handle, eventID, param);
		break;	
	case gameDownLoadFinish:
	case gameDownLoadFileExist:
		fireEvent(handle, eventID, param);
		{
			string fileFullPath = m_savePath+getFileName(m_downloadRes.gameid, m_downloadRes.versionid, m_downloadRes.module, m_downloadRes.channid);
			//解压文件
			if (ZipUtils::unZipFileToPath(m_savePath.c_str(), fileFullPath.c_str()))
			{
				fireEvent(0, gameUnzipFinish, (void*)&m_downloadRes);
			}
			else
			{
				fireEvent(0, gameUnzipFailed, (void*)&m_downloadRes);
			}
		}
		break;
	case gameDownLoading:
		{
			fireEvent(0, gameDownLoading,param);
		}
		break;
	}
}			 

bool GameResDown::isFileExist(string fileFullName)
{
	if (access(fileFullName.c_str(), 0) == -1)
	{
		return false;
	}
	return true;
}

bool GameResDown::isFileExist(int gameid, int versionid, string module, string channid)
{
	string filePath = getResPath(gameid);;
	char fileName[64] = {0};
	snprintf(fileName,sizeof(fileName) - 1,"R%d_%s_%d_%s.zip",gameid, module.c_str(), versionid, channid.c_str());

	return isFileExist(filePath+fileName);
}

string GameResDown::getResPath(int gameid)
{
	string filePath = CCFileUtils::sharedFileUtils()->getWriteablePath();
	filePath += "V";
	filePath += int2string(gameid);
#ifdef _WIN32
	filePath += "\\";
#else
	filePath += "/";
#endif
	return filePath;
}

vector<string> GameResDown::getFiles(string filePath)
{
	vector<string> files;

#ifdef _WIN32
	//文件句柄  
	long   hFile   =   0;  
	//文件信息  
	struct _finddata_t fileinfo;  
	string p;  
	if((hFile = _findfirst(p.assign(filePath).append("\\*").c_str(),&fileinfo)) !=  -1)  
	{  
		do  
		{  
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if(!(fileinfo.attrib &  _A_SUBDIR))  
			{  
				files.push_back(p.assign(filePath).append("\\").append(fileinfo.name) );  
			}  
		}while(_findnext(hFile, &fileinfo)  == 0);  
		_findclose(hFile);  
	}  
#else
	struct dirent* ent = NULL;
	DIR* pDir;
	pDir = opendir(filePath.c_str());
	while(NULL != (ent = readdir(pDir)))
	{
		string fullpath = filePath + "/" + ent->d_name;
		if(isFile(fullpath))
		{
			files.push_back(fullpath);
		}
	}
	closedir(pDir);
#endif
	return files;
}

void GameResDown::deleteResFile(vector<string> files, string subStr)
{
	for (int i=0; i<files.size(); i++)
	{
		if (files[i].find(subStr) != string::npos)
		{
			//delete
			remove(files[i].c_str());
		}
	}
}

void GameResDown::Run()
{
	pthread_mutex_lock(&s_download_mutex); //Get request task from queue
	m_isDownload = true;
	while(m_downloadReses.size() > 0)
	{
		m_downloadRes = m_downloadReses.front();
		m_downloadReses.pop();
		pthread_mutex_unlock(&s_download_mutex);

		string fileName = getFileName(m_downloadRes.gameid, m_downloadRes.versionid, m_downloadRes.module, m_downloadRes.channid);
		
		CCLog(" runaction savePath:%s, filename:%s", m_savePath.c_str(), fileName.c_str());
		if (isFileExist(m_savePath+fileName))
		{
			fireEvent(0, gameDownLoadFileExist, (void*)&m_downloadRes);
			//解压文件
			if (ZipUtils::unZipFileToPath(m_savePath.c_str(), (m_savePath+fileName).c_str()))
			{
				fireEvent(0, gameUnzipFinish, (void*)&m_downloadRes);
			}
			else
			{
				fireEvent(0, gameUnzipFailed, (void*)&m_downloadRes);
			}
			return;
		}
		//删除以前版本的文件
		char delFile[32] = {0};
		snprintf(delFile, sizeof(delFile)-1, "R%d_%s_", m_downloadRes.gameid, m_downloadRes.module.c_str());
		deleteResFile(getFiles(m_savePath), delFile);

		//下载文件
		DownLoadFile::instance()->downLoadFile(m_downloadRes.downurl+fileName, m_savePath+fileName);
		
	}
	m_isDownload = false;
	pthread_mutex_unlock(&s_download_mutex);
}

string GameResDown::getFileName(int gameid, int versionid, string module, string channid)
{
	char fileName[64] = {0};
	snprintf(fileName,sizeof(fileName) - 1,"R%d_%s_%d_%s.zip",gameid, module.c_str(), versionid, channid.c_str());
	return string(fileName);
}


bool GameResDown::isFile(const string fileName)
{
#ifdef _WIN32
	return true;
#else
	struct stat st;
	int ret = stat(fileName.c_str(), &st);
	return ret>=0 && S_ISREG(st.st_mode);
#endif
}