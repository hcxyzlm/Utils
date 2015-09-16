#include "HttpGetImg.h"
#include "Util.h"
#include "cocos2d.h"  
USING_NS_CC;  
USING_NS_CC_EXT; 
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
HttpGetImg* HttpGetImg::m_pInstance = NULL;

void HttpGetImg::setWebCallBack(IWeb *pWeb)
{
	m_pWebCallBack = pWeb;
}

HttpGetImg::HttpGetImg(void)
{
    m_Sprite=CCSprite::create();
}
HttpGetImg* HttpGetImg::instance()
{
    if (!m_pInstance)
    {
        m_pInstance = new HttpGetImg();
    }
    return m_pInstance;
}
void HttpGetImg::GetHttpImg(string imgurl,int userid)
{
    CCLog("HttpGetImg   1213132   GetHttpImg");
std::string pathToSave=CCFileUtils::sharedFileUtils()->getWriteablePath()+"Head";
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
 CCLog("HttpGetImg   1213132   GetHttpIm212313g pathToSave %s",pathToSave.c_str());
    if ((access(pathToSave.c_str(), 0) == -1))
	{
		 CCLog("HttpGetImg   1213132   GetHtsdfsdtpImg");
		  mkdir(pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
#else

    if ((GetFileAttributesA(pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
    {
		 CCLog("HttpGetImg   1213132   Ge1232tHttpImg");
        CreateDirectoryA(pathToSave.c_str(), 0);
    }
#endif
    CCHttpRequest* request = new CCHttpRequest();
    // required fields
    request->setUrl(imgurl.c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
	CCLog("HttpGetImg:: onHttpRequestRptImg22 ");
    request->setResponseCallback(this, httpresponse_selector(HttpGetImg::onHttpRequestRptImg));
    // optional fields
    char thisnumber[10] = "";
    sprintf(thisnumber, "%d",userid);
    request->setTag(thisnumber);
    CCHttpClient::getInstance()->send(request);

    request->release();
    CCLog("HttpGetImg   121er3132   GetHttpImg");
}

void HttpGetImg::onHttpRequestRptImg(CCHttpClient *sender, CCHttpResponse *response)
{
	CCLog("HttpGetImg:: onHttpRequestRptImg ");
    char c_tag[20]= "";
    sprintf(c_tag, "%s",response->getHttpRequest()->getTag());
        CCLog("%s completed", response->getHttpRequest()->getTag());
    string str_tag = c_tag;
    if (!response)
    {
		CCLog("HttpGetImg:: onHttpRequestRptImg 1");
        return;
    }
	CCLog("HttpGetImg:: onHttpRequestRptImg 2");
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
            CCLog("%s completed", response->getHttpRequest()->getTag());
    }
    CCLog("HttpGetImg:: onHttpRequestRptImg 3");
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer: %s", response->getErrorBuffer());
        return;
    }
	CCLog("HttpGetImg:: onHttpRequestRptImg 4");
	string headurl=response->getHttpRequest()->getUrl();
    int nend=headurl.find_last_of("/", headurl.length());
	if (nend==-1)
	{
		CCLog("HttpGetImg:: onHttpRequestRptImg 5");
	}
	CCLog("HttpGetImg:: onHttpRequestRptImg 6");
	string strname =  headurl.substr((nend+1), headurl.length()-(nend+1));
    vector<char> *buffer = response->getResponseData();
	char * buf=(char *)malloc(buffer->size());
	std::copy(buffer->begin(),buffer->end(),buf);
	string filename=getData();
	CCLog("HttpGetImg:: onHttpRequestRptImg 7 strname %s",strname.c_str());
	 std::string pathToSave=CCFileUtils::sharedFileUtils()->getWriteablePath()+"Head/"+filename;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	 CCLog("HttpGetImg:: onHttpRequestRptImg 11");
	if ((access(pathToSave.c_str(), 0) == -1))
	{
		 mkdir(pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
#else
    if ((GetFileAttributesA(pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
    {
        CreateDirectoryA(pathToSave.c_str(), 0);
    }
	CCLog("HttpGetImg:: onHttpRequestRptImg 10");
#endif
	CCLog("HttpGetImg:: onHttpRequestRptImg 8 pathToSave %s",pathToSave.c_str());
    std::string path = pathToSave+"/"+strname;
   FILE *fp=fopen(path.c_str(),"wb+");
	if (!fp)
	{
		CCLog("HttpGetImg:: onHttpRequestRptImg  error error error 21 path %s",path.c_str());
		return;
	}
	CCLog("HttpGetImg:: onHttpRequestRptImg 21 path %s",path.c_str());
	fwrite(buf,1,buffer->size(),fp); 
	CCLog("HttpGetImg:: onHttpRequestRptImg 22 path %s",path.c_str());
	fclose(fp);
	m_pWebCallBack->onGetPicRet(1,atoi(response->getHttpRequest()->getTag()),path);  //获取配置失败
    CCLog(" completed  m_Sprite setTexture");	
    //已下是自己封装的PublicDoFunc异步调用主线程执行换图操作的部分，可以自己去封装
    //SEL_CallFuncN pfnCallback =callfuncN_selector(View_Room::upPlayerHead);
    //CCNode* nd = CCNode::create();
    //buffer->clear();
    //nd->setTag(CCString::create(str_tag)->intValue());
    //PublicDoFunc::toDoFuncN(LAYER_ROOM, pfnCallback,nd);
}
void HttpGetImg::showhead(float time)
{
    
}
void HttpGetImg::setImgSize(int oldhead_w,int oldhead_h,CCSprite* sprite)
{
    float s_X=oldhead_w/sprite->getContentSize().width;
    float s_Y=oldhead_h/sprite->getContentSize().height;
    sprite->setScaleX(s_X);
    sprite->setScaleY(s_Y);
}

void HttpGetImg::CreateImg(CCSprite* tagSprite,CCSprite* oldSprite,CCTexture2D* texture,int newTag)
{
    int oldw =oldSprite->getContentSize().width;
    int oldh =oldSprite->getContentSize().height;
    if(texture!=NULL&&oldSprite!=NULL)
    {
        oldSprite->setTexture(texture);
//        CCSprite* sprite = CCSprite::createWithTexture(texture);
//        //按原比例设置
//        setImgSize(oldw,oldh,sprite);
//        sprite->setPosition(oldSprite->getPosition());
//        tagSprite->addChild(sprite,1,newTag);
//        texture->release();
    }
}


HttpGetImg::~HttpGetImg(void)
{
    
}