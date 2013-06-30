#include "libcommon/memdata_converter.hpp"

namespace picasso {

	namespace picasso_common {

		class_memdata_converter::class_memdata_converter() {
			_data = NULL;
			_curr_data = NULL;
			_total_size = 0;
			_current = 0;
		}

		class_memdata_converter::~class_memdata_converter() {
		}

		__void class_memdata_converter::init() {
			_curr_data = _data;
			_current = 0;
		}

		__bool class_memdata_converter::is_end() {
			if ( _data + _total_size <= _curr_data )
				return TRUE;	
			return FALSE;
		}

		__void class_memdata_converter::set_data( __byte* data, __integer total_size /*= 0*/ ) {
			_data = (__byte*)data;
			_total_size = total_size;
		}

		__integer class_memdata_converter::get_data( __byte* data, __integer size ) {
			if ( _current >= _total_size && _total_size != 0 ) {
				memset( data, 0, size );
				return 0;
			}
			memcpy( data, _curr_data, size );
			_curr_data += size;
			_current += size;
			return size;
		}

		__byte* class_memdata_converter::get_data() {
			return _data;
		}

		__void class_memdata_converter::save_data( __byte* data, __integer size ) {
			memcpy( _curr_data, data, size );
			_curr_data += size;
		}

		__void class_memdata_converter::add_position( __integer size ) {
			_curr_data += size;
		}

		__byte* class_memdata_converter::get_current_data() {
			return _curr_data;
		}

		__integer class_memdata_converter::get_total_size() {
			return _total_size;
		}

	}/* end picasso_common namespace */

}/* end picasso namespace */
