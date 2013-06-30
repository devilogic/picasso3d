#include "stdafx.h"

#include "RMaxDataHelper.h"
#include ".\rmaxdatahelper.h"

string RHelper::m_strTextureName = "";

bool RHelper::IsMesh( INode *pNode )
{
	if( pNode == NULL ) return false;

	ObjectState os = pNode->EvalWorldState( 0 );
	if( !os.obj ) return false;

	if( os.obj->SuperClassID() == GEOMOBJECT_CLASS_ID )
		return true;
	return false;
}
bool RHelper::IsBone( INode *pNode )
{
	if( pNode == NULL ) return false;

	ObjectState os = pNode->EvalWorldState( 0 );
	if( !os.obj ) return false;

	if( os.obj->ClassID() == Class_ID( BONE_CLASS_ID, 0 ) )
		return true;

	if( os.obj->ClassID() == Class_ID( DUMMY_CLASS_ID, 0 ) )
		return true;

	/*

	Control* cont = pNode->GetTMController();
	if( cont->ClassID() == BIPSLAVE_CONTROL_CLASS_ID || cont->ClassID() == BIPBODY_CONTROL_CLASS_ID )
		return true;
	// 从名字判断
	TSTR name= pNode->GetName();
	char temp[ 100 ];
	strcpy( temp, name );
	if( strncmp( temp, "Bip0", sizeof( char ) * 4 ) == 0 )
		return true;
	else if( strncmp( temp, "bip0", sizeof( char ) * 4 ) == 0 )
		return true;
	else if( strncmp( temp, "H_", sizeof( char ) * 2 ) == 0 )
		return true;
	else if( strncmp( temp, "bone", sizeof( char ) * 4 ) == 0 )
		return true;
	else if( strncmp( temp, "Bone", sizeof( char ) * 4 ) == 0 )
	*/
		return true;
	
	return false;
}

int RHelper::GetBoneCount( INode *pNode )
{
	int	nCount = 0;
	if( IsBone( pNode ) )
		nCount++;

	for( int cBone = 0; cBone < pNode->NumberOfChildren(); ++cBone )
		nCount += GetBoneCount( pNode->GetChildNode( cBone ) );

	return nCount;
}

int RHelper::GetChildBoneCount( INode *pNode )
{
	int nCount = 0;
	for( int cChild = 0; cChild < pNode->NumberOfChildren(); ++cChild )
	{
		if( IsBone( pNode->GetChildNode( cChild ) ) )
			nCount++;
	}

	return nCount;
}



int RHelper::GetBoneIndex( INode *pRoot, INode *pNode )
{
	if( IsBone( pNode ) == false )
		return -1;
	int nBoneCount = 0;
	return RecursiveGetBoneIndex( pRoot, pNode, nBoneCount );
}

int RHelper::RecursiveGetBoneIndex( INode *pRoot, INode *pNodeTest, int &cBone )
{
	int nBoneIndex = -1;
	if( IsBone( pRoot ) )
	{
		nBoneIndex = cBone;
		cBone++;
		if( pRoot == pNodeTest )
			return nBoneIndex;
	}

	for( int cChild = 0; cChild < pRoot->NumberOfChildren(); ++cChild )
	{
		nBoneIndex = RecursiveGetBoneIndex( pRoot->GetChildNode( cChild ), pNodeTest, cBone );
		if( nBoneIndex >= 0 )
			return nBoneIndex;
	}

	return -1;
}

// 如果pnode是骨骼节点则放入到Iterator中并且返回骨骼索引到currIdx中
bool RHelper::BuildIter (INode* pnode, INode** const Iterator, int& currIdx) {
	
	if(IsBone(pnode)){
		Iterator[currIdx++] = pnode;
	}

	// 遍历当前节点的所有子节点
	for(int i = 0; i < pnode->NumberOfChildren(); i++) {
		BuildIter (pnode->GetChildNode(i),Iterator,currIdx);
	}

	return true;
}

// 获取骨骼节点通过骨骼索引
INode* RHelper::GetBoneNodeByIndex( INode *pRoot, int nIndex )
{
	INode *pNode = NULL;
	const int nMaxBone = GetBoneCount( pRoot );
	if( nIndex >= nMaxBone ) return NULL;
	INode** const Iterator = new INode*[ nMaxBone ];
	int nCurIdx = 0;
	BuildIter( pRoot, Iterator, nCurIdx );
	assert( nCurIdx == nMaxBone );
	pNode = Iterator[ nIndex ];
	assert( GetBoneIndex( pRoot, pNode ) == nIndex );
	delete []Iterator;
	assert( IsBone( pNode ) );

	return pNode;
}

Matrix3 RHelper::GetBoneTM( INode *pNode, TimeValue t, bool bScale )
{
	Matrix3 tm( 1 );
	tm = pNode->GetNodeTM( t );
	//if( bScale == false )
		tm.NoScale();
	return tm;
}

Modifier* RHelper::GetPhysiqueMod( INode *pNode )
{
	Object *pObj = pNode->GetObjectRef();
	if( pObj == NULL ) return NULL;

	char szName[ 256 ];
	strcpy( szName, pNode->GetName() );

	/*

	while( pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID && pNode )
	{
		// Yes -> Cast.
		IDerivedObject *pDerivedObj = static_cast<IDerivedObject*>( pObj );

		// Iterate over all entries of the modifier stack.
		int ModStackIndex = 0;
		while( ModStackIndex < pDerivedObj->NumModifiers() )
		{
			// Get current modifier.
			Modifier* pMod = pDerivedObj->GetModifier( ModStackIndex );
			if( pMod->ClassID() == Class_ID( PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B ) )
				return pMod;

			// Next modifier stack entry.
			ModStackIndex++;
		}

		pObj = pDerivedObj->GetObjRef();
	}
	*/

	return NULL;
}

// 获取图元从对象中
TriObject* RHelper::GetTriObjectFromNode( INode *pNode, TimeValue t, bool &bDeleteIt )
{
	bDeleteIt = false;

	// 从某个时间获取矩阵
	Object *obj = pNode->EvalWorldState( t ).obj;
	if( obj->CanConvertToType( Class_ID( TRIOBJ_CLASS_ID, 0 ) ) )
	{ 
		// 获取某个时刻的图元对象
		TriObject *tri = (TriObject *) obj->ConvertToType( t, Class_ID(TRIOBJ_CLASS_ID, 0 ) );
		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if ( obj != tri ) 
			bDeleteIt = true;
		return tri;
	}
	else {

		return NULL;

	}
}

bool RHelper::TMNegParity( Matrix3 &m )
{
	// 叉乘两个个行向量后点乘一个行向量

	// 1.两个行向量叉积计算出这两个向量的法线指向
	// 2.与第三个行向量的点积计算之间的夹角cos值,如果小于0表示此角是钝角,则三角图元的朝向向后
	return (DotProd(CrossProd(m.GetRow(0),m.GetRow(1)),m.GetRow(2))<0.0) ? true : false ;
}

void RHelper::MAXtoD3D( Point3 &point )
{
	float tmp = point.y;
	point.y	= point.z;
	point.z	= tmp;
}

Point3 RHelper::MAXtoR3D( Point3 &point )
{
	return Point3( point.x, point.z, point.y );
}

void RHelper::MAXtoD3D( Matrix3 &m )
{
	// swap 2nd and 3rd rows
	Point3 row = m.GetRow( 1 );
	m.SetRow( 1, m.GetRow( 2 ) );
	m.SetRow( 2, row );

	// swap 2nd and 3rd columns
	Point4 column = m.GetColumn( 1 );
	m.SetColumn( 1, m.GetColumn( 2 ) );
	m.SetColumn( 2, column );
}

void RHelper::MAXtoD3D( const Matrix3 &m, float d3dTM[ 16 ] )
{
	Matrix3 maxTM = m;
	MAXtoD3D(maxTM);

	Point3 row = maxTM.GetRow( 0 );
	d3dTM[ 0 ] = row.x;
	d3dTM[ 1 ] = row.y;
	d3dTM[ 2 ] = row.z;
	d3dTM[ 3 ] = 0.f;

	row = maxTM.GetRow( 1 );
	d3dTM[ 4 ] = row.x;
	d3dTM[ 5 ] = row.y;
	d3dTM[ 6 ] = row.z;
	d3dTM[ 7 ] = 0.f;

	row = maxTM.GetRow( 2 );
	d3dTM[ 8 ] = row.x;
	d3dTM[ 9 ] = row.y;
	d3dTM[ 10 ] = row.z;
	d3dTM[ 11 ] = 0.f;

	row = maxTM.GetRow( 3 );
	d3dTM[ 12 ] = row.x;
	d3dTM[ 13 ] = row.y;
	d3dTM[ 14 ] = row.z;
	d3dTM[ 15 ] = 1.f;
}

void RHelper::MAXtoD3D( const Matrix3 &m, Matrix3 & mOut )
{
	Matrix3 maxTM = m;
	MAXtoD3D(maxTM);

	mOut = maxTM;
}


bool RHelper::EqualPoint3( Point3 p1, Point3 p2 )
{
	const float ALMOST_ZERO	= 1.0e-3f;
	if( fabs( p1.x - p2.x ) > ALMOST_ZERO )
		return false;
	if( fabs( p1.y - p2.y ) > ALMOST_ZERO )
		return false;
	if( fabs( p1.z - p2.z ) > ALMOST_ZERO )
		return false;
	return true;
}
bool RHelper::IsMatrialTextureMapDiffuse( Class_ID cid, int subNo )
{	
	if( cid == Class_ID(0,0) )
	{
		return false;
	}
	else if( cid == Class_ID( DMTL_CLASS_ID, 0 ) ) 
	{
		// 此材质为漫射纹理
		if( subNo == ID_DI )
		{
			return true;
		}
		/*
		switch (subNo) {
			
			case ID_AM: strcpy(buf, ID_MAP_AMBIENT); break;
			case ID_DI: strcpy(buf, ID_MAP_DIFFUSE); break;
			case ID_SP: strcpy(buf, ID_MAP_SPECULAR); break;
			case ID_SH: strcpy(buf, ID_MAP_SHINE); break;
			case ID_SS: strcpy(buf, ID_MAP_SHINESTRENGTH); break;
			case ID_SI: strcpy(buf, ID_MAP_SELFILLUM); break;
			case ID_OP: strcpy(buf, ID_MAP_OPACITY); break;
			case ID_FI: strcpy(buf, ID_MAP_FILTERCOLOR); break;
			case ID_BU: strcpy(buf, ID_MAP_BUMP); break;
			case ID_RL: strcpy(buf, ID_MAP_REFLECT); break;
			case ID_RR: strcpy(buf, ID_MAP_REFRACT); break;
			
		}
		*/
	}	
	
	return false;
}

string RHelper::GetFilename( string strData )
{	
	int nSize = (int)strlen( strData.c_str() ) + 1;
	char *pStrTemp = (char*)( _alloca( nSize * sizeof( char ) ) );
	strcpy( pStrTemp, strData.c_str() );

	char *p = strrchr( pStrTemp, '/' );
	if( p == NULL )
	{
		p = strrchr( pStrTemp, '\\' );
		if( p == NULL )
		{
			string strRes = pStrTemp;
			return strRes;		 
		}
	}
	p++;
	string strRes = p;
	return strRes;
}

void RHelper::ReplaceExt( string & strData, char *pExt )
{
	char strTemp[MAX_PATH];
	strcpy( strTemp, strData.c_str() );

	char *p = strrchr( strTemp, '.' );	 
	if( p == NULL )
		return;
	p++;
	strcpy(p, pExt);

	strData = strTemp;
}

string RHelper::GetDiffuseTextureMap( Mtl* pMtl )
{
	m_strTextureName = "";
	if( pMtl )
	{
		// 遍历子纹理
		for( int cTexture = 0; cTexture < pMtl->NumSubTexmaps(); cTexture++ )
		{
			Texmap *pSubTex = pMtl->GetSubTexmap( cTexture );			

			if( pSubTex )
			{
				// 确定是否是漫射纹理图
				if( RHelper::IsMatrialTextureMapDiffuse( pMtl->ClassID(), cTexture ) )
				{	
					string strClassName;
					CStr str;
					pSubTex->GetClassName( str );										// 获取材质的类型
					if( strcmp( str, "Bitmap" ) == 0 )									// 如果是Bitmap
					{
						m_strTextureName= ( ( BitmapTex* )pSubTex )->GetMapName();		// 获取纹理图名称
						string strExtTextureName = GetFilename( m_strTextureName );
						ReplaceExt( strExtTextureName, "dds" );							// 转换为DDS的后缀

						m_strTextureName = strExtTextureName;
						return m_strTextureName;
					}
				}
			}
		}
	}
	return m_strTextureName;
}

// 获取点法线
Point3 RHelper::GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv)
{
	// 获取模型的第faceNo个面缓存
	Face* f = &mesh->faces[faceNo];
	DWORD smGroup = f->smGroup;					// 是否是平滑组
	int numNormals;
	Point3 vertexNormal;
	
	// Is normal specified
	// SPCIFIED is not currently used, but may be used in future versions.
	// 是正规法线
	if (rv->rFlags & SPECIFIED_NORMAL) {
		vertexNormal = rv->rn.getNormal();
	}
	// If normal is not specified it's only available if the face belongs
	// to a smoothing group
	// 如果此面是在一个平滑组内
	else if ((numNormals = rv->rFlags & NORCT_MASK) && smGroup) {
		// If there is only one vertex is found in the rn member.
		if (numNormals == 1) {
			vertexNormal = rv->rn.getNormal();
		}
		else {
			// If two or more vertices are there you need to step through them
			// and find the vertex with the same smoothing group as the current face.
			// You will find multiple normals in the ern member.
			for (int i = 0; i < numNormals; i++) {
				if (rv->ern[i].getSmGroup() & smGroup) {
					vertexNormal = rv->ern[i].getNormal();
				}
			}
		}
	}
	else {
		// Get the normal from the Face if no smoothing groups are there
		vertexNormal = mesh->getFaceNormal(faceNo);
	}
	
	return Point3( vertexNormal.x, vertexNormal.y, vertexNormal.z );
}

// 是否是光节点
bool RHelper::IsLight( INode *pNode )
{
	if( pNode == NULL ) return false;

	// 对象0秒的状态
	ObjectState os = pNode->EvalWorldState( 0 );
	if( !os.obj ) return false;

	if( os.obj->SuperClassID() == LIGHT_CLASS_ID )
		return true;
	return false;
}

// 通过模型名字获取广告牌的类型
int RHelper::GetBillboardType( char *szMeshName )
{
	if( strncmp( szMeshName, "e_nb_", 5 ) == 0 )
		return REBT_NO;
	else if( strncmp( szMeshName, "e_bxyz_", 7 ) == 0 )
		return REBT_XYZ;
	else if( strncmp( szMeshName, "e_bx_", 5 ) == 0 )
		return REBT_X;
	else if( strncmp( szMeshName, "e_by_", 5 ) == 0 )
		return REBT_Y;
	else if( strncmp( szMeshName, "e_bz_", 5 ) == 0 )
		return REBT_Z;
	else if( strncmp( szMeshName, "e_bxy_", 6 ) == 0 )
		return REBT_XY;
	else if( strncmp( szMeshName, "e_bxz_", 6 ) == 0 )
		return REBT_XZ;
	else if( strncmp( szMeshName, "e_byz_", 6 ) == 0 )
		return REBT_YZ;
	return REBT_NO;
}

// 通过模型名字获取动画类型
int RHelper::GetAnimationType( char *szMeshName )
{
	// Mesh Animation
	if( strstr( szMeshName, "_ma!" ) != 0 )
	{
		return ( REAT_KEYFRAME | REAT_MESH );
	}
	else if( strstr( szMeshName, "_ta!" ) != 0 )
	{
		return ( REAT_KEYFRAME | REAT_TEXTURE );
	}
	else if( strstr( szMeshName, "_mta!" ) != 0 || strstr( szMeshName, "_tma!" ) != 0  )
	{
		int nResult = REAT_KEYFRAME | REAT_MESH | REAT_TEXTURE;
		return nResult;
	}
	else if ( strstr( szMeshName, "_ba!" ) != 0 )
	{
		int nResult = REAT_KEYFRAME | REAT_BONE;
		return nResult;
	}

	return REAT_KEYFRAME;
}


void RHelper::Matrix3ToFloat16( const Matrix3 & m, float fOut[ 16 ] )
{
	Point3 pt = m.GetRow( 0 );
	fOut[ 0 ] = pt.x; fOut[ 1 ] = pt.y; fOut[ 2 ] = pt.z; fOut[ 3 ] = 0.0f;
	pt = m.GetRow( 1 );
	fOut[ 4 ] = pt.x; fOut[ 5 ] = pt.y; fOut[ 6 ] = pt.z; fOut[ 7 ] = 0.0f;
	pt = m.GetRow( 2 );
	fOut[ 8 ] = pt.x; fOut[ 9 ] = pt.y; fOut[ 10 ] = pt.z; fOut[ 11 ] = 0.0f;
	pt = m.GetRow( 3 );
	fOut[ 12 ] = pt.x; fOut[ 13 ] = pt.y; fOut[ 14 ] = pt.z; fOut[ 15 ] = 1.0f;
}