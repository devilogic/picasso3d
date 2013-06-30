#include "libcommon/memory_pool.hpp"

namespace picasso {

	namespace picasso_common {

		template <typename T>
		class_memory_pool<T>::class_memory_pool() {
			_head = NULL;
			_tail = NULL;
			_create_count = -1;

			_use_mutex = FALSE;

#if defined(_DEBUG)
			_count = 0;
			_max_count = 0;
#endif
		}

		template <typename T>
		class_memory_pool<T>::~class_memory_pool() {
		}

		template <typename T>	
		T *class_memory_pool<T>::memory_alloc() {
			if ( _use_mutex )
				pthread_mutex_lock( &_lock );

			if ( !_head )
				new_memory_alloc();

			struct_pool_node<T> *node = _head;
			_head = _head->next;
			T *data = (T *)node;
#ifdef _DEBUG
			_count++;
			if ( _max_count < _count ) {
				_max_count = _count;
#endif
				if ( _use_mutex )
					pthread_mutex_unlock( &_lock );

				return data;
			}

			template <typename T>
				__void class_memory_pool<T>::memory_free( T *p ) {
				if ( _use_mutex )
					pthread_mutex_lock( &_lock );

#ifdef _DEBUG
				_count--;
#endif

				struct_pool_node<T> *node = (struct_pool_node<T> *)p;
				if ( _tail ) _tail->next = node;
				_tail = node;
				node->next = NULL;

				if ( !_head )
					_head = node;

				if ( _use_mutex )
					pthread_mutex_unlock( &_lock );
			}

#ifdef _DEBUG
			template <typename T>
				__void class_memory_pool<T>::create( const __char *name, __integer create_count, __bool use_mutex ) {
				_name = name;
				_create_count = create_count;
				_use_mutex = use_mutex;

				new_memory_alloc();

				if ( _use_mutext ) pthread_mutex_init( &_lock, NULL );

				__char msg[256] = {0};
				sprintf( msg, "===== Create ClassName[ %s ] =====\n", _name.c_str() );
				// 调试信息输出msg
			}
#else
			template <typename T>
				__void class_memory_pool<T>::create( __integer create_count, __bool use_mutex ) {
				_create_count = create_count;
				_use_mutex = use_mutex;

				new_memory_alloc();
				if ( _use_mutex ) pthread_mutex_init( &_lock, NULL );
			}
#endif

			template <typename T>
				__void class_memory_pool<T>::release() {
#ifdef _DEBUG
				__char msg[256];
				sprintf( msg, "===== Release ClassName[ %s ] - [ %d ] - [size:%d] =====\n", _name.c_str(), _max_count, sizeof(struct_pool_node<T>) );
				// 输出调试信息msg
#endif
				for ( std::list<__char *>::iterator pos = _node_list.begin(); pos != _node_list.end(); ++pos )
					delete [] (*pos);

				_node_list.clear();
				_head = NULL;
				_tail = NULL;
				if ( _use_mutex )
					pthread_mutex_destroy( &_lock );
			}

			template <typename T>
				__void class_memory_pool<T>::new_memory_alloc() {
				struct_pool_node<T> *node_list = (struct_pool_node<T> *)new __char[sizeof(struct_pool_node<T>) * _create_count];
				_node_list.push_back((__char *)node_list);

				__integer i;
				for ( i = 0; i < _create_count - 1; i++ )
					node_list[i].next = &node_list[i+1];
				node_list[i].next = NULL;

				if ( _tail )
					_tail->next = node_list;

				if ( _head == NULL )
					_head = node_list;
				_tail = &node_list[_create_count - 1];
			}

			template <typename T> 
				class_memory_pool<T>& get_memory_pool() {
				static class_memory_pool<T> global;
				return global;
			}

		}/* end picasso_common namespace */

	}/* end picasso namespace */
