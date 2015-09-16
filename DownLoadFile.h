#ifndef __DownLoadFile_h__
#define __DownLoadFile_h__

#include <iostream>
using namespace std;

#include <curl/curl.h>
#include <curl/easy.h>
#include "../Core/GameEventProducer.h"

class DownLoadFile : public GameEventProducer
{
private:
	DownLoadFile();
	~DownLoadFile();
public:
	static DownLoadFile* instance();
	void downLoadFile(string url, string fileFullName);
	void onProgressbar(double maxsize, double downloadsize);

private:
	bool dlFile(string url);
	string getDownFileTempName(string filename);

private:
	string m_fileFullName;
};

#endif