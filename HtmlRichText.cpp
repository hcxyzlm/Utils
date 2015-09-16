/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HtmlRichText.h"
#include "support/ccUTF8.h"

	static std::string utf8_substr(const std::string& str, unsigned long start, unsigned long leng)
	{
		if (leng==0)
		{
			return "";
		}
		unsigned long c, i, ix, q, min=std::string::npos, max=std::string::npos;
		for (q=0, i=0, ix=str.length(); i < ix; i++, q++)
		{
			if (q==start)
			{
				min = i;
			}
			if (q <= start+leng || leng==std::string::npos)
			{
				max = i;
			}

			c = (unsigned char) str[i];

			if      (c<=127) i+=0;
			else if ((c & 0xE0) == 0xC0) i+=1;
			else if ((c & 0xF0) == 0xE0) i+=2;
			else if ((c & 0xF8) == 0xF0) i+=3;
			else return "";//invalid utf8
		}
		if (q <= start+leng || leng == std::string::npos)
		{
			max = i;
		}
		if (min==std::string::npos || max==std::string::npos)
		{
			return "";
		}
		return str.substr(min,max);
	}
    
bool HtmlRichElement::init(int tag, const ccColor3B &color, GLubyte opacity)
{
    _tag = tag;
    _color = color;
    _opacity = opacity;
    return true;
}
    
    
HtmlRichElementText* HtmlRichElementText::create(int tag, const ccColor3B &color, GLubyte opacity, const char *text, const char *fontName, float fontSize)
{
    HtmlRichElementText* element = new HtmlRichElementText();
    if (element && element->init(tag, color, opacity, text, fontName, fontSize))
    {
        element->autorelease();
        return element;
    }
    CC_SAFE_DELETE(element);
    return NULL;
}
    
bool HtmlRichElementText::init(int tag, const ccColor3B &color, GLubyte opacity, const char *text, const char *fontName, float fontSize)
{
    if (HtmlRichElement::init(tag, color, opacity))
    {
        _text = text;
        _fontName = fontName;
        _fontSize = fontSize;
        return true;
    }
    return false;
}

HtmlRichElementImage* HtmlRichElementImage::create(int tag, const ccColor3B &color, GLubyte opacity, const char *filePath)
{
    HtmlRichElementImage* element = new HtmlRichElementImage();
    if (element && element->init(tag, color, opacity, filePath))
    {
        element->autorelease();
        return element;
    }
    CC_SAFE_DELETE(element);
    return NULL;
}
    
bool HtmlRichElementImage::init(int tag, const ccColor3B &color, GLubyte opacity, const char *filePath)
{
    if (HtmlRichElement::init(tag, color, opacity))
    {
        _filePath = filePath;
        return true;
    }
    return false;
}

HtmlRichElementCustomNode* HtmlRichElementCustomNode::create(int tag, const ccColor3B &color, GLubyte opacity, cocos2d::CCNode *customNode)
{
    HtmlRichElementCustomNode* element = new HtmlRichElementCustomNode();
    if (element && element->init(tag, color, opacity, customNode))
    {
        element->autorelease();
        return element;
    }
    CC_SAFE_DELETE(element);
    return NULL;
}
    
bool HtmlRichElementCustomNode::init(int tag, const ccColor3B &color, GLubyte opacity, cocos2d::CCNode *customNode)
{
    if (HtmlRichElement::init(tag, color, opacity))
    {
        _customNode = customNode;
        _customNode->retain();
        return true;
    }
    return false;
}
    
HtmlRichText::HtmlRichText():
_formatTextDirty(true),
_HtmlRichElements(NULL),
_leftSpaceWidth(0.0f),
_verticalSpace(0.0f),
_elementRenderersContainer(NULL)
{
    
}
    
HtmlRichText::~HtmlRichText()
{
    _HtmlRichElements->release();
}
    
HtmlRichText* HtmlRichText::create()
{
    HtmlRichText* widget = new HtmlRichText();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}
    
bool HtmlRichText::init()
{
    if (Widget::init())
    {
        _HtmlRichElements = CCArray::create();
        _HtmlRichElements->retain();
        return true;
    }
    return false;
}
    
void HtmlRichText::initRenderer()
{
    _elementRenderersContainer = CCNode::create();
    _elementRenderersContainer->setAnchorPoint(ccp(0.5f, 0.5f));
    CCNode::addChild(_elementRenderersContainer, 0, -1);
}

void HtmlRichText::insertElement(HtmlRichElement *element, int index)
{
    _HtmlRichElements->insertObject(element, index);
    _formatTextDirty = true;
}
    
void HtmlRichText::pushBackElement(HtmlRichElement *element)
{
    _HtmlRichElements->addObject(element);
    _formatTextDirty = true;
}
    
void HtmlRichText::removeElement(int index)
{
    _HtmlRichElements->removeObjectAtIndex(index);
    _formatTextDirty = true;
}
    
void HtmlRichText::removeElement(HtmlRichElement *element)
{
    _HtmlRichElements->removeObject(element);
    _formatTextDirty = true;
}
    
