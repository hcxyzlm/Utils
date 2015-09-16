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

#ifndef __UIHtmlRichTEXT_H__
#define __UIHtmlRichTEXT_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
using namespace cocos2d::ui;

NS_CC_BEGIN

/*add By Lee*/    
typedef enum {
    HtmlRich_TEXT,
    HtmlRich_IMAGE,
    HtmlRich_CUSTOM,
	HtmlRich_ADD_LINE
}HtmlRichElementType;
    
class HtmlRichElement : public CCObject
{
public:
    HtmlRichElement(){};
    virtual ~HtmlRichElement(){};
    virtual bool init(int tag, const ccColor3B& color, GLubyte opacity);
protected:
    HtmlRichElementType _type;
    int _tag;
    ccColor3B _color;
    GLubyte _opacity;
    friend class HtmlRichText;
};
    
class HtmlRichElementText : public HtmlRichElement
{
public:
    HtmlRichElementText(){_type = HtmlRich_TEXT;};
    virtual ~HtmlRichElementText(){};
    virtual bool init(int tag, const ccColor3B& color, GLubyte opacity, const char* text, const char* fontName, float fontSize);
    static HtmlRichElementText* create(int tag, const ccColor3B& color, GLubyte opacity, const char* text, const char* fontName, float fontSize);
protected:
    std::string _text;
    std::string _fontName;
    float _fontSize;
    friend class HtmlRichText;
    
};

class HtmlRichElementAddLine : public HtmlRichElement
{
public:
	HtmlRichElementAddLine(){_type = HtmlRich_ADD_LINE;};
	virtual ~HtmlRichElementAddLine(){};
	static HtmlRichElementAddLine* create();
protected:
	friend class HtmlRichText;

};
    
class HtmlRichElementImage : public HtmlRichElement
{
public:
    HtmlRichElementImage(){_type = HtmlRich_IMAGE;};
    virtual ~HtmlRichElementImage(){};
    virtual bool init(int tag, const ccColor3B& color, GLubyte opacity, const char* filePath);
    static HtmlRichElementImage* create(int tag, const ccColor3B& color, GLubyte opacity, const char* filePath);
protected:
    std::string _filePath;
    CCRect _textureRect;
    int _textureType;
    friend class HtmlRichText;
};
    
class HtmlRichElementCustomNode : public HtmlRichElement
{
public:
    HtmlRichElementCustomNode(){_type = HtmlRich_CUSTOM;};
    virtual ~HtmlRichElementCustomNode(){CC_SAFE_RELEASE(_customNode);};
    virtual bool init(int tag, const ccColor3B& color, GLubyte opacity, CCNode* customNode);
    static HtmlRichElementCustomNode* create(int tag, const ccColor3B& color, GLubyte opacity, CCNode* customNode);
protected:
    CCNode* _customNode;
    friend class HtmlRichText;
};
    
class HtmlRichText : public Widget
{
public:
    HtmlRichText();
    virtual ~HtmlRichText();
    static HtmlRichText* create();
    void insertElement(HtmlRichElement* element, int index);
    void pushBackElement(HtmlRichElement* element);
    void removeElement(int index);
    void removeElement(HtmlRichElement* element);
    virtual void visit();
    void setVerticalSpace(float space);
    virtual void setAnchorPoint(const CCPoint &pt);
    virtual const CCSize& getContentSize() const;
    void formatText();
    virtual void ignoreContentAdaptWithSize(bool ignore);
    virtual std::string getDescription() const;
protected:
    virtual bool init();
    virtual void initRenderer();
    void pushToContainer(CCNode* renderer);
    void handleTextRenderer(const char* text, const char* fontName, float fontSize, const ccColor3B& color, GLubyte opacity);
    void handleImageRenderer(const char* fileParh, const ccColor3B& color, GLubyte opacity);
    void handleCustomRenderer(CCNode* renderer);
	void handleAddLineRenderer();
    void formarRenderers();
    void addNewLine();
protected:
    bool _formatTextDirty;
    CCArray* _HtmlRichElements;
    std::vector<CCArray*> _elementRenders;
    float _leftSpaceWidth;
    float _verticalSpace;
    CCNode* _elementRenderersContainer;
};
    

NS_CC_END

#endif /* defined(__UIHtmlRichText__) */
