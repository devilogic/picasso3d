#if !defined(__STRING_H__)
#define __STRING_H__

#include "libcommon/base/base.hpp"

namespace picasso {

	namespace picasso_common {

		/*! \class class_string
		 *  \brief 字符串
		 */
		class class_string : public std::string {
		public:
			class_string();
			class_string( const char* s );
			class_string( const std::string &s );

		public:
			__integer get_length() const { return (__integer)length(); }
			const __char* get_string() const { return c_str(); }
			__bool is_empty() const { return empty(); }
			__integer a2i();
			__float a2f();
			class_string get_token( __integer tok, __char *sep = NULL ) const;
			__void make_lower();
			__void make_upper();
			__void format( const __char *s, ... );

			class_string get_filename() const;
			__integer search( const __char *s ) const;
			__void replace( __char c, __char rc );
			__void replace( const __char *s, const __char *rs );
			__void extract_ext();
			__void replace_ext( __char *ext );
			__void get_path();
			__void trim_right();
			__void trim_left();
			__integer get_char_count( __char c ) const;
		};

		__integer compute_hashcode( const __char *s );
		__bool get_str_param( const __char *s, __integer idx, __char *p );
		__bool get_str_param( const __wchar *s, __integer idx, __wchar *p);

	}/* end picasso namespace */

}/* end picasso namespace */

#endif