void HtmlRichText::formatText()
{
    if (_formatTextDirty)
    {
        _elementRenderersContainer->removeAllChildren();
        _elementRenders.clear();
        if (_ignoreSize)
        {
            addNewLine();
            for (unsigned int i=0; i<_HtmlRichElements->count(); i++)
            {
                HtmlRichElement* element = static_cast<HtmlRichElement*>(_HtmlRichElements->objectAtIndex(i));
                CCNode* elementRenderer = NULL;
                switch (element->_type)
                {
                    case HtmlRich_TEXT:
                    {
                        HtmlRichElementText* elmtText = static_cast<HtmlRichElementText*>(element);
                        elementRenderer = CCLabelTTF::create(elmtText->_text.c_str(), elmtText->_fontName.c_str(), elmtText->_fontSize);
                        break;
                    }
                    case HtmlRich_IMAGE:
                    {
                        HtmlRichElementImage* elmtImage = static_cast<HtmlRichElementImage*>(element);
                        elementRenderer = CCSprite::create(elmtImage->_filePath.c_str());
                        break;
                    }
                    case HtmlRich_CUSTOM:
                    {
                        HtmlRichElementCustomNode* elmtCustom = static_cast<HtmlRichElementCustomNode*>(element);
                        elementRenderer = elmtCustom->_customNode;
                        break;
                    }
                    default:
                        break;
                }
                CCRGBAProtocol* colorRenderer = dynamic_cast<CCRGBAProtocol*>(elementRenderer);
                colorRenderer->setColor(element->_color);
                colorRenderer->setOpacity(element->_opacity);
                pushToContainer(elementRenderer);
            }
        }
        else
        {
            addNewLine();
            for (unsigned int i=0; i<_HtmlRichElements->count(); i++)
            {
                
                HtmlRichElement* element = static_cast<HtmlRichElement*>(_HtmlRichElements->objectAtIndex(i));
                switch (element->_type)
                {
                    case HtmlRich_TEXT:
                    {
                        HtmlRichElementText* elmtText = static_cast<HtmlRichElementText*>(element);
                        handleTextRenderer(elmtText->_text.c_str(), elmtText->_fontName.c_str(), elmtText->_fontSize, elmtText->_color, elmtText->_opacity);
                        break;
                    }
                    case HtmlRich_IMAGE:
                    {
                        HtmlRichElementImage* elmtImage = static_cast<HtmlRichElementImage*>(element);
                        handleImageRenderer(elmtImage->_filePath.c_str(), elmtImage->_color, elmtImage->_opacity);
                        break;
                    }
                    case HtmlRich_CUSTOM:
                    {
                        HtmlRichElementCustomNode* elmtCustom = static_cast<HtmlRichElementCustomNode*>(element);
                        handleCustomRenderer(elmtCustom->_customNode);
                        break;
                    }
					case HtmlRich_ADD_LINE:
					{
						HtmlRichElementAddLine *elmAddLine = static_cast<HtmlRichElementAddLine*>(element);
						handleAddLineRenderer();
						break;
					}
                    default:
                        break;
                }
            }
        }
        formarRenderers();
        _formatTextDirty = false;
    }
}
    
void HtmlRichText::handleTextRenderer(const char *text, const char *fontName, float fontSize, const ccColor3B &color, GLubyte opacity)
{
    CCLabelTTF* textRenderer = CCLabelTTF::create(text, fontName, fontSize);
    float textRendererWidth = textRenderer->getContentSize().width;
    _leftSpaceWidth -= textRendererWidth;
    if (_leftSpaceWidth < 0.0f)
    {
        float overstepPercent = (-_leftSpaceWidth) / textRendererWidth;
        std::string curText = text;
        size_t stringLength = cc_utf8_strlen(text,-1);
        int leftLength = stringLength * (1.0f - overstepPercent);
        std::string leftWords = utf8_substr(curText,0,leftLength);
        std::string cutWords = utf8_substr(curText, leftLength, curText.length() - leftLength);
        if (leftLength > 0)
        {
            CCLabelTTF* leftRenderer = CCLabelTTF::create(utf8_substr(leftWords, 0, leftLength).c_str(), fontName, fontSize);
            if (leftRenderer)
            {
                leftRenderer->setColor(color);
                leftRenderer->setOpacity(opacity);
                pushToContainer(leftRenderer);
            }
        }

        addNewLine();
        handleTextRenderer(cutWords.c_str(), fontName, fontSize, color, opacity);
    }
    else
    {
        textRenderer->setColor(color);
        textRenderer->setOpacity(opacity);
        pushToContainer(textRenderer);
    }
}
    
void HtmlRichText::handleImageRenderer(const char *fileParh, const ccColor3B &color, GLubyte opacity)
{
    CCSprite* imageRenderer = CCSprite::create(fileParh);
    handleCustomRenderer(imageRenderer);
}
    
void HtmlRichText::handleCustomRenderer(cocos2d::CCNode *renderer)
{
    CCSize imgSize = renderer->getContentSize();
    _leftSpaceWidth -= imgSize.width;
    if (_leftSpaceWidth < 0.0f)
    {
        addNewLine();
        pushToContainer(renderer);
        _leftSpaceWidth -= imgSize.width;
    }
    else
    {
        pushToContainer(renderer);
    }
}
    
void HtmlRichText::addNewLine()
{
    _leftSpaceWidth = _customSize.width;
    _elementRenders.push_back(CCArray::create());
}
    
void HtmlRichText::formarRenderers()
{
    if (_ignoreSize)
    {
        float newContentSizeWidth = 0.0f;
        float newContentSizeHeight = 0.0f;
        
        CCArray* row = (CCArray*)(_elementRenders[0]);
        float nextPosX = 0.0f;
        for (unsigned int j=0; j<row->count(); j++)
        {
            CCNode* l = (CCNode*)(row->objectAtIndex(j));
            l->setAnchorPoint(CCPointZero);
            l->setPosition(ccp(nextPosX, 0.0f));
            _elementRenderersContainer->addChild(l, 1, j);
            CCSize iSize = l->getContentSize();
            newContentSizeWidth += iSize.width;
            newContentSizeHeight = MAX(newContentSizeHeight, iSize.height);
            nextPosX += iSize.width;
        }
        _elementRenderersContainer->setContentSize(CCSizeMake(newContentSizeWidth, newContentSizeHeight));
    }
    else
    {
        float newContentSizeHeight = 0.0f;
        float *maxHeights = new float[_elementRenders.size()];
        
        for (unsigned int i=0; i<_elementRenders.size(); i++)
        {
            CCArray* row = (CCArray*)(_elementRenders[i]);
            float maxHeight = 0.0f;
            for (unsigned int j=0; j<row->count(); j++)
            {
                CCNode* l = (CCNode*)(row->objectAtIndex(j));
                maxHeight = MAX(l->getContentSize().height, maxHeight);
            }
            maxHeights[i] = maxHeight;
            newContentSizeHeight += maxHeights[i];
        }
        
        
        float nextPosY = _customSize.height;
        for (unsigned int i=0; i<_elementRenders.size(); i++)
        {
            CCArray* row = (CCArray*)(_elementRenders[i]);
            float nextPosX = 0.0f;
            nextPosY -= (maxHeights[i] + _verticalSpace);
            
            for (unsigned int j=0; j<row->count(); j++)
            {
                CCNode* l = (CCNode*)(row->objectAtIndex(j));
                l->setAnchorPoint(CCPointZero);
                l->setPosition(ccp(nextPosX, nextPosY));
                _elementRenderersContainer->addChild(l, 1, i*10 + j);
                nextPosX += l->getContentSize().width;
            }
        }
        _elementRenderersContainer->setContentSize(_size);
        delete [] maxHeights;
    }
    _elementRenders.clear();
    if (_ignoreSize)
    {
        CCSize s = getContentSize();
        _size = s;
    }
    else
    {
        _size = _customSize;
    }
}
    
void HtmlRichText::pushToContainer(cocos2d::CCNode *renderer)
{
    if (_elementRenders.size() <= 0)
    {
        return;
    }
    _elementRenders[_elementRenders.size()-1]->addObject(renderer);
}

void HtmlRichText::visit()
{
    if (_enabled)
    {
        formatText();
        Widget::visit();
    }
}
    
void HtmlRichText::setVerticalSpace(float space)
{
    _verticalSpace = space;
}
    
void HtmlRichText::setAnchorPoint(const CCPoint &pt)
{
    Widget::setAnchorPoint(pt);
    _elementRenderersContainer->setAnchorPoint(pt);
}
    
const CCSize& HtmlRichText::getContentSize() const
{
    return _elementRenderersContainer->getContentSize();
}
    
void HtmlRichText::ignoreContentAdaptWithSize(bool ignore)
{
    if (_ignoreSize != ignore)
    {
        _formatTextDirty = true;
        Widget::ignoreContentAdaptWithSize(ignore);
    }
}
    
std::string HtmlRichText::getDescription() const
{
    return "HtmlRichText";
}

HtmlRichElementAddLine* cocos2d::HtmlRichElementAddLine::create()
{
	HtmlRichElementAddLine* element = new HtmlRichElementAddLine();
	if (element)
	{
		element->autorelease();
		return element;
	}
	CC_SAFE_DELETE(element);
	return NULL;
}

void cocos2d::HtmlRichText::handleAddLineRenderer()
{
	CCLabelTTF* spaceRenderer = CCLabelTTF::create(" ","Helvetica", 20);
	float textRendererWidth = spaceRenderer->getContentSize().width;
	_leftSpaceWidth -= textRendererWidth;
	if (_leftSpaceWidth > 0.0f)
	{
		pushToContainer(spaceRenderer);
		handleAddLineRenderer();
	}
	else
	{
		addNewLine();
	}
}