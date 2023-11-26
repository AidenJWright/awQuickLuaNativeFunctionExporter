#pragma once
#include <unordered_map>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace aw_qLua
{
  static std::unordered_map<const char*, int (**)(lua_State*)> exportedCpptoLua;

  void ExportAll(lua_State* L)
  {
    for (const auto& [name, location] : exportedCpptoLua)
    {
      lua_pushcfunction(L, *location);
      lua_setglobal(L, name);
    }
  }

  bool ExportOne(lua_State* L, const char* name)
  {
    if (auto search = exportedCpptoLua.find(name); search != exportedCpptoLua.end())
    {
      lua_pushcfunction(L, *search->second);
      lua_setglobal(L, name);
      return true;
    }
    else
      return false;
    
  }
  
  void PrintExportList()
  {
    for (const auto& [name, location] : exportedCpptoLua)
      printf("Name:[%s] Location[%p]\n", name, *location);
  }
}

#define awLuaFunction(Fn) \
int (*Fn)(lua_State*) = nullptr;\
aw_qLua::exportedCpptoLua[#Fn] = &Fn;\
Fn = [](lua_State* L) -> int
