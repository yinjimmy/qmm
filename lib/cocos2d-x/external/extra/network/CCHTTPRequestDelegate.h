
#ifndef __CC_EXTENSION_CCHTTP_REQUEST_DELEGATE_H_
#define __CC_EXTENSION_CCHTTP_REQUEST_DELEGATE_H_

#if (CC_CURL_ENABLED > 0)

#include "cocos2dx_extra.h"

NS_CC_EXTRA_BEGIN

class CCHTTPRequest;

class CCHTTPRequestDelegate
{
public:
    virtual void requestFinished(CCHTTPRequest* request) {}
    virtual void requestFailed(CCHTTPRequest* request) {}
};

NS_CC_EXTRA_END

#endif // (CC_CURL_ENABLED > 0)
#endif // __CC_EXTENSION_CCHTTP_REQUEST_DELEGATE_H_
