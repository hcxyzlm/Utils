#ifndef __GameResDown_h__
#define __GameResDown_h__

#include "IGameEvent.h"
#include "../../Core/GameEventProducer.h"
#include "CThread.h"

#include <iostream> 
#include <queue>
using namespace std;

struct DownRes
{
	int gameid;
	int versionid;
	string module;
	string channid;
	string downurl;
};

class GameResDown : public IGameEvent , public GameEventProducer , public CThread
{
private:
	GameResDown();
	virtual ~GameResDown();

public:
	static GameResDown* instance();
	void downResFile(string url, int gameid, int versionid, string module="default", string channid="default");  //指定下载的资源文件，后台配置以Rgameid_module_versionid_channid.zip命名
	virtual void onGameEvent(int handle, int eventID, void *param);

	bool isFileExist(string fileFullName);
	bool isFileExist(int gameid, int versionid, string module="default", string channid="default");
	string getResPath(int gameid);
	void Run();
private:
	vector<string> getFiles(string filePath);
	void deleteResFile(vector<string> files, string subStr);  //删除文件名字中含有自定字符串的文件
	string getFileName(int gameid, int versionid, string module, string channid);
	bool isFile(const string fileName);
private:
	DownRes m_downloadRes;
	static GameResDown *m_pInstance;
	queue<DownRes> m_downloadReses;
	bool m_isDownload;
	string m_savePath;
};

#endif