#include "libcommon/error.hpp"

namespace picasso {

	namespace picasso_common {

		class_error global_error;
		class_error *gerror = NULL;

		class_error::class_error() { 
			__Assert__( !gerror );
			gerror = this;
			_ecode = 0;

			pthread_mutex_init( &_lock, NULL );
		}

		class_error::~class_error() {
			pthread_mutex_destroy( &_lock );
		}

		__bool class_error::set_error_win( const __char *s, __char *file, __integer line, __char *func ) {
			return FALSE;
		}

		__void class_error::convert_string( const __char *s ) {
			__integer len = (__integer)strlen(s) + 1; 
			mbstowcs( _bufferw, s, len );
			_errorw = _bufferw;
		}

		__void class_error::convert_string( const __wchar *s ) {
			__integer len = (__integer)wcslen(s) * 2 + 1; 
			wcstombs( _buffer, s, len );
			_error = _buffer;
		}

		__void class_error::set_error( __integer ec, const __char *es ) {
			pthread_mutex_lock( &_lock );

			_ecode = ec;
			_error = es; 
			convert_string( es );

			pthread_mutex_unlock( &_lock );
		}

		__void class_error::set_error( __integer ec, const __wchar *es ) {
			pthread_mutex_lock( &_lock );

			_ecode = ec; 
			_errorw = es; 
			convert_string( es );

			pthread_mutex_lock( &_lock );
		}
		__void class_error::set_error_format( __integer ec, const __char *f, ... ) {
			__char *buf = NULL;
			__GetStrFormat__( buf, f );
			set_error( ec, buf );
		}

		__void class_error::set_error_format( __integer ec, const __wchar *f, ... ) {
			__wchar *buf = NULL;
			__GetStrFormatW__( buf, f );
			set_error( ec, buf );
		}

		__void class_error::add_error_format( const __char *f, ... ) {
			__char *buf = NULL;
			__GetStrFormat__( buf, f );
			set_error_format( _ecode, "%s\n<--%s", _error.c_str(), buf );
		}

		__void class_error::add_error_format( const __wchar *f, ... ) {
			__wchar *buf = NULL;
			__GetStrFormatW__( buf, f );
			set_error_format( _ecode, L"%s\n<--%s", _errorw.c_str(), buf );
		}

		const __char *class_error::get_error() {
			return (const __char *)_error.c_str();
		}

		const __wchar *class_error::get_errorw() {
			return _errorw.c_str();
		}

		__integer class_error::get_error_code() {
			return _ecode;
		}

		__integer class_error::get_error_string_lens() {
			return (__integer)_error.length();
		}

	}/* end picasso_common namespace */

}/* end picasso namespace */
