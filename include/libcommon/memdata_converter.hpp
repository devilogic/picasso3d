#if !defined(__MEMDATA_CONVERTER_H__)
#define __MEMDATA_CONVERTER_H__

#include "libcommon/base/base.hpp"

namespace picasso {

	namespace picasso_common {

		/*! \class class_memdata_converter
		 *  \brief 内存数据保存类
		 */
		class class_memdata_converter {
		public:
			class_memdata_converter();
			virtual ~class_memdata_converter();

		public:
			__void init();
			__void set_data( __byte* data, __integer total_size = 0 );
			__integer get_data( __byte* data, __integer size );
			__byte*	get_data();
			__void save_data( __byte* data, __integer size );
			__void add_position( __integer size );
			__byte*	get_current_data();
			__integer get_total_size();
			__bool is_end();

		private:
			__byte* _data;
			__byte*	_curr_data;
			__integer _total_size;
			__integer _current;

		};

	}/* end picasso_common namespace */

}/* end picasso namespace */

#endif
