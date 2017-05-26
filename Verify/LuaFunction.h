#pragma once

void Lua_ShowError(const char* error);

int Lua_DbgPrint(lua_State *L);
int Lua_SetVerifyData(lua_State *L);
int Lua_MessageBox(lua_State *L);
int Lua_GetProcessWindow(lua_State *L);
int Lua_GetProceesNum(lua_State *L);

