
#ifndef __SRC_UTILS_DEFINE_H__
#define __SRC_UTILS_DEFINE_H__

#include <map>
#include <stdint.h>

// UnorderedMap定义
#if ( __GUNC__ < 4 || __GNUC__ == 4 && __GNUC_MINOR__ < 1 )
#include <map>
#define UnorderedMap std::map
#else
#include <tr1/unordered_map>
#define UnorderedMap std::tr1::unordered_map
#endif

// 分支预测
#define likely(x)       __builtin_expect( (x), 1 )
#define unlikely(x)     __builtin_expect( (x), 0 )

// 全局事件修正
extern int32_t g_ModifyTimestamp;

#endif