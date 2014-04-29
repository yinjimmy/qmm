--[[

Copyright (c) 2011-2014 chukong-inc.com

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

]]

--[[--

定义所有已经废弃的 API

]]

-- debug
echoLog   = DEPRECATED(printLog, "echoLog", "printLog")
echoError = DEPRECATED(printError, "echoError", "printError")
echoInfo  = DEPRECATED(printInfo, "echoInfo", "printInfo")

-- functions
tonum = DEPRECATED(checknumber, "tonum", "checknumber")
toint = DEPRECATED(checkint, "toint", "checkint")
tobool = DEPRECATED(checkbool, "tobool", "checkbool")
totable = DEPRECATED(checktable, "totable", "checktable")

table.insertTo = DEPRECATED(table.insertto, "table.insertTo", "table.insertto")
table.indexOf = DEPRECATED(table.indexof, "table.indexOf", "table.indexof")
table.keyOfItem = DEPRECATED(table.keyof, "table.keyOfItem", "table.keyof")
table.removeItem = DEPRECATED(table.removebyvalue, "table.removeItem", "table.removebyvalue")

string.htmlspecialcharsDecode = DEPRECATED(string.restorehtmlspecialchars, "string.htmlspecialcharsDecode", "string.restorehtmlspecialchars")
