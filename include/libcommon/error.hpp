#if !defined(__ERROR_H__)
#define __ERROR_H__

#include "libcommon/base/base.hpp"

namespace picasso {

	namespace picasso_common {

		/*! \class class_error
		 *  \brief 错误输出类
		 */
		class class_error {
		public:
			class_error();
			~class_error();

			const __char *get_error();
			const __wchar *get_errorw();
			__integer get_error_code();
			__integer get_error_string_lens();

			__void set_error( __integer ec, const __char *es );
			__void set_error_format( __integer ec, const __char *f, ... );
			__void add_error_format( const __char *f, ... );

			virtual __bool set_error_win( const __char *s, __char *file, __integer line, __char *func );

			__void set_error( __integer ec, const __wchar *es );
			__void set_error_format( __integer ec, const __wchar *f, ... );
			__void add_error_format( const __wchar *f, ... );

		private:
			__void convert_string( const __char *s );
			__void convert_string( const __wchar *s );

			std::string _error;
			std::wstring _errorw;
			__integer _ecode;

			__wchar	_bufferw[1024];
			__char _buffer[1024];

			pthread_mutex_t _lock;
		};

		__EXTERN__ class_error *gerror;

#define SET_ERROR_MSG( s ) gerror->set_error_format( 1, "File %s\nLine %d\nFunc %s\n%s", __FILE__, (__integer)__LINE__, __FUNCTION__, s );
#define SET_ERROR_MSG_WIN( s ) gerror->set_error_win( s, __FILE__, (DWORD)__LINE__, __FUNCTION__ );

	}/* end picasso_common namespace */

}/* end picasso namespace */

#endif
