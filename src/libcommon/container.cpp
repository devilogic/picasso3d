#include "libcommon/container.hpp"
#include "libcommon/crc32.hpp"

namespace picasso {
	namespace picasso_common {

		template< typename TYPE >   
		class_container< TYPE >::class_container() {
		}

		template< typename TYPE >   
		class_container< TYPE >::~class_container() {
			clear();
		}

		template< typename TYPE >
		TYPE class_container< TYPE >::find( const __char* nodename, __bool ref ) {
			__dword key = 0;
			class_crc32::string_crc32( nodename, key );
			__hash_map< __dword, class class_container_node* >::iterator pos = _node_list.find( key );
			if ( pos != _node_list.end() ) {
				class_container_node* node = pos->second;
				if ( ref ) node->_ref_count++;
				return (TYPE)node->_node;
			}
		}

		template< typename TYPE >
		TYPE class_container< TYPE >::get_head() {
			typename __hash_map< __dword, class class_container_node* >::iterator pos = _node_list.begin();
			if ( pos != _node_list.end() ) {
			    class_container_node* node = pos->second;
				return (TYPE)node->_node;
			}
			return NULL;
		}

		template< typename TYPE > 
		TYPE class_container< TYPE >::next() {
			if ( _pos != _node_list.end() ) {
				class_container_node* node = _pos->second;
				++_pos;
				return (TYPE)node->_node;
			}
			return NULL;
		}

		template< typename TYPE > 
		TYPE class_container< TYPE >::next( class_string& nodename ) {
			if ( _pos != _node_list.end() ) {
				class_container_node* node = _pos->second;
				++_pos;
				nodename = node->_nodename;
				return (TYPE)node->_node;
			}
			return NULL;
		}

		template< typename TYPE >
		__void class_container< TYPE >::add( TYPE T, const __char* nodename ) {
			__dword key = 0;
			class_crc32::string_crc32( nodename, key );
			__hash_map< __dword, class_container_node* >::iterator pos = _node_list.find( key );
			if ( pos != _node_list.end() ) {
				class_container_node* node = pos->second;
				node->_ref_count++;
				return;
			}

			class_container_node* add_node;
			__PoolAlloc__( class_container_node, add_node );
			add_node->_node = T;
			add_node->_ref_count = 1;
			add_node->_key = key;
			add_node->_nodename = nodename;
			_node_list[key] = add_node;
		}

		template< typename TYPE >
		__void class_container< TYPE >::add_noref( TYPE T, const __char* nodename ) {
			__dword key = 0;
			class_crc32::string_crc32( nodename, key );
			__hash_map< __dword, class_container_node* >::iterator pos = _node_list.find( key );
			if ( _pos != _node_list.end() ) {
				class_container_node* node = _pos->second;
				node->_ref_count++;
				return;
			}

			class_container_node* add_node;
			__PoolAlloc__( class_container_node, add_node );
			add_node->_node = T;
			add_node->_ref_count = 0;
			add_node->_key = key;
			add_node->_nodename = nodename;
			_node_list[key] = add_node;
		}

		template< typename TYPE >
		__integer class_container< TYPE >::delete_node( TYPE T ) {
			for( __hash_map< __dword, class class_container_node* >::iterator pos = _node_list.begin(); pos != _node_list.end(); ++pos ) {
				class_container_node* node = pos->second;
				if ( node->_node == T ) {
					node->_ref_count--;
					if ( node->_ref_count == 0 ) {
						_node_list.erase( pos );
						__PoolFree__( class_container_node, node );
						return 0;
					}
					return node->_ref_count;
				}
			}
			return -1;
		}

		template< typename TYPE >
		__void class_container< TYPE >::start() {
			_pos = _node_list.begin(); 
		}

		template< typename TYPE >   
		__integer class_container< TYPE >::get_count() {
			return (__integer)_node_list.size(); 
		}

		template< typename TYPE >
		__hash_map< __dword, class_container_node* >& class_container< TYPE >::get_list() {
			return _node_list;
		}

		template< typename TYPE > const __char* class_container< TYPE >::next_name() {
			if ( _pos != _node_list.end() ) {
				class_container_node* node = _pos->second;
				++_pos;
				return node->_nodename.get_string();
			}
			return NULL;
		}

		template< typename TYPE >
		__integer class_container< TYPE >::delete_node( const __char* nodename ) {
			__dword key = 0;
			class_crc32::string_crc32( nodename, key );
			__hash_map< __dword, class class_container_node* >::iterator pos = _node_list.find( key );
			if ( pos != _node_list.end() ) {
				class_container_node* node = pos->second;
				node->_ref_count--;

				if ( node->_ref_count == 0 ) {
					_node_list.erase( pos );
					__PoolFree__( class_container_node, node );
					return 0;
				}

				return node->_ref_count;
			}
			return -1;
		}

		template< typename TYPE >
		__void class_container< TYPE >::clear() { 
			for ( __hash_map< __dword, class class_container_node* >::iterator pos = _node_list.begin(); pos != _node_list.end(); ++pos )
				__PoolFree__( class_container_node, pos->second );
			_node_list.clear();
		}
	}/* end picasso_common namespace */
}/* end picasso namespace */
