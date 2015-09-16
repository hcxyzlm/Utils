#include "HtmlLabel.h"
#include "utils/LangUtil.h"

using namespace std;

HtmlLabel::HtmlLabel(CCSize labSize,string strHtmlText)
{
	m_strHtmlText = strHtmlText;
	m_labSize = labSize;
	m_nElementTag = 1;
}

HtmlLabel::~HtmlLabel()
{
}

HtmlLabel* HtmlLabel::create(CCSize labSize,string strHtmlText)
{
	HtmlLabel *pHtmlLabel = new HtmlLabel(labSize,strHtmlText);
	pHtmlLabel->init();
	pHtmlLabel->autorelease();
	pHtmlLabel->initUI();
	return pHtmlLabel;
}

void HtmlLabel::onEnter()
{
	UILayout::onEnter();
}

void HtmlLabel::onExit()
{
	UILayout::onExit();
}

void HtmlLabel::initUI()
{
	m_htmlText = HtmlRichText::create();
	m_htmlText->ignoreContentAdaptWithSize(false);
	m_htmlText->setSize(m_labSize);
	this->setSize(m_labSize);

	parseHtmlText(m_strHtmlText);
	addChild(m_htmlText);
}

void HtmlLabel::parseHtmlText(string strHtmlText)
{
	using namespace liigo;

	 HtmlParser htmlParser;
	 htmlParser.parseHtml(strHtmlText.c_str()); 

	 for(int index = 0, count = htmlParser.getHtmlNodeCount(); index < count; index++) 
	 {
		  HtmlNode* pNode = htmlParser.getHtmlNode(index);

		  if (pNode->type == NODE_START_TAG)
		  {
			  switch (pNode->tagType)
			  {
			  case TAG_FONT:
				  {
					  ccColor3B contentColor = ccBLACK;
					  int nSize = 20;
					  if(pNode->attributeCount>0) 
					  { 
						  string strColor=HtmlParser::getAttributeStringValue(pNode,"color","#000000");
						  string strSize = HtmlParser::getAttributeStringValue(pNode,"size","20");

						  if(strColor!="") 
						  { 
								contentColor = getColor(strColor);
						  } 
						  if (strSize!= "")
						  {
							  nSize = atoi(strSize.c_str());
						  }
					  } 

					  index++; 
					  liigo::HtmlNode* nextNode = htmlParser.getHtmlNode(index); 
					  if(nextNode->type==NODE_CONTENT) 
					  { 
						  std::string content=nextNode->text; 
						  HtmlRichElementText* htElem = HtmlRichElementText::create(m_nElementTag, contentColor, 255,content.c_str(), "Helvetica", nSize);
						  m_htmlText->pushBackElement(htElem);
						  m_nElementTag++;
					  } 
					   break;
				  } 
			  case TAG_BR:
				  {
					  HtmlRichElementAddLine* htElem = HtmlRichElementAddLine::create();
					  m_htmlText->pushBackElement(htElem);
					  break;
				  }
			  case TAG_A:
				  {
					  break;
				  }
			  default:
				  break;
			  }
		  }

		  if (pNode->type == NODE_CONTENT)
		  {
			   ccColor3B contentColor = ccBLACK;
			   int nSize = 20;
			   
			   std::string content=pNode->text; 
			   HtmlRichElementText* htElem = HtmlRichElementText::create(m_nElementTag, contentColor, 255,content.c_str(), "Helvetica", nSize);
			   m_htmlText->pushBackElement(htElem);
			   m_nElementTag++;
			 
		  }
		  if (pNode->type == NODE_END_TAG)
		  {

		  }
	 }
}

ccColor3B HtmlLabel::getColor(std::string str) 
{ 
	int r,g,b; 
	sscanf(str.substr(1,2).c_str(),"%x",&r); 
	sscanf(str.substr(3,2).c_str(),"%x",&g); 
	sscanf(str.substr(5,2).c_str(),"%x",&b); 
	return ccc3(r,g,b); 
} 