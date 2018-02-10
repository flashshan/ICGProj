//#include "Core\Basic\LuaData.h"
//
//
//size_t ReadIntArray(lua_State *i_pState, int i_index, int *o_pInts, size_t i_numInts)
//{
//	ASSERT(i_pState);
//	ASSERT(lua_gettop(i_pState) >= -i_index);
//	ASSERT(lua_type(i_pState, i_index) == LUA_TTABLE);
//
//	size_t	index = 0;
//
//	lua_pushnil(i_pState);
//
//	while (lua_next(i_pState, i_index - 1) != 0)
//	{
//		if (lua_type(i_pState, -1) == LUA_TNUMBER)
//		{
//			lua_Integer lValue = lua_tointeger(i_pState, -1);
//			ASSERT(lValue <= INT_MAX);
//
//			o_pInts[index] = lValue < INT_MAX ? int(lValue) : INT_MAX;
//		}
//		else
//		{
//			// DEBUG_PRINT( "Expected a number at position %d while reading table as int array.\n", index );
//		}
//
//		lua_pop(i_pState, 1);
//
//		if (++index == i_numInts)
//		{
//			lua_pop(i_pState, 1);
//			break;
//		}
//	}
//
//	return index;
//}
//
//size_t ReadFloatArray(lua_State *i_pState, int i_index, float *o_pFloats, size_t i_numFloats)
//{
//	ASSERT(i_pState);
//	ASSERT(lua_gettop(i_pState) >= -i_index);
//	ASSERT(lua_type(i_pState, i_index) == LUA_TTABLE);
//
//	size_t index = 0;
//
//	lua_pushnil(i_pState);
//
//	while (lua_next(i_pState, i_index - 1) != 0)
//	{
//		if (lua_type(i_pState, -1) == LUA_TNUMBER)
//		{
//			o_pFloats[index] = float(lua_tonumber(i_pState, -1));
//		}
//		else
//		{
//			// DEBUG_PRINT( "Expected a number at position %d while reading table as float array.\n", index );
//		}
//
//		// pop the value now that we've retrieved it
//		lua_pop(i_pState, 1);
//		if (++index == i_numFloats)
//		{
//			lua_pop(i_pState, 1);
//			break;
//		}
//	}
//	return index;
//}
//
//size_t GetFloatArray(lua_State *i_pState, int i_indexTable, const char *i_pArrayKey, float *o_pFloats, size_t i_numFloats)
//{
//	ASSERT(i_pArrayKey);
//
//	lua_pushstring(i_pState, i_pArrayKey);
//	i_indexTable--;
//
//	int type = lua_gettable(i_pState, i_indexTable);
//
//	size_t itemsRead = 0;
//
//	if (type == LUA_TTABLE)
//	{
//		itemsRead = ReadFloatArray(i_pState, i_indexTable, o_pFloats, i_numFloats);
//	}
//	else
//	{
//		// DEBUG_PRINT( "Expected Lua table for %s found value of type %d.\n", i_pArrayKey, type );
//	}
//
//	lua_pop(i_pState, 1);
//
//	return itemsRead;
//}
//
