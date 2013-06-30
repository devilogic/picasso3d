#include "libcommon/string.hpp"

namespace picasso {

	namespace picasso_common {

		class_string::class_string() {
		}

		class_string::class_string( const __char *s )
			: std::string( s ) {		
		}

		class_string::class_string( const std::string &s ) 
			: std::string( s ) {
		}

		__integer class_string::a2i() {
			return atoi(std::string::c_str());
		}

		__float class_string::a2f() {
			return (__float)atof(std::string::c_str());
		}

		class_string class_string::get_token( __integer tok, __char *sep ) const {	
			if ( sep == NULL )
				sep = (__char *)" ,\t\n/";

			__char *tmp = (__char *)(alloca((length() + 1) * sizeof(__char)));
			strcpy( tmp, c_str() );

			__char *tmp2 = (__char *)(alloca((length() + 1) * sizeof(__char)));
			strcpy( tmp2, c_str() );

			__char *ptok = NULL;
			ptok = strtok( tmp, sep );
			for ( __integer t = 0; t < tok; t++ ) {
				ptok = strtok( NULL, sep );
				if ( !ptok )
					return "";
			}
			if ( !ptok )
				return "";

			return ptok;
		}

		__void class_string::make_lower() {
			for ( std::string::iterator p = std::string::begin(); p != std::string::end(); ++p )
				*p = toupper( *p );
		}

		__void class_string::make_upper() {
			for ( std::string::iterator p = std::string::begin(); p != std::string::end(); ++p )
				*p = tolower( *p );
		}

		__void class_string::format( const __char *s, ... ) {
			__char temp[2048] = {0};
			va_list args;
			va_start( args, s );
			sprintf( temp, s, args );
			operator=( temp );
		}

		class_string class_string::get_filename() const {	
			__integer size = (__integer)(length()+ 1);
			__char *temp = (__char*)(alloca(size * sizeof(__char)));
			strcpy( temp, c_str() );

			__char *p = strrchr( temp, '/' );
			if ( !p ) {
				p = strrchr( temp, '\\' );
				if ( !p ) {
					class_string res = temp;
					return res;
				}
			}
			p++;

			return p;
		}

		__integer class_string::search( const __char *s ) const {
			if ( strstr( c_str(), s ) )
				return 1;
			return 0;
		}

		__void class_string::replace( __char c, __char rc ) {
			__char *temp = (__char*)(alloca((length() + 1) * sizeof(__char)));
			strcpy( temp, c_str() );

			__integer len = length();
			for ( __integer i = 0; i < len; i++ ) {
				if ( temp[ i ] == c )
					temp[ i ] = rc;
			}

			operator=(temp);
		}

		__void class_string::replace( const __char *s, const __char *rs ) {
			__integer i = find( s );
			if ( i != std::string::npos )
				std::string::replace( i, strlen( s ), rs );
		}

		__void class_string::extract_ext() {
			__char *temp = (__char*)(alloca((length() + 1) * sizeof(__char)));
			strcpy( temp, c_str() );

			__char *p = strrchr( temp, '.' );
			if ( !p )
				return;
			*p = 0;

			std::string::operator=( temp );
		}

		__void class_string::replace_ext( __char *ext ) {
			__char temp[2048] = {0};
			strcpy( temp, c_str() );

			__char *p = strrchr( temp, '.' ); 
			if ( !p )
				return;
			p++;
			strcpy( p, ext );

			std::string::operator=( temp );
		}

		__void class_string::get_path() {
			__char *temp = (__char*)(alloca((length() + 1) * sizeof(__char)));
			strcpy( temp, c_str() );

			__char *p = strrchr( temp, '/' );
			if ( !p ) {
				p = strrchr( temp, '\\' );
				if ( !p )
					return;		 
			}
			p[ 0 ] = 0;	
			std::string::operator=( temp );
		}

		__void class_string::trim_right() {
		}
		__void class_string::trim_left() {
		}

		__integer class_string::get_char_count( __char c ) const {
			__integer n = 0;
			for ( __integer i = 0; i < length(); i++ ) {
				if ( operator[](i) == c )
					n++;
			}

			return n;
		}

		__integer compute_hashcode( const __char *s ) {
			__integer i,len;
			__integer ch;
			__integer r;

			len = (__integer)strlen( s );
			if ( len <= 0 )
				return 0;

			r = 0x19831210;
			for ( i = 0; i < len; i++ ) {
				ch = (__integer)s[i];
				r = ((r << 5) + r) + ch;
			}

			return r;
		}

		__bool get_str_param( const __char *s, __integer idx, __char *p ) {
			const __char *f = s;
			__integer c = 0;
			__bool open = FALSE;
			__integer ip = 0;
			__integer i = 0;
			while ( f[i] != '\0' && c <= idx ) {
				if ( f[i] == '"' )
					open = (open == FALSE) ? TRUE : FALSE;
				else {
					if (open == FALSE && (f[i] == ' ' || f[i] == '\t'))
						c++;
					else {
						if ( c == idx )
							p[ip++] = f[i];
					}
				}
				i++;
			}
			p[ip] = '\0';

			if ( ip == 0 )
				return FALSE;

			return TRUE;
		}

		__bool get_str_param( const __wchar *s, __integer idx, __wchar *p ) {
			const __wchar *f = s;
			__integer c = 0;
			__bool open = FALSE;
			__integer ip = 0;
			__integer i = 0;
			while ( f[i] != L'\0' && c <= idx ) {
				if ( f[i] == L'"' )
					open = (open == FALSE) ? TRUE : FALSE;
				else {
					if (open == FALSE && (f[i] == L' ' || f[i] == L'\t'))
						c++;
					else {
						if ( c == idx )
							p[ip++] = f[i];
					}
				}
				i++;
			}
			p[ip] = L'\0';

			if ( ip == 0 )
				return FALSE;

			return TRUE;
		}

	}/* end picasso_common namespace */

}/* end picasso namespace */
