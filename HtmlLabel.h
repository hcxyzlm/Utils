#ifndef __HtmlLabel_h__
#define __HtmlLabel_h__
#include <iostream>
using namespace std;

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
using namespace cocos2d::ui;
#include "HtmlRichText.h"
#include "HtmlParser.h"

class HtmlLabel : public UILayout
{
private:
	HtmlLabel(CCSize labSize,string strHtmlText);
	virtual ~HtmlLabel();

public:
	static HtmlLabel* create(CCSize labSize,string strHtmlText);

	void onEnter();
	void onExit();

	void initUI();
private:
	void parseHtmlText(string strHtmlText);
	ccColor3B getColor(std::string str);
protected:
	HtmlRichText* m_htmlText;
	string m_strHtmlText;
	CCSize m_labSize;

	int m_nElementTag;
};

#endif