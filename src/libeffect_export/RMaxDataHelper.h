#ifndef __RMAX_EXPORTER_HELPER__H
#define __RMAX_EXPORTER_HELPER__H

// 效果绘制类型
enum REffectRenderType 
{
	RERT_NONE		= 0,
	RERT_NORMAL,
	RERT_ADD,

	RERT_ADD_A,
	RERT_ADD_B,
	RERT_ADD_C,
	RERT_ADD_D,
	RERT_ADD_E,

	RERT_SUB_A,
	RERT_SUB_B,
	RERT_SUB_C,
	RERT_SUB_D,
	RERT_SUB_E,

	RERT_RESUB_A,
	RERT_RESUB_B,
	RERT_RESUB_C,
	RERT_RESUB_D,
	RERT_RESUB_E
};

// 效果广告牌类型
enum REffectBillboardType
{
	REBT_NO			 = 0,
	REBT_XYZ,
	REBT_X,
	REBT_Y,
	REBT_Z,
	REBT_XY,
	REBT_XZ,
	REBT_YZ
};

// 特效动画类型
enum REffectAnimationType
{
	REAT_KEYFRAME	= 0x00000001,			/*!< 关键帧 */
	REAT_TEXTURE	= 0x00000002,			/*!< 纹理 */
	REAT_MESH		= 0x00000004,			/*!< 网格模型 */
	REAT_BONE		= 0x00000008,			/*!< 骨骼 */
};

class RHelper
{
public:
	static bool					IsBone( INode *pNode );																/*!< 是骨骼 */
	static bool					IsLight( INode *pNode );															/*!< 是光 */
	static bool					IsMesh( INode *pNode );																/*!< 是模型 */
	static int					GetBoneCount( INode *pNode );														/*!< 获取模型的骨骼数量 */
	static int					GetChildBoneCount( INode *pNode );													/*!< 获取模型的子骨骼数量 */
	static int					GetBoneIndex( INode *pRoot, INode *pNode );											/*!< 获取骨骼索引 */
	static INode*				GetBoneNodeByIndex( INode *pRoot, int nIndex );										/*!< 获取骨骼节点通过索引 */
	static Matrix3				GetBoneTM( INode *pNode, TimeValue t, bool bScale = true );							/*!< 获取骨骼变换矩阵 */
	static Modifier*			GetPhysiqueMod( INode *pNode );														/*!< 获取屋里修改器指针 */
	static TriObject*			GetTriObjectFromNode( INode *pNode, TimeValue t, bool &bDeleteIt );					/*!< 获取图元列表从节点指针 */
	static bool					TMNegParity( Matrix3 &m );															/*!< 取矩阵的反 */
	static void					MAXtoD3D( Point3 &point );															/*!< 转换MAX点到D3D类型 */
	static void					MAXtoD3D( Matrix3 &m );																/*!< 转换MAX矩阵到D3D类型 */
	static void					MAXtoD3D( const Matrix3 &m, float d3dTM[ 16 ] );									/*!< 转换MAX点到float矩阵类型 */
	static void					MAXtoD3D( const Matrix3 &m, Matrix3 & mOut );										/*!< 转换矩阵到矩阵类型 */
	static Point3 				MAXtoR3D( Point3 &point );															/*!< 转换MAX矩阵到D3D矩阵 */
	static bool					EqualPoint3( Point3 p1, Point3 p2 );												/*!< 两个点是否相等 */
	static bool					IsMatrialTextureMapDiffuse( Class_ID cid, int subNo );								/*!< 材质纹理颜色图 */
	static string				GetDiffuseTextureMap( Mtl* pMtl );													/*!< 获取颜色纹理图 */
	static string				GetFilename( string strData );														/*!< 获取文件名 */
	static void					ReplaceExt( string & strData, char *pExt );											/*!< 替换文件名后缀 */

	static Point3				GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv);								/*!< 获取顶点法线 */
	static int					GetBillboardType( char *szMeshName );												/*!< 获取广告牌类型 */
	static int					GetAnimationType( char *szMeshName );												/*!< 获取动画类型 */

	static void					Matrix3ToFloat16( const Matrix3 & m, float fOut[ 16 ] );							/*!< MAX矩阵到float矩阵 */

private:
	static int					RecursiveGetBoneIndex( INode *pRoot, INode *pNodeTest, int &cBone );				/*!< 递归获取骨骼索引 */
	static bool					BuildIter (INode* pnode, INode** const Iterator, int& currIdx );					/*!< 建立 */

public:
	static string				m_strTextureName;																	/*!< 纹理图名称 */
	
	
};

class color  
{
public:
	union
	{
		struct
		{
			unsigned char b,g,r,a;
		};
		long c;
	};

	color::color( unsigned char ia, unsigned char ir, unsigned char ig, unsigned char ib ) : a( ia ), r( ir ), g( ig ), b( ib ) {}
	color( long lnColor ) : c( lnColor ) {}

	color() {};
	~color() {};
};

// 增加数据到列表,不得重复添加
template< class T >
void AddUniqueVector( vector<T> &vecData, T pData )
{
	for ( UINT i=0; i < vecData.size(); i++ )
	{
		if ( pData == vecData[i] )
			return;
	}
	vecData.push_back( pData );
}

/*!	\fn FindVector
 *	\brief 寻找向量并返回索引
 */
template< class T >
int FindVector( vector<T> &vecData, T pData )
{
	for ( UINT i=0; i < vecData.size(); i++ )
	{
		if ( pData == vecData[i] )
			return i;
	}
	return -1;
}

#endif
