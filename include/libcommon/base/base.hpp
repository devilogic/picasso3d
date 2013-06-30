#if !defined(__BASE_H__)
#define __BASE_H__

//////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <memory.h>
#include <pthread.h>
#include <inttypes.h>
#include <limits.h>

//////////////////////////////
#include <vector>
#include <map>
#include <string>
#include <list>
#include <set>
#include <queue>
#include <stack>
// 一些STL的实现没有将hash_map放入std命名空间
#if defined(_MSC_VER)
#include <hash_map>
#define __hash_map std::hash_map
#else
#include <ext/hash_map>
#define __hash_map __gnu_cxx::hash_map
#endif

//////////////////////////////
#include "configure.hpp"
#include "typedef.hpp"
#include "except.hpp"

namespace picasso {

	namespace picasso_common {

#if defined(_DEBUG)
#define __Assert__( s ) assert(s)
#else
#define __Assert__( s )
#endif

#define __GetStrFormat__( buf, fmt )					\
		va_list args;									\
		va_start(args, fmt);							\
		__integer len = vprintf( fmt, args ) + 1;		\
		buf = (__char *)(alloca(len * sizeof(__char)));	\
		sprintf(buf, fmt, args);

#define __GetStrFormatW__(buf, fmt)							\
		va_list args;										\
		va_start(args, fmt);								\
		__integer len = vwprintf(fmt, args) + 1;			\
		buf = (__wchar *)(alloca(len * sizeof(__wchar)));	\
		swprintf(buf, len, fmt, args);

#define __SafeDelete__( p ) { if(p) { delete (p); (p)=NULL; } }
#define __SafeDeleteArray__(p) { if(p) { delete[] (p); (p)=NULL; } }
#define __SafeRelease__(p) { if(p) { (p)->release(); (p)=NULL; } }
#define __SafeDeleteVector__(p) { if(!p.empty()) { for(__integer i=0; i<p.size(); i++) __SafeDelete__(p[i]); p.resize(0); } }
#define __SafeDeleteVectorArray__(p) { if(!p.empty()) { for(__integer i=0; i<p.size(); i++) __SafeDeleteArray__(p[i]); p.resize(0); } }
#define __SafeDeleteList__(type, p) { if (!p.empty()) { for(list<type>::iterator listPos=p.begin(); listPos != p.end(); ++listPos) { __SafeDelete__(*listPos); } p.clear(); } }
#define __SetAttr__(baseAttr, setAttr) ((baseAttr) |= (setAttr))
#define __ReSetAttr__(baseAttr, setAttr) ((baseAttr) &= ~(setAttr))
#define __IsAttr__(baseAttr, setAttr) (((baseAttr) & (setAttr)) ? TRUE : FALSE)
#define __IsAttrAll__(baseAttr, setAttr) ((((baseAttr) & (setAttr)) == (setAttr)) ? TRUE: FALSE)

	}/* end picasso_common namespace */

}/* end picasso namespace */

#endif
