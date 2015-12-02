//added by adolph start
Sprite* Sprite::createMaskedSprite(Sprite* src, Sprite* mask)
{
    Size size = Director::getInstance()->getWinSize();
    
    if (src == NULL || mask == NULL)
        return NULL;
    
    Size srcContent = src->getContentSize();
    Size maskContent = mask->getContentSize();
    
    RenderTexture * rt = RenderTexture::create(srcContent.width, srcContent.height);
    
    float x = srcContent.width * srcContent.width / size.width;
    float y = srcContent.height * srcContent.height / size.height;
    
    mask->setScaleX(srcContent.width / size.width);
    mask->setScaleY(srcContent.height / size.height);
    mask->setPosition(x / 2.f, y / 2.f);
    
    src->setScaleX(srcContent.width / size.width);
    src->setScaleY(srcContent.height / size.height);
    src->setPosition(x / 2.f, y / 2.f);
    
    BlendFunc blendFunc2;
    blendFunc2.src = GL_ONE;
    blendFunc2.dst = GL_ZERO;
    mask->setBlendFunc(blendFunc2);
    BlendFunc blendFunc3;
    blendFunc3.src = GL_DST_ALPHA;        
    // mask图片的当前alpha值是多少，如果是0（完全透明），那么就显示mask的。如果是1（完全不透明）
    blendFunc3.dst = GL_ZERO;                // maskSprite不可见
    src->setBlendFunc(blendFunc3);
    
    rt->begin();
    mask->visit();
    src->visit();
    rt->end();
    
//    rt->saveToFile("123.jpg" , Image::Format::JPG);
    
    Sprite * retval = Sprite::createWithTexture(rt->getSprite()->getTexture());
    retval->setFlippedY(true);
    return retval;
}
