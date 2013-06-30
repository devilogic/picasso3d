#if !defined(__MEMORY_POOL_H__)
#define __MEMORY_POOL_H__

#include "./base/base.hpp"

namespace picasso {

	namespace picasso_common {

#define __USE_MEM_POOL__ 1
#if defined(__USE_MEM_POOL__)

		template <typename T>
			struct struct_pool_node {
				T data;
				struct_pool_node *next;
			};

		/*! \class class_memory_pool
		 *  \brief 内存池
		 */
		template <typename T>
			class class_memory_pool {
		public:
			class_memory_pool();
			virtual ~class_memory_pool();

		public:
			T *memory_alloc();
			__void memory_free(T *p);

#if defined(_DEBUG)
			__void create( const __char *name, __integer create_count, __bool use_mutex );
#else
			__void create( __integer create_count, __bool use_mutex );
#endif

			__void release();

		private:
			__void new_memory_alloc();

		private:
			__integer _create_count;
			struct struct_pool_node<T> _head;
			struct struct_pool_node<T> _tail;
			std::list<__char *>	_node_list;

			pthread_mutex_t _lock;
			__bool _use_mutex;

#if defined(_DEBUG)
			std::string _name;
			__integer _count;
			__integer _max_count;
#endif
		};

		/*! \fn get_memory_pool
		 *  \brief 获取内存池对象
		 */
		template <typename T> class_memory_pool<T>& get_memory_pool();

		/*! \class class_memory_pool_object
		 *  \brief 内存池节点对象类
		 *  \details 要使用内存池进行管理,必须都继承此类
		 */
		class class_memory_pool_object {
		public:
			virtual __void init() = 0;
			virtual __void release() = 0;
		};

		////////////////////////////////////////////////////////////
		// 宏
#define __PoolAlloc__(class_t, object) { object = (class_t *)get_memory_pool<class_t>().memory_alloc(); ((class_t *)object)->init(); }
#define __PoolFree__(class_t, object) { if (object) { ((class_t *)object)->release(); get_memory_pool<class_t>().memory_free((class_t *)object); } }

#if defined(_DEBUG)
#define __PoolCreate__(class_t, count, lock) get_memory_pool<class_t>().create(class_t, count, lock);
#else
#define __PoolCreate__(class_t, count, lock) get_memory_pool<class_t>().create(count, lock);
#endif
#define __PoolRelease__(class_t) get_memory_pool<class_t>().release();

#else // NON DEFINED __USE_MEM_POOL__

#define __PoolAlloc__(class_t, object) { object = new class_t; object->init(); }
#define __PoolFree__(class_t, object) { (class_t *)object->release() object };

#define __PoolCreate__(class_t, count, lock) 
#define __PoolRelease__(class_t)

#endif /* end __USE_MEM_POOL__ */

	}/* end picasso_common namespace */

}/* end picasso namespace */

#endif

