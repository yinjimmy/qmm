
/* lanes_scripts.c */

#include "lua.h"
#include "lauxlib.h"
#include "lanes_scripts.h"

/* lanes, lanes.lua */
static const unsigned char lua_m_lanes[] = {
27,76,74,1,2,64,0,1,3,1,1,0,12,43,1,0,0,16,2,0,0,62,1,2,2,7,1,0,0,84,1,3,128,39,1,0,0,0,1,0,0,84,1,2,128,41,1,1,0,84,2,1,128,41,1,2,0,72,1,2,0,4,192,11,110,117,109,98,101,114,77,0,1,3,1,1,0,15,15,0,0,0,84,1,10,128,43,1,0,0,16,2,0,0,62,1,2,2,6,1,0,0,84,1,2,128,41,1,1,0,84,2,1,128,41,1,2,0,72,1,2,0,84,1,2,128,41,1,2,0,72,1,2,0,71,0,1,0,4,192,12,98,111,111,108,101,97,110,77,0,1,3,1,1,0,15,15,0,0,0,84,1,10,128,43,1,0,0,16,2,0,0,62,1,2,2,6,1,0,0,84,1,2,128,41,1,1,0,84,2,1,128,41,1,2,0,72,1,2,0,84,1,2,128,41,1,2,0,72,1,2,0,71,0,1,0,4,192,12,98,111,111,108,101,97,110,70,0,1,3,1,1,0,13,15,0,0,0,84,1,5,128,43,1,0,0,16,2,0,0,62,1,2,2,6,1,0,0,84,1,4,128,41,1,2,0,84,2,3,128,41,1,1,0,84,2,1,128,41,1,2,0,72,1,2,0,4,192,13,102,117,110,99,116,105,111,110,64,0,1,3,1,1,0,12,43,1,0,0,16,2,0,0,62,1,2,2,7,1,0,0,84,1,3,128,39,1,0,0,2,1,0,0,84,1,2,128,41,1,1,0,84,2,1,128,41,1,2,0,72,1,2,0,4,192,11,110,117,109,98,101,114,69,0,1,3,1,1,0,13,15,0,0,0,84,1,5,128,43,1,0,0,16,2,0,0,62,1,2,2,6,1,0,0,84,1,4,128,41,1,2,0,84,2,3,128,41,1,1,0,84,2,1,128,41,1,2,0,72,1,2,0,4,192,12,98,111,111,108,101,97,110,77,0,1,3,1,1,0,15,15,0,0,0,84,1,10,128,43,1,0,0,16,2,0,0,62,1,2,2,6,1,0,0,84,1,2,128,41,1,1,0,84,2,1,128,41,1,2,0,72,1,2,0,84,1,2,128,41,1,2,0,72,1,2,0,71,0,1,0,4,192,12,98,111,111,108,101,97,110,138,2,0,1,14,6,4,0,44,14,0,0,0,84,1,2,128,43,1,0,0,72,1,2,0,43,1,1,0,16,2,0,0,62,1,2,2,6,1,0,0,84,1,3,128,43,1,2,0,37,2,1,0,62,1,2,1,43,1,3,0,43,2,0,0,62,1,2,4,68,4,25,128,54,6,4,0,41,7,0,0,10,6,0,0,84,8,2,128,16,7,6,0,84,8,2,128,43,8,0,0,54,7,4,8,43,8,4,0,54,8,4,8,16,9,7,0,62,8,2,2,14,0,8,0,84,8,10,128,43,8,2,0,37,9,2,0,16,10,4,0,37,11,3,0,43,12,5,0,16,13,7,0,62,12,2,2,36,9,12,9,39,10,2,0,62,8,3,1,57,7,4,0,66,4,3,3,78,4,229,127,72,0,2,0,8,192,4,192,7,192,5,192,9,192,6,192,7,58,32,9,66,97,100,32,61,66,97,100,32,112,97,114,97,109,101,116,101,114,32,35,49,32,116,111,32,108,97,110,101,115,46,99,111,110,102,105,103,117,114,101,40,41,44,32,115,104,111,117,108,100,32,98,101,32,97,32,116,97,98,108,101,10,116,97,98,108,101,61,0,1,5,0,4,0,9,52,1,0,0,55,1,1,1,16,2,1,0,55,1,2,1,16,3,0,0,37,4,3,0,36,3,4,3,62,1,3,1,71,0,1,0,6,10,10,119,114,105,116,101,11,115,116,100,101,114,114,7,105,111,106,0,1,12,3,3,0,22,14,0,0,0,84,1,1,128,71,0,1,0,37,1,0,0,43,2,0,0,16,3,0,0,62,2,2,4,68,5,8,128,16,7,1,0,16,8,5,0,37,9,1,0,43,10,1,0,16,11,6,0,62,10,2,2,37,11,2,0,36,1,11,7,66,5,3,3,78,5,246,127,43,2,2,0,16,3,1,0,62,2,2,1,71,0,1,0,5,192,6,192,15,192,6,10,6,61,5,89,2,0,10,9,1,0,12,43,0,0,0,43,1,1,0,43,2,2,0,43,3,3,0,55,3,0,3,43,4,4,0,43,5,5,0,43,6,6,0,43,7,7,0,43,8,8,0,67,9,0,0,63,0,8,0,7,0,4,192,1,128,8,0,7,128,6,128,8,128,9,128,10,128,20,111,110,95,115,116,97,116,101,95,99,114,101,97,116,101,217,9,3,0,20,9,25,2,220,1,50,0,0,0,41,1,0,0,39,2,2,0,43,3,0,0,37,4,0,0,67,5,0,0,61,3,1,2,9,3,0,0,84,4,3,128,43,4,1,0,37,5,1,0,62,4,2,1,39,4,1,0,21,5,1,3,39,6,1,0,73,4,43,128,43,8,0,0,16,9,7,0,67,10,0,0,61,8,1,2,43,9,2,0,16,10,8,0,62,9,2,2,7,9,2,0,84,9,10,128,15,0,1,0,84,9,6,128,16,9,1,0,37,10,3,0,16,11,8,0,36,9,11,9,12,1,9,0,84,10,1,128,16,1,8,0,84,9,23,128,43,9,2,0,16,10,8,0,62,9,2,2,7,9,4,0,84,9,8,128,43,9,3,0,16,10,8,0,62,9,2,4,68,12,1,128,57,13,12,0,66,12,3,3,78,12,253,127,84,9,10,128,11,8,0,0,84,9,1,128,84,9,7,128,43,9,1,0,37,10,5,0,43,11,4,0,16,12,8,0,62,11,2,2,36,10,11,10,62,9,2,1,75,4,213,127,43,4,0,0,16,5,3,0,67,6,0,0,61,4,1,2,43,5,2,0,16,6,4,0,62,5,2,2,6,5,6,0,84,6,9,128,6,5,2,0,84,6,7,128,43,6,1,0,37,7,7,0,43,8,4,0,16,9,4,0,62,8,2,2,36,7,8,7,62,6,2,1,15,0,1,0,84,6,36,128,6,1,8,0,84,6,34,128,50,6,0,0,43,7,5,0,16,8,1,0,37,9,9,0,62,7,3,4,84,10,26,128,43,11,6,0,54,11,10,11,14,0,11,0,84,11,6,128,43,11,1,0,37,12,10,0,16,13,10,0,36,12,13,12,62,11,2,1,84,11,16,128,54,11,10,6,14,0,11,0,84,12,1,128,39,11,0,0,20,11,1,11,57,11,10,6,54,11,10,6,39,12,1,0,1,12,11,0,84,11,6,128,43,11,1,0,37,12,11,0,16,13,10,0,37,14,12,0,36,12,14,12,62,11,2,1,65,10,3,2,78,10,228,127,42,6,10,0,43,11,3,0,16,12,0,0,62,11,2,4,68,14,92,128,7,14,13,0,84,16,2,128,16,6,15,0,84,16,88,128,7,14,14,0,84,16,25,128,11,15,2,0,84,16,2,128,39,7,100,0,84,16,20,128,11,15,1,0,84,16,2,128,39,7,0,0,84,16,16,128,43,16,2,0,16,17,15,0,62,16,2,2,7,16,15,0,84,16,2,128,12,7,15,0,84,16,9,128,43,16,1,0,37,17,16,0,43,18,4,0,16,19,15,0,62,18,2,2,36,17,18,17,16,18,2,0,62,16,3,2,16,7,16,0,84,16,61,128,7,14,17,0,84,16,2,128,16,8,15,0,84,16,57,128,7,14,18,0,84,16,17,128,43,16,2,0,16,17,15,0,62,16,2,2,7,16,4,0,84,16,2,128,12,9,15,0,84,16,9,128,43,16,1,0,37,17,19,0,43,18,4,0,16,19,15,0,62,18,2,2,36,17,18,17,16,18,2,0,62,16,3,2,16,9,16,0,84,16,38,128,7,14,20,0,84,16,17,128,43,16,2,0,16,17,15,0,62,16,2,2,7,16,4,0,84,16,2,128,12,10,15,0,84,16,9,128,43,16,1,0,37,17,21,0,43,18,2,0,16,19,15,0,62,18,2,2,36,17,18,17,16,18,2,0,62,16,3,2,16,10,16,0,84,16,19,128,9,14,1,0,84,16,9,128,43,16,1,0,37,17,22,0,43,18,4,0,16,19,15,0,62,18,2,2,36,17,18,17,16,18,2,0,62,16,3,1,84,16,8,128,43,16,1,0,37,17,23,0,43,18,4,0,16,19,14,0,62,18,2,2,36,17,18,17,16,18,2,0,62,16,3,1,66,14,3,3,78,14,162,127,14,0,9,0,84,11,1,128,52,9,18,0,49,11,24,0,48,0,0,128,72,11,2,0,3,192,7,192,4,192,5,192,6,192,2,192,17,192,12,192,0,128,0,17,66,97,100,32,111,112,116,105,111,110,58,32,21,117,110,107,101,121,101,100,32,111,112,116,105,111,110,58,32,49,66,97,100,32,39,114,101,113,117,105,114,101,100,39,32,111,112,116,105,111,110,58,32,101,120,112,101,99,116,105,110,103,32,116,97,98,108,101,44,32,103,111,116,32,13,114,101,113,117,105,114,101,100,18,66,97,100,32,112,97,99,107,97,103,101,58,32,12,112,97,99,107,97,103,101,12,103,108,111,98,97,108,115,21,66,97,100,32,99,97,110,99,101,108,115,116,101,112,58,32,11,110,117,109,98,101,114,15,99,97,110,99,101,108,115,116,101,112,13,112,114,105,111,114,105,116,121,21,39,32,109,111,114,101,32,116,104,97,110,32,111,110,99,101,34,108,105,98,115,32,115,112,101,99,105,102,105,99,97,116,105,111,110,32,99,111,110,116,97,105,110,115,32,39,23,66,97,100,32,108,105,98,114,97,114,121,32,110,97,109,101,58,32,13,91,37,97,37,100,46,93,43,6,42,44,76,97,115,116,32,112,97,114,97,109,101,116,101,114,32,110,111,116,32,102,117,110,99,116,105,111,110,32,111,114,32,115,116,114,105,110,103,58,32,13,102,117,110,99,116,105,111,110,20,66,97,100,32,112,97,114,97,109,101,116,101,114,58,32,10,116,97,98,108,101,6,44,11,115,116,114,105,110,103,19,78,111,32,112,97,114,97,109,101,116,101,114,115,33,6,35,0,2,47,0,0,2,1,1,0,4,43,0,0,0,37,1,0,0,62,0,2,1,71,0,1,0,7,192,26,116,105,109,101,114,115,32,97,114,101,32,110,111,116,32,97,99,116,105,118,101,109,0,0,15,3,0,0,24,50,0,0,0,43,1,0,0,43,2,1,0,62,1,2,4,68,4,16,128,54,6,4,5,43,7,0,0,16,8,5,0,62,7,2,4,68,10,9,128,4,10,4,0,84,12,7,128,43,12,2,0,16,13,0,0,50,14,4,0,59,6,1,14,59,10,2,14,59,11,3,14,62,12,3,1,66,10,3,3,78,10,245,127,66,4,3,3,78,4,238,127,72,0,2,0,1,0,1,192,2,192,146,2,0,4,13,3,1,0,64,43,4,0,0,10,2,0,0,84,5,5,128,39,5,0,0,0,5,2,0,84,5,2,128,41,5,1,0,84,6,1,128,41,5,2,0,62,4,2,1,43,4,0,0,10,3,0,0,84,5,5,128,39,5,0,0,0,5,3,0,84,5,2,128,41,5,1,0,84,6,1,128,41,5,2,0,62,4,2,1,16,5,0,0,55,4,0,0,62,4,2,2,43,5,0,0,16,6,4,0,62,5,2,1,43,5,1,0,54,5,4,5,14,0,5,0,84,6,5,128,50,6,0,8,57,0,4,6,16,5,6,0,43,6,1,0,57,5,4,6,11,2,0,0,84,6,19,128,41,6,0,0,57,6,1,5,41,6,2,0,43,7,2,0,16,8,5,0,62,7,2,4,68,10,4,128,4,10,4,0,84,12,2,128,41,6,1,0,84,7,2,128,66,10,3,3,78,10,250,127,15,0,6,0,84,7,11,128,43,7,1,0,41,8,0,0,57,8,4,7,84,6,7,128,54,6,1,5,14,0,6,0,84,7,2,128,50,6,0,0,57,6,1,5,59,2,1,6,59,3,2,6,71,0,1,0,0,0,1,192,1,0,9,100,101,101,112,219,1,0,0,19,4,1,0,50,43,0,0,0,62,0,1,2,41,1,0,0,43,2,1,0,43,3,2,0,62,2,2,4,68,5,40,128,43,7,1,0,16,8,6,0,62,7,2,4,68,10,34,128,5,10,5,0,84,12,1,128,84,12,31,128,56,12,1,11,56,13,2,11,3,12,0,0,84,14,20,128,54,14,5,6,43,15,3,0,16,16,14,0,62,15,2,1,16,16,14,0,55,15,0,14,16,17,10,0,16,18,0,0,62,15,4,1,14,0,13,0,84,15,4,128,41,15,0,0,57,15,10,6,41,12,0,0,84,15,5,128,81,15,3,128,30,12,13,12,1,0,12,0,84,15,252,127,59,12,1,11,15,0,12,0,84,14,5,128,15,0,1,0,84,14,2,128,1,12,1,0,84,14,1,128,16,1,12,0,66,10,3,3,78,10,220,127,66,5,3,3,78,5,214,127,72,1,2,0,5,192,1,0,1,192,0,0,8,115,101,116,101,0,2,6,3,2,0,15,15,0,0,0,84,2,12,128,43,2,0,0,16,3,0,0,62,2,2,2,6,2,0,0,84,2,7,128,43,2,1,0,37,3,1,0,43,4,2,0,16,5,0,0,62,4,2,2,36,3,4,3,62,2,2,1,71,0,1,0,2,0,4,0,5,0,23,76,97,110,101,115,84,105,109,101,114,32,101,114,114,111,114,58,32,13,117,115,101,114,100,97,116,97,255,4,1,0,21,9,20,0,112,52,0,0,0,37,1,1,0,62,0,2,2,50,1,0,0,43,2,0,0,52,3,2,0,55,3,3,3,62,2,2,2,49,3,4,0,49,4,5,0,55,5,6,0,43,6,0,0,43,7,2,0,16,8,5,0,62,7,2,2,6,7,7,0,84,7,2,128,41,7,1,0,84,8,1,128,41,7,2,0,62,6,2,1,49,6,8,0,43,7,3,0,55,7,9,7,52,8,10,0,37,9,11,0,62,8,2,1,52,8,12,0,49,9,13,0,62,8,2,1,81,8,79,128,16,8,6,0,62,8,1,2,41,9,0,0,15,0,8,0,84,10,7,128,16,10,5,0,62,10,1,2,31,9,10,8,39,10,0,0,1,9,10,0,84,10,1,128,39,9,0,0,43,10,3,0,16,11,10,0,55,10,14,10,16,12,9,0,43,13,6,0,43,14,7,0,62,10,5,3,43,12,6,0,5,10,12,0,84,12,35,128,43,12,0,0,52,13,15,0,16,14,11,0,62,13,2,2,6,13,16,0,84,13,2,128,41,13,1,0,84,14,1,128,41,13,2,0,62,12,2,1,43,12,3,0,16,13,12,0,55,12,14,12,39,14,0,0,16,15,7,0,43,16,6,0,39,17,3,0,62,12,6,5,43,16,0,0,16,17,13,0,62,16,2,1,16,16,4,0,16,17,11,0,16,18,13,0,16,19,14,0,15,0,15,0,84,20,5,128,39,20,0,0,1,20,15,0,84,20,2,128,12,20,15,0,84,20,1,128,41,20,0,0,62,16,5,1,84,12,198,127,43,12,7,0,5,10,12,0,84,12,195,127,7,11,17,0,84,12,8,128,43,12,3,0,16,13,12,0,55,12,18,12,43,14,8,0,16,15,3,0,62,15,1,0,61,12,2,1,84,12,185,127,43,12,3,0,16,13,12,0,55,12,18,12,43,14,8,0,37,15,19,0,16,16,11,0,36,15,16,15,62,12,4,1,84,8,176,127,48,0,0,128,71,0,1,0,1,192,5,192,4,192,23,192,15,192,6,192,24,192,25,192,26,192,19,117,110,107,110,111,119,110,32,113,117,101,114,121,32,9,115,101,110,100,15,103,101,116,95,116,105,109,101,114,115,10,76,105,110,100,97,17,103,101,116,109,101,116,97,116,97,98,108,101,12,114,101,99,101,105,118,101,0,18,115,101,116,95,102,105,110,97,108,105,122,101,114,15,76,97,110,101,115,84,105,109,101,114,25,115,101,116,95,100,101,98,117,103,95,116,104,114,101,97,100,110,97,109,101,12,98,97,116,99,104,101,100,0,13,102,117,110,99,116,105,111,110,13,110,111,119,95,115,101,99,115,0,0,11,105,110,115,101,114,116,10,116,97,98,108,101,15,108,97,110,101,115,46,99,111,114,101,12,114,101,113,117,105,114,101,207,2,0,4,12,5,8,1,61,52,4,0,0,16,5,0,0,62,4,2,2,6,4,1,0,84,4,3,128,43,4,0,0,37,5,2,0,62,4,2,1,9,2,0,0,84,4,21,128,16,5,0,0,55,4,3,0,16,6,1,0,43,7,1,0,55,7,4,7,62,7,1,0,61,4,2,1,15,0,3,0,84,4,2,128,9,3,0,0,84,4,9,128,43,4,2,0,16,5,4,0,55,4,5,4,43,6,3,0,16,7,0,0,16,8,1,0,42,9,10,0,62,4,7,1,71,0,1,0,16,2,3,0,43,4,4,0,16,5,2,0,62,4,2,2,7,4,6,0,84,4,6,128,43,4,1,0,55,4,7,4,16,5,2,0,62,4,2,2,14,0,4,0,84,5,9,128,15,0,2,0,84,4,6,128,43,4,1,0,55,4,4,4,62,4,1,2,30,4,2,4,14,0,4,0,84,5,1,128,41,4,0,0,43,5,2,0,16,6,5,0,55,5,5,5,43,7,3,0,16,8,0,0,16,9,1,0,16,10,4,0,16,11,3,0,62,5,7,1,71,0,1,0,7,192,11,192,23,192,24,192,4,192,16,119,97,107,101,117,112,95,99,111,110,118,10,116,97,98,108,101,9,115,101,110,100,13,110,111,119,95,115,101,99,115,8,115,101,116,22,101,120,112,101,99,116,105,110,103,32,97,32,76,105,110,100,97,10,76,105,110,100,97,17,103,101,116,109,101,116,97,116,97,98,108,101,0,85,0,0,4,3,3,0,12,43,0,0,0,16,1,0,0,55,0,0,0,43,2,1,0,37,3,1,0,62,0,4,1,43,0,0,0,16,1,0,0,55,0,2,0,43,2,2,0,62,0,3,3,72,1,2,0,23,192,25,192,26,192,12,114,101,99,101,105,118,101,15,103,101,116,95,116,105,109,101,114,115,9,115,101,110,100,46,0,1,4,1,0,1,9,39,1,0,0,1,1,0,0,84,1,5,128,41,1,2,0,43,2,0,0,21,3,0,0,62,2,2,0,69,1,1,0,71,0,1,0,3,192,2,122,0,1,8,3,2,0,24,39,1,0,0,1,1,0,0,84,1,10,128,43,1,0,0,16,2,1,0,55,1,0,1,41,3,0,0,43,4,1,0,43,5,2,0,16,6,0,0,62,5,2,0,61,1,3,1,84,1,10,128,39,1,1,0,18,2,0,0,39,3,1,0,73,1,6,128,43,5,0,0,16,6,5,0,55,5,1,5,43,7,1,0,62,5,3,1,75,1,250,127,71,0,1,0,0,192,1,192,3,192,12,114,101,99,101,105,118,101,9,115,101,110,100,75,1,3,7,0,4,0,14,16,4,0,0,55,3,0,0,16,5,1,0,16,6,2,0,62,3,4,1,16,4,0,0,55,3,1,0,16,5,1,0,41,6,0,0,62,3,4,1,49,3,2,0,49,4,3,0,48,0,0,128,72,4,2,0,0,0,8,115,101,116,10,108,105,109,105,116,116,0,1,6,2,3,0,23,43,1,0,0,16,2,1,0,55,1,0,1,41,3,0,0,43,4,1,0,41,5,2,0,62,1,5,1,43,1,0,0,16,2,1,0,55,1,1,1,43,3,1,0,62,1,3,2,12,2,0,0,84,2,1,128,39,2,1,0,30,1,2,1,43,2,0,0,16,3,2,0,55,2,2,2,43,4,1,0,16,5,1,0,62,2,4,1,72,1,2,0,0,192,1,192,8,115,101,116,8,103,101,116,9,115,101,110,100,78,1,3,7,0,3,0,15,16,4,0,0,55,3,0,0,16,5,1,0,39,6,2,0,62,3,4,1,16,4,0,0,55,3,1,0,16,5,1,0,12,6,2,0,84,6,1,128,39,6,0,0,62,3,4,1,49,3,2,0,48,0,0,128,72,3,2,0,0,8,115,101,116,10,108,105,109,105,116,56,0,0,2,1,1,0,4,43,0,0,0,37,1,0,0,62,0,2,1,71,0,1,0,7,192,35,108,97,110,101,32,116,114,97,99,107,105,110,103,32,105,115,32,110,111,116,32,97,118,97,105,108,97,98,108,101,188,1,0,1,15,6,3,0,31,43,1,0,0,12,2,0,0,84,2,1,128,43,2,1,0,62,1,2,2,16,0,1,0,43,1,2,0,16,2,0,0,62,1,2,4,68,4,17,128,43,6,1,0,54,6,4,6,4,5,6,0,84,7,13,128,43,7,3,0,37,8,0,0,16,9,4,0,37,10,1,0,43,11,4,0,16,12,5,0,62,11,2,2,37,12,2,0,43,13,4,0,16,14,6,0,62,13,2,2,36,8,13,8,62,7,2,1,66,4,3,3,78,4,237,127,43,1,5,0,72,1,2,0,10,192,0,128,5,192,7,192,6,192,0,0,17,32,105,110,115,116,101,97,100,32,111,102,32,9,32,105,115,32,31,109,105,115,109,97,116,99,104,101,100,32,99,111,110,102,105,103,117,114,97,116,105,111,110,58,32,147,13,1,1,34,1,71,0,194,1,52,1,0,0,14,0,1,0,84,1,4,128,52,1,1,0,37,2,2,0,39,3,2,0,62,1,3,1,52,1,3,0,16,2,1,0,52,3,0,0,55,3,4,3,62,2,2,2,16,3,1,0,52,4,5,0,62,3,2,2,16,4,1,0,52,5,6,0,62,4,2,2,16,5,1,0,52,6,7,0,62,5,2,2,16,6,1,0,52,7,8,0,62,6,2,2,16,7,1,0,52,8,1,0,62,7,2,2,51,8,9,0,52,9,10,0,15,0,9,0,84,10,6,128,52,9,10,0,55,9,11,9,15,0,9,0,84,10,2,128,41,9,2,0,84,10,1,128,41,9,1,0,58,9,12,8,51,9,14,0,49,10,13,0,58,10,15,9,49,10,16,0,58,10,17,9,49,10,18,0,58,10,12,9,49,10,19,0,58,10,20,9,49,10,21,0,58,10,22,9,49,10,23,0,58,10,24,9,49,10,25,0,58,10,26,9,49,10,27,0,16,11,10,0,16,12,0,0,62,11,2,2,16,0,11,0,52,11,28,0,37,12,29,0,62,11,2,2,16,12,1,0,16,13,4,0,16,14,11,0,62,13,2,2,6,13,30,0,84,13,2,128,41,13,1,0,84,14,1,128,41,13,2,0,62,12,2,1,55,12,31,11,15,0,12,0,84,13,8,128,55,12,31,11,55,13,15,0,55,14,20,0,55,15,22,0,55,16,24,0,55,17,12,0,55,18,26,0,62,12,7,1,16,12,1,0,55,13,32,11,62,12,2,2,16,13,1,0,55,14,33,11,62,13,2,2,16,14,1,0,55,15,34,11,62,14,2,2,43,15,0,0,51,16,36,0,16,17,1,0,55,18,11,11,62,17,2,2,58,17,11,16,58,16,35,15,49,15,37,0,49,16,38,0,51,17,39,0,49,18,40,0,55,19,41,11,49,20,42,0,41,21,0,0,16,22,20,0,55,23,17,0,10,23,1,0,84,23,37,128,16,23,1,0,55,24,43,11,62,23,2,2,37,24,44,0,37,25,45,0,37,26,46,0,37,27,47,0,16,29,23,0,55,28,48,23,16,30,27,0,62,28,3,2,10,28,0,0,84,28,2,128,41,28,1,0,84,29,1,128,41,28,2,0,16,30,23,0,55,29,49,23,16,31,27,0,41,32,2,0,62,29,4,1,15,0,28,0,84,29,11,128,49,29,50,0,16,30,18,0,37,31,51,0,51,32,52,0,50,33,0,0,58,33,53,32,58,14,54,32,16,33,29,0,62,30,4,2,62,30,1,2,16,21,30,0,49,20,55,0,49,22,56,0,48,23,0,128,49,23,57,0,49,24,58,0,43,25,0,0,55,26,28,11,58,26,28,25,43,25,0,0,58,18,59,25,43,25,0,0,55,26,41,11,58,26,41,25,43,25,0,0,55,26,60,11,58,26,60,25,43,25,0,0,55,26,61,11,58,26,61,25,43,25,0,0,55,26,24,0,15,0,26,0,84,27,6,128,55,26,62,11,15,0,26,0,84,27,3,128,55,26,62,11,14,0,26,0,84,27,1,128,49,26,63,0,58,26,62,25,43,25,0,0,58,20,64,25,43,25,0,0,58,21,65,25,43,25,0,0,58,22,66,25,43,25,0,0,58,23,67,25,43,25,0,0,55,26,68,11,58,26,68,25,43,25,0,0,58,24,69,25,43,25,0,0,49,26,70,0,58,26,31,25,43,25,0,0,48,0,0,128,72,25,2,0,0,192,0,14,103,101,110,97,116,111,109,105,99,13,110,111,119,95,115,101,99,115,12,103,101,110,108,111,99,107,11,116,105,109,101,114,115,15,116,105,109,101,114,95,108,97,110,101,10,116,105,109,101,114,0,12,116,104,114,101,97,100,115,11,110,97,109,101,111,102,17,99,97,110,99,101,108,95,101,114,114,111,114,8,103,101,110,0,0,0,0,13,112,114,105,111,114,105,116,121,12,112,97,99,107,97,103,101,1,0,0,6,42,0,8,115,101,116,8,103,101,116,15,102,105,114,115,116,32,116,105,109,101,18,40,116,105,109,101,114,32,114,101,112,108,121,41,18,40,116,105,109,101,114,32,113,117,101,114,121,41,20,40,116,105,109,101,114,32,99,111,110,116,114,111,108,41,18,116,105,109,101,114,95,103,97,116,101,119,97,121,0,10,108,105,110,100,97,0,1,0,11,9,109,97,116,104,2,12,112,97,99,107,97,103,101,2,9,98,97,115,101,2,10,100,101,98,117,103,2,11,115,116,114,105,110,103,2,15,108,97,110,101,115,46,99,111,114,101,2,7,105,111,2,7,111,115,2,10,116,97,98,108,101,2,10,98,105,116,51,50,2,14,99,111,114,111,117,116,105,110,101,2,0,0,1,0,4,11,97,117,116,104,111,114,76,65,115,107,111,32,75,97,117,112,112,105,32,60,97,107,97,117,112,112,105,64,103,109,97,105,108,46,99,111,109,62,44,32,66,101,110,111,105,116,32,71,101,114,109,97,105,110,32,60,98,110,116,46,103,101,114,109,97,105,110,64,103,109,97,105,108,46,99,111,109,62,16,100,101,115,99,114,105,112,116,105,111,110,44,82,117,110,110,105,110,103,32,109,117,108,116,105,112,108,101,32,76,117,97,32,115,116,97,116,101,115,32,105,110,32,112,97,114,97,108,108,101,108,14,99,111,112,121,114,105,103,104,116,68,67,111,112,121,114,105,103,104,116,32,40,99,41,32,50,48,48,55,45,49,48,44,32,65,115,107,111,32,75,97,117,112,112,105,59,32,40,99,41,32,50,48,49,49,45,49,50,44,32,66,101,110,111,105,116,32,71,101,114,109,97,105,110,12,108,105,99,101,110,115,101,12,77,73,84,47,88,49,49,10,65,66,79,85,84,13,109,97,120,95,112,114,105,111,23,115,101,116,95,115,105,110,103,108,101,116,104,114,101,97,100,101,100,15,116,104,114,101,97,100,95,110,101,119,14,99,111,110,102,105,103,117,114,101,10,116,97,98,108,101,15,108,97,110,101,115,46,99,111,114,101,12,114,101,113,117,105,114,101,0,19,118,101,114,98,111,115,101,95,101,114,114,111,114,115,0,16,116,114,97,99,107,95,108,97,110,101,115,0,21,115,104,117,116,100,111,119,110,95,116,105,109,101,111,117,116,0,20,111,110,95,115,116,97,116,101,95,99,114,101,97,116,101,0,0,16,119,105,116,104,95,116,105,109,101,114,115,0,15,110,98,95,107,101,101,112,101,114,115,1,0,0,0,22,112,114,111,116,101,99,116,95,97,108,108,111,99,97,116,111,114,12,118,101,114,115,105,111,110,8,106,105,116,1,0,4,19,118,101,114,98,111,115,101,95,101,114,114,111,114,115,1,15,110,98,95,107,101,101,112,101,114,115,3,1,16,119,105,116,104,95,116,105,109,101,114,115,2,21,115,104,117,116,100,111,119,110,95,116,105,109,101,111,117,116,4,0,128,128,192,254,3,13,116,111,115,116,114,105,110,103,10,112,97,105,114,115,9,116,121,112,101,11,115,101,108,101,99,116,11,103,109,97,116,99,104,11,97,115,115,101,114,116,67,84,111,32,117,115,101,32,39,108,97,110,101,115,39,44,32,121,111,117,32,119,105,108,108,32,97,108,115,111,32,110,101,101,100,32,116,111,32,104,97,118,101,32,39,115,116,114,105,110,103,39,32,97,118,97,105,108,97,98,108,101,46,10,101,114,114,111,114,11,115,116,114,105,110,103,38,3,0,2,0,2,0,5,50,0,0,0,49,1,1,0,58,1,0,0,48,0,0,128,72,0,2,0,0,14,99,111,110,102,105,103,117,114,101,0,
};



int luaopen_lua_m_lanes(lua_State *L) {
    int rc = luaL_loadbuffer(L, (const char*)lua_m_lanes, sizeof(lua_m_lanes), "lanes.lua") || lua_pcall(L, 0, 1, 0);
	if(rc)
	{
		return luaL_error(L, "failed to initialize embedded Lanes");
	}
	return 1;
}


static luaL_Reg lanes_scripts_modules[] = {
    {"lanes", luaopen_lua_m_lanes},
    {NULL, NULL}
};

void luaopen_lanes_scripts(lua_State* L)
{
    luaL_Reg* lib = lanes_scripts_modules;
    for (; lib->func; lib++)
    {
        lua_getglobal(L, "package");
        lua_getfield(L, -1, "preload");
        lua_pushcfunction(L, lib->func);
        lua_setfield(L, -2, lib->name);
        lua_pop(L, 2);
    }
}
