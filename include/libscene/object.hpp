/**************************************************
 * 简介: 对象基类
 * 描述: 所有的场景对象结构所在
 *
 * 作者: devilogic
 * 联系方式: logic.yan@gmail.com
 **************************************************/
#if !defined(__OBJECT_H__)
#define __OBJECT_H__

#include "libcommon/base/base.hpp"

namespace picasso {
	namespace picasso_scene {
		class class_object {
			/*! \class class_object
			 *  \brief 纯虚类用于定义场景中所有对象的基础
			 */
		public:
			class_object();
			virtual ~class_object();
		private:
		};
		
	}/* end picasso_scene namespace */
}/* end picasso namespace */



#endif
