#ifndef __HttpGetImg__
#define __HttpGetImg__
#include "web/IWeb.h"
#include "cocos2d.h"
#include "HttpRequest.h"
#include "HttpClient.h"
USING_NS_CC_EXT;
using namespace std;
USING_NS_CC;

class HttpGetImg : public cocos2d::CCLayer
{
public:
    static HttpGetImg* instance();
    HttpGetImg();
    ~HttpGetImg();
    //获取网络图片，需要地址
    void GetHttpImg(string imgurl,int userid);
    void setWebCallBack(IWeb *pWeb);
    void onHttpRequestRptImg(cocos2d::extension::CCHttpClient *sender, cocos2d::extension::CCHttpResponse *response);
    void showhead(float time);
    //按原来默认头像比例设置新图片大小
    static void setImgSize(int oldhead_w,int oldhead_h,CCSprite* sprite);
    
    //创建图片覆盖原有图片
    static void CreateImg(CCSprite* tagSprite,CCSprite* oldSprite,CCTexture2D* texture,int newTag);
private:
	IWeb *m_pWebCallBack;
	CCTexture2D* texture;
    CCSprite* m_Sprite;
    static HttpGetImg *m_pInstance;
    
};

#endif /* HttpGetImg */