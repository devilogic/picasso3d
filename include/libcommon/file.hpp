#if !defined(__FILE_H__)
#define __FILE_H__

#include "libcommon/base/base.hpp"

namespace picasso {

	namespace picasso_common {

		/*! \class class_file
		 *  \brief 操作文件的类
		 */
		class class_file {
		public:
			class_file();
			virtual ~class_file();

			virtual __bool open( const __char *filename );
			virtual __bool save( const __char *filename );
			virtual __void close();

			virtual __void write( __sinteger si );
			virtual __void write( __float f );
			virtual __void write( __double d );
			virtual __void write( __char c );
			virtual __void write( __byte b );
			virtual __void write( __word w );
			virtual __void write( __dword dw );
#if defined(_X64)
			virtual __void write( __qword qw );
#endif
			virtual __void write( const __char *s );
			virtual __void write( const std::string &s );
			virtual __void write( const __wchar *ws );
			virtual __void write( const __void *p, __integer size );

			virtual __bool read( __void *p, __integer size );
			virtual __bool read( std::string &s );

			virtual __void set_pos( __integer i );
			virtual __bool is_end() const;

			__bool is_save() const { return _saved; }
			__integer get_size() const { return _size; }
			const __char *get_filename() const { return _filename.c_str(); }

		protected:
			__void* allocate_string_buffer( __integer size );

		private:
			__bool read_data( __void *p, __integer size );

		protected:
			FILE *_fp;
			std::string _filename;
			__integer _size;
			__bool _saved;
			__integer _type; /*!< 0:class_file, 1:class_read_buffer */
		};

		/*! \class class_read_buffer
		 *  \brief 缓冲区读取
		 */
		class class_read_buffer : public class_file {
		public :
			class_read_buffer();
			virtual ~class_read_buffer();

			__bool read_all_buffer();
			__void set_read_buffer( const __char *filename, __char *buffer, __integer size, __bool mset = false );
	
			__bool read( std::string &s );
			__bool read( __void *p, __integer size );

			virtual __void close();
			virtual __void set_pos( __integer pos );
			virtual __bool is_end() const;

			__char *get_buffer() { return _buffer; }

		private:
			__bool read_data( __void *p, __integer size );

		private:
			__char *_buffer;
			__char *_pos;
			__bool _mset;
		};

	}/* end picasso_common namespace */

}/* end picasso namespace */

#endif
