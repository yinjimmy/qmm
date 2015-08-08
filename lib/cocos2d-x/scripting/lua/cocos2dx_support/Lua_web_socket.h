#ifndef __LUA_WEB_SOCKET_H__
#define __LUA_WEB_SOCKET_H__

#if (CC_WEBSOCKET_ENABLED > 0)

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

TOLUA_API int tolua_web_socket_open(lua_State* tolua_S);

#endif // (CC_WEBSOCKET_ENABLED > 0)

#endif //__LUA_WEB_SOCKET_H__
