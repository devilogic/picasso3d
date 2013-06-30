#if !defined(__CONTAINER_H__)
#define __CONTAINER_H__

#include "libcommon/base/base.hpp"
#include "libcommon/string.hpp"
#include "libcommon/memory_pool.hpp"

namespace picasso {
	namespace picasso_common {

		class class_container_node : public class_memory_pool_object {
		public:
			class_string _nodename;
			__dword _key;
			__void* _node;
			__integer _ref_count;
		public:
			virtual __void init(){};
			virtual __void release(){};
		};

		template< typename TYPE > 
			class class_container {
		public:
			class_container();
			virtual ~class_container();
		public:
			__void add( TYPE T, const __char* nodename );
			__void add_noref( TYPE T, const __char* nodename );
			TYPE find( const __char* nodename, __bool ref );
			__integer delete_node( TYPE T );
			__integer delete_node( const __char* nodename );
			__void clear();
			__void start();
			TYPE get_head();
			TYPE next();
			TYPE next( class_string& nodename );
			const __char* next_name();
			__integer get_count();
			__hash_map< __dword, class class_container_node* >& get_list();

		private:
			__hash_map< __dword, class class_container_node* > _node_list;
			typename __hash_map< __dword, class class_container_node* >::iterator _pos;
		};

#define __ContainerLoopDeleteClear__( name, class_t ) { \
			name.start();								\
			class_t* data;								\
			while ( data = name.next() ) delete pData;	\
			name.clear();								\
		}	

#define __ContainerLoopObjectFuncClear__( name, class_t, fun ) {	\
			name.start();											\
			class_t* data;											\
			while ( data = name.next() )							\
				fun( data );										\
			name.clear();											\
		}

#define __ContainerLoopStringFuncClear__( name, fun ) {			\
			name.start();										\
			__char* name_string;								\
			while ( name_string = (__char*)name.nextname() )	\
				fun( name_string );								\
			name.clear();										\
		}

	}/* end picasso_common namespace */
}/* end picasso namespace */

#endif
