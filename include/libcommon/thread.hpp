/**************************************************
 * 简介: 线程控制类
 * 描述: 用于线程的定义与实现
 *
 * 作者: devilogic
 * 联系方式: logic.yan@gmail.com
 **************************************************/
#if !defined(__THREAD_H__)
#define __THREAD_H__


namespace picasso {
	namespace picasso_thread {
		
		class class_thread_lock {
			/*! \class class_thread_lock
			 *  \brief 线程锁类
			 */
		public:
			class_thread_lock();
			virtual ~class_thread_lock();
		public:
			__bool open();
			__void close();
			__bool is_open();
			__void lock();
			__void unlock();
			__bool try_lock();
		};

		class class_thread {
			/*! \class class_thread
			 *  \brief 线程类
			 */
		public:
			class_thread();
			virtual ~class_thread();
		public:
			__bool create_thread(__void* args);
			__void terminate_thread();
		private:
			pthread_t* _thread;
		};
	}/* end picasso_thread namespace */
}/* end picasso namespace */


#endif
