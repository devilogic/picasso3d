#include "libcommon/base/base.hpp"

namespace picasso {
	namespace picasso_math {

		/* \class color
		 * \brief 颜色类
		 */
		class color {
		public:
			union {
				struct {
					__byte _b,_g,_r,_a;
				};
				__dword _c;
			};
			color( __byte a, __byte r, __byte _g, __byte _b);
			color( color c );

			void FromNormal3D(const D3DXVECTOR3& n);
			D3DXVECTOR3 ToNormal3D();

			color() {};
			~color() {};
		};

		const color operator+(color const &a,color const &b);
		const color operator*(color const &a, float fValue);

		class colorG16R16  
		{
		public:
			union
			{
				struct
				{
					unsigned short g,r;
				};
				long c;
			};
			colorG16R16( unsigned short ir, unsigned short ig);
			colorG16R16( long lnColor );

			colorG16R16() {};
			~colorG16R16() {};
		};

		const colorG16R16 operator+(colorG16R16 const &a,colorG16R16 const &b);
		const colorG16R16 operator*(colorG16R16 const &a, float fValue);

	};

}/* end picasso namespace */
}/* end picasso_math namespace */
