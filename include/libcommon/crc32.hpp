#include "libcommon/base/base.hpp"

namespace picasso {

	namespace picasso_common {

		class class_crc32 {
		public:
			class_crc32();
			virtual ~class_crc32();

		public:
			__dword string_crc32( __char* s );
			__dword string_crc32( __char* s, __dword &v );
			__dword string_crc32( __wchar* s );
			__dword string_crc32( __wchar* s, __dword &v );
			__dword file_crc32( __char* filename, __dword &v );
			__dword memory_crc32( __byte* data, __integer size );
			__dword memory_crc32( __byte* data, __integer size, __dword &v );

		protected:
			__STATIC__ __bool get_file_size( FILE* fp );
			__STATIC__ __void calc( const __byte b, __dword &v );

		protected:
			__STATIC__ __dword _crc32_table[256];
		};

	}/* end picasso_common namespace */

}/* end picasso namespace */
