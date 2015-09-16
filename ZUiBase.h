#ifndef __ZUiBase_h__
#define __ZUiBase_h__


#define IF_NULL_RETURN(pObj) if(pObj==NULL) return
#define IF_NULL_BREAK(pObj) if(pObj==NULL) break
#define IF_NULL_CONTINU(pObj) if(pObj==NULL) continue
#define IF_TRUE_RETURN(isTrue) if(isTrue) return
#define IF_TRUE_BREAK(isTrue) if(isTrue) break
#define IF_TRUE_CONTINU(isTrue) if(isTrue) continue

#define STATIC_CREATE_METHORD(T, METHOD) static T * METHOD() { \
    T * ptr = new T(); \
    if(ptr != NULL && ptr->init()) { \
        ptr->autorelease(); \
        return ptr; \
    } \
    CC_SAFE_DELETE(ptr); \
    return NULL; \
}

#define CCS_BUTTON_BIND(pUiBtn, pUiLayout, widgetName, pCCObject, touchSelector) pUiBtn = dynamic_cast<Button*>(UIHelper::seekWidgetByName(pUiLayout, widgetName)); \
	pUiBtn->setTouchEnabled(true); \
	pUiBtn->addTouchEventListener(pCCObject, toucheventselector(touchSelector));

#define CCS_IMAGE_BUTTON_BIND(pUiBtn, pUiLayout, widgetName, pCCObject, touchSelector) pUiBtn = dynamic_cast<ImageView*>(UIHelper::seekWidgetByName(pUiLayout, widgetName)); \
	pUiBtn->setTouchEnabled(true); \
	pUiBtn->addTouchEventListener(pCCObject, toucheventselector(touchSelector));

#define CCS_LAYOUT_BUTTON_BIND(pUiBtn, pUiLayout, widgetName, pCCObject, touchSelector) pUiBtn = dynamic_cast<Layout*>(UIHelper::seekWidgetByName(pUiLayout, widgetName)); \
	pUiBtn->setTouchEnabled(true); \
	pUiBtn->addTouchEventListener(pCCObject, toucheventselector(touchSelector));

#define CCS_CHECKBOX_BIND(pUiCheck, pUiLayout, widgetName, pCCObject, touchSelector) pUiCheck = dynamic_cast<CheckBox*>(UIHelper::seekWidgetByName(pUiLayout, widgetName)); \
	pUiCheck->setTouchEnabled(true); \
	pUiCheck->addEventListenerCheckBox(pCCObject, checkboxselectedeventselector(touchSelector));

#define CCS_SLIDER_BIND(pSlider, pLayout, widgetName, pObj, touchSelector) pSlider = dynamic_cast<Slider*>(UIHelper::seekWidgetByName(pLayout, widgetName)); \
	pSlider->setTouchEnabled(true); \
	pSlider->addEventListenerSlider(this, sliderpercentchangedselector(touchSelector));
	
#define CCS_IMAGE_BIND(pUiImg, pUiLayout, widgetName) pUiImg = dynamic_cast<ImageView*>(UIHelper::seekWidgetByName(pUiLayout, widgetName))
#define CCS_LABEL_BIND(pUiLab, pUiLayout, widgetName) pUiLab = dynamic_cast<Label*>(UIHelper::seekWidgetByName(pUiLayout, widgetName))
#define CCS_ATLASLABEL_BIND(pUiLab, pUiLayout, widgetName) pUiLab = dynamic_cast<UILabelAtlas*>(UIHelper::seekWidgetByName(pUiLayout, widgetName))
#define CCS_SCROLLVIEW_BIND(pScroll, pUiLayout, widgetName) pScroll = dynamic_cast<ScrollView*>(UIHelper::seekWidgetByName(pUiLayout, widgetName))
#define CCS_LAYOUT_BIND(pLayout, pSrcLayout, widgetName) pLayout = dynamic_cast<Layout*>(UIHelper::seekWidgetByName(pSrcLayout, widgetName))
#define CCS_TEXTFIELD_BIND(pTextField, pLayout, widgetName) pTextField = dynamic_cast<TextField*>(UIHelper::seekWidgetByName(pLayout, widgetName))
#define CCS_LOADINGBAR_BIND(pTextField, pLayout, widgetName) pTextField = dynamic_cast<LoadingBar*>(UIHelper::seekWidgetByName(pLayout, widgetName))

#endif