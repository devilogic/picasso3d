#include "libcommon/file.hpp"
#include "libcommon/error.hpp"

namespace picasso {

	namespace picasso_common {

		class_file::class_file() {
			_fp = NULL; 
			_size = 0; 
			_saved = FALSE;
			_type = 0;
		}

		class_file::~class_file() {
			close();
		}

		__bool class_file::open( const __char *filename ) {
			_filename = filename;

			_fp = fopen( _filename.c_str(), "rb" );
			if ( !_fp ) {
				gerror->set_error_format( 1, "Error: Failed to open file (%s)", _filename.c_str() );
				return FALSE;
			}

			fseek( _fp, 0, SEEK_SET );
			fseek( _fp, 0, SEEK_END );
			_size = ftell( _fp );

			fseek( _fp, 0, SEEK_SET );
			_saved = FALSE;

			return TRUE;
		}

		__bool class_file::save( const __char *filename ) {
			_filename = filename;

			_fp = fopen( _filename.c_str(), "wb" );
			if ( !_fp ) {
				gerror->set_error_format( 1, "Error: Failed to save file (%s)", _filename.c_str() );
				return FALSE;
			}

			fseek( _fp, 0, SEEK_SET );
			fseek( _fp, 0, SEEK_END );
			_size = ftell( _fp );

			fseek( _fp, 0, SEEK_SET );
			_saved = TRUE;

			return TRUE;
		}

		__void* class_file::allocate_string_buffer( __integer size ) {
			return (__void *)new __byte [size];
		}

		__void class_file::write( const __char *s ) {
			__Assert__( is_save() );
			__integer len = (__integer)strlen( s );
			if( len > 0 ) {
				len++;
				write( len );
				write( s, sizeof(__char) * len );
			} else {
				write( 0 );
			}
		}

		__void class_file::write( const std::string &s ) {
			__Assert__( is_save() );
			write( s.c_str() );
		}

		__void class_file::write( const __wchar *ws ) {
			__Assert__( is_save() );
			__integer len = (__integer)wcslen( ws );
			if ( len > 0 ) {
				len++;
				write( len );
				write( ws, sizeof(__wchar) * len );
			} else {
				write( 0 );
			}
		}

		__void class_file::write( __sinteger si ) {
			__Assert__( is_save() );
			fwrite( &si, sizeof( __sinteger ), 1, _fp );
		}

		__void class_file::write( __float f ) {
			__Assert__( is_save() );
			fwrite( &f, sizeof( __float ), 1, _fp );
		}

		__void class_file::write( __double d ) {
			__Assert__( is_save() );
			fwrite( &d, sizeof( __double ), 1, _fp );
		}

		__void class_file::write( __char c ) {
			__Assert__( is_save() );
			fwrite( &c, sizeof( __char ), 1, _fp );
		}

		__void class_file::write( __byte b ) {
			__Assert__( is_save() );
			fwrite( &b, sizeof( __byte ), 1, _fp );
		}

		__void class_file::write( __word w ) {
			__Assert__( is_save() );
			fwrite( &w, sizeof( __word ), 1, _fp );
		}

		__void class_file::write( __dword dw ) {
			__Assert__( is_save() );
			fwrite( &dw, sizeof( __dword ), 1, _fp );
		}

#if defined(_X64)
		__void class_file::write( __qword qw ) {
			__Assert__( is_save() );
			fwrite( &qw, sizeof( __qword ), 1, _fp );
		}
#endif

		__void class_file::write( const __void *p, __integer size ) {
			__Assert__( is_save() );
			fwrite( p, sizeof( __byte ), size, _fp );
		}

		__void class_file::close() {
			fclose( _fp );
			_fp = NULL;
		}

		__void class_file::set_pos( __integer i ) {
			fseek( _fp, i, SEEK_SET );
		}

		__bool class_file::is_end() const {
			return ( ftell( _fp ) == _size );
		}

		__bool class_file::read( std::string &s ) {
			__integer size = 0;
			__bool b = read_data( &size, sizeof( __integer ) );
			if ( !b ) return FALSE;
	
			__char *str = new __char [ size ];
			b = read_data( str, size );
			if ( !b ) {
				delete [] str;
				return FALSE;
			}

			s = str;
			delete [] str;
			return TRUE;
		}

		__bool class_file::read( __void *p, __integer size ) {
			return read_data( p, size );
		}

		__bool class_file::read_data( __void *p, __integer size ) {
			__Assert__( p );
			__integer rsize = fread( p, sizeof( __char ), size, _fp );
			if( rsize != size )
				return FALSE;
			return TRUE;
		}

		////////////////////////////////////////////////////////////
		// class_read_buffer
		class_read_buffer::class_read_buffer() {
			_buffer = NULL;
			_pos = NULL;
			_mset = false;
			_type = 1;
		}

		class_read_buffer::~class_read_buffer() {
			close();
		}

		__bool class_read_buffer::read_all_buffer() {
			__Assert__( _buffer );

			_buffer = new __char[ _size ];
			__integer rsize = fread( _buffer, sizeof(__byte), _size, _fp );
			if( rsize != _size ) {
				delete [] _buffer;
				_buffer = NULL;
				return FALSE;
			}

			_pos = _buffer;
			_mset = FALSE;
	
			return TRUE;
		}

		__void class_read_buffer::set_read_buffer( const __char *filename, __char *buffer, __integer size, __bool mset ) {
			_filename = filename;
			if ( mset )
				_buffer = buffer;
			else {
				_buffer = new __char[ size ];
				memcpy( _buffer, buffer, size );
			}
			_pos = _buffer;
			_size = size;
			_saved = FALSE;
			_mset = mset;
		}

		__void class_read_buffer::close() {
			if ( _mset )
				_buffer = NULL;
			else {
				if ( !_mset ) {
					delete [] _buffer;
					_buffer = NULL;
				}
			}
			_pos = NULL;
		}

		__void class_read_buffer::set_pos( __integer pos ) {
			_pos = _buffer + pos;
		}

		__bool class_read_buffer::is_end() const {
			return ( _pos >= ( _buffer + _size ) ) ? TRUE : FALSE;
		}

		__bool class_read_buffer::read( std::string &s ) {
			__integer size = 0;
			__bool b = read_data( &size, sizeof( __integer ) );
			if ( !b ) return FALSE;
	
			__char *str = new __char [ size ];
			b = read_data( str, size );
			if ( !b ) {
				delete [] str;
				return FALSE;
			}

			s = str;
			delete [] str;
			return TRUE;
		}

		__bool class_read_buffer::read( __void *p, __integer size ) {
			return read_data( p, size );
		}

		__bool class_read_buffer::read_data( __void *p, __integer size ) {
			if ( (_pos + size) > (_buffer + _size) )
				return FALSE;

			memcpy( p, _pos, size );
			_pos += size;
			return TRUE;
		}

	}/* end picasso_common namespace */

}/* end picasso namespace */
