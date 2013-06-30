//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "stdafx.h"
#include "Red3DMaxEffectUtil2008.h"
#include "RMaxDataHelper.h"
#include "LogFilePort.h"

#define Red3DMaxEffectUtil2008_CLASS_ID	Class_ID(0xc996058b, 0x7d7137bd)		/*!< 此输出控件的ID */	

const int MAX_NODE					= 50;										/*!< 一个节点最多有50个图元 */

/*!	\class Red3DMaxEffectUtil2008
 *	\brief 特效输出功能
 */
class Red3DMaxEffectUtil2008 : public UtilityObj 
{
	/*!	\class REffectVertex
	 *	\brief 特效输出顶点
	 */
	class REffectVertex
	{
	public:
		Point3			m_vecPosition;						/*!< 位置 */
		Point2			m_vecTextureCoordinate;				/*!< 纹理坐标0-1范围内 */
		color			m_vecNormal;						/*!< 法线方向 */
		color			m_vecTangentU;						/*!< 切线U */
		color			m_vecTangentV;						/*!< 切线V */
	};

	/*!	\class RTextureMat
	 *	\brief 纹理材质结构
	 */
	class RTextureMat
	{
	public:		
		string			m_strTextureName;					/*!< 纹理名称 */
		int				m_nMatID;							/*!< 材质ID */
		int				m_nMatSubID;						/*!< 子材质ID */
	};	

	/*!	\class RMeshAnimation
	 *	\brief 模型动画结构
	 */
	class RMeshAnimation
	{
	public:		
		vector< Point3 > m_lstPosition;						/*!< 位置信息 */
		vector< Point2 > m_lstTextureCoord;					/*!< 纹理坐标 */
		vector< color >	m_lstColor;							/*!< 颜色队列 */
		vector< color >	m_lstNormal;						/*!< 法线队列 */
		vector< color >	m_lstTangentU;						/*!< 切线U列表 */
		vector< color >	m_lstTangentV;						/*!< 切线V列表 */
	};

	/*!	\struct RWeight
	 *	\brief 顶点权重
	 */
	struct RWeight
	{
		vector< string >	vecBoneName;					/*!< 骨骼名称 */
		vector< int >		vecBoneIndex;					/*!< 骨骼索引 */
		vector< float >		vecWeight;						/*!< 骨骼权重 */
	};

	/*!	\class RMesh
	 *	\brief 模型类型
	 */
	class RMesh
	{
	public:
		INode			*m_pNode;							/*!< 节点指针 */
		INode			*m_pParent;							/*!< 父节点指针 */

		int				m_nNode;							/*!< 节点索引 */
		int				m_nParent;							/*!< 父节点索引 */
		int				m_nVertexCount;						/*!< 顶点计数 */
		int				m_nIndexCount;						/*!< 索引计数 */
		int				m_nAnimationType;					/*!< 动画类型 */
		int				m_nBillboardType;					/*!< 广告牌类型 */
		bool			m_bMeshAnimation;					/*!< 是否是模型动画 */

		char			m_szName[ 64 ];						/*!< 模型网格名称 */

		Matrix3			m_matWorld;							/*!< 世界矩阵 */
		Matrix3			m_matLocal;							/*!< 本地矩阵 */
		Matrix3			m_matInvWorld;						/*!< 世界逆矩阵 */

		vector< int >	m_lstTextureID;						/*!< 纹理ID列表 */
		vector< int >	m_lstVertexCount;					/*!< 顶点计数列表 */
		vector< int >	m_lstVertexOffset;					/*!< 顶点偏移列表 */
		vector< int >	m_lstIndexCount;					/*!< 索引计数列表 */
		vector< int >	m_lstIndexOffset;					/*!< 索引偏移列表 */

		vector< Point3 > m_lstPosition;						/*!< 位置列表 */
		vector< Point2 > m_lstTextureCoord;					/*!< 纹理坐标列表 */
		vector< color >	m_lstColor;							/*!< 颜色列表 */
		vector< color >	m_lstNormal;						/*!< 法线列表 */
		vector< color >	m_lstTangentU;						/*!< 切线U列表 */
		vector< color >	m_lstTangentV;						/*!< 切线V列表 */

		vector< WORD >	m_lstIndex;							/*!< 索引列表 */

		vector< Point3 >	m_lstAnimationPositionKey;		/*!< 动画关键帧位置列表 */
		vector< Point4 > m_lstAnimationRotateKey;			/*!< 动画关键帧旋转列表 */
		vector< Point3 >	m_lstAnimationScaleKey;			/*!< 动画关键帧比例列表 */
		vector< float >	m_lstAnimationVisibilityKey;		/*!< 动画关键帧可见度列表 */
		
		vector< RMeshAnimation* >	m_lstMeshAnimation;		/*!< 网格动画列表 */

		int					m_iUsedBoneCount;				/*!< 使用骨骼计数 */
		vector< RWeight >	m_vecWeightList;				/*!< 权重值列表 */

		RMesh() : m_iUsedBoneCount(0) { m_bMeshAnimation = false; }
		~RMesh() {
			// 如果是网格动画,则遍历并释放内存
			if( m_bMeshAnimation )
			{
				for( UINT cAnimation = 0; cAnimation < m_lstMeshAnimation.size(); cAnimation++ )
				{
					delete m_lstMeshAnimation[ cAnimation ];
				}
			}
		}

	};

	/*!	\struct BoneData
	 *	\brief 骨骼数据
	 */
	struct BoneData
	{
		BoneData() : pNode(NULL), iParentID(-1) {}
		string				strBoneName;					/*!< 骨骼名称 */
		int					iParentID;						/*!< 父骨骼ID */
		INode *				pNode;							/*!< 指向Object指针 */

		Matrix3				matWorld;						/*!< 世界矩阵 */
		Matrix3				matLocal;						/*!< 局部矩阵 */

		vector< Point3 >	vecPositionList;				/*!< 位置列表 */
		vector< Point4 >	vecRotationList;				/*!< 旋转列表 */
		vector< Point3 >	vecScaleList;					/*!< 比例列表 */
	};

	vector< Mtl* >		m_lstMAXMatrial;					/*!< 材质列表 */
	vector< RTextureMat > m_lstTexture;						/*!< 纹理列表,保存了纹理结构 */

	vector< RMesh* >	m_lstMesh;							/*!< 模型列表 */
	vector< string >	m_lstSaveTextureName;				/*!< 保存纹理图名称列表 */
	vector< Point3 >	m_lstSavePosition[ MAX_NODE ];		/*!< 图元位置信息 */
	vector< Point3 >	m_lstSaveNormal[ MAX_NODE ];		/*!< 图元法线信息 */
	vector< Point2 >	m_lstSaveTexture[ MAX_NODE ];		/*!< 图元纹理坐标信息 */
    vector< color >		m_lstSaveColor[ MAX_NODE ];			/*!< 图元颜色信息 */
	vector< WORD >		m_lstSaveIndex[ MAX_NODE ];			/*!< 图元顶点索引信息 */
	vector< RWeight >	m_lstSaveWeight[ MAX_NODE ];		/*!< 图元权重信息 */

	// 以下向量标志了动画的最终输出
    vector< Point3 >	m_lstFinalPosition;
	vector< Point3 >	m_lstFinalNormal;
	vector< Point2 >	m_lstFinalTexture;
	vector< color >		m_lstFinalColor;
	vector< WORD >		m_lstFinalIndex;
	vector< string >	m_lstFinalTextureName;				/*!< 仅保存的纹理图 */

	int					m_nFirstFrame;						/*!< 第一帧 */
	int					m_nLastFrame;						/*!< 最后帧 */
	int					m_nFrameSpeed;						/*!< 帧速 */
	int					m_nTicksPerFrame;					/*!< 帧率 */

	bool				m_bVertexAni;						/*!< 是否是顶点动画 */

	vector< BoneData * >		m_vecBoneList;				/*!< 骨骼列表 */
public:
		
	//Constructor/Destructor
	Red3DMaxEffectUtil2008();
	virtual ~Red3DMaxEffectUtil2008();

	virtual void DeleteThis() { };		
	
	virtual void BeginEditParams(Interface *ip,IUtil *iu);
	virtual void EndEditParams(Interface *ip,IUtil *iu);

	virtual void Init(HWND hWnd);							/*!< 初始化 */
	virtual void Destroy(HWND hWnd);						/*!< 销毁 */

	void				Cleanup();							/*!< 清除 */
	void				Export();							/*!< 输出 */

private:

	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND			hPanel;
	IUtil			*iu;
	Interface		*ip;

	/*!	\fn MakeMaterialList
	 *	\param pNode 节点指针
	 *	\brief 产生材质表
	 */
	bool				MakeMaterialList( INode *pNode );

	/*!	\fn MakeTextureList
	 *	\brief 产生纹理列表
	 */
	bool				MakeTextureList(void);

	/*!	\fn ExportMesh
	 *	\param pNode 节点指针
	 *	\brief 输出网格模型
	 */
	void				ExportMesh( INode *pNode );

	/*!	\fn GetMeshID
	 *	\param pNode 节点指针
	 *	\brief 获取网格模型ID
	 */
	int					GetMeshID( INode *pNode );

	/*!	\fn MakeR3DMesh
	 *	\param pMesh 网格模型
	 *	\brief 产生R3D网格模型
	 */
	void				MakeR3DMesh( RMesh *pMesh );

	/*!	\fn MakeR3DMeshAnimation
	 *	\param pMesh 网格动画模型
	 *	\brief 产生R3D网格模型动画
	 */
	void				MakeR3DMeshAnimation( RMeshAnimation *pMesh );

	/*!	\fn MakeSaveVertex
	 *	\param pPosition 顶点位置队列
	 *	\param pNormal 法线队列
	 *	\param pTexture 纹理队列
	 *	\param pColor 颜色队列
	 *	\param pWeight 权重队列
	 *	\param pMtl 材质队列
 	 *	\param nSubMat 子材质ID
	 *	\brief 保存顶点信息
	 */
	bool				MakeSaveVertex( Point3 *pPosition, Point3 *pNormal, Point2 *pTexture, color *pColor, RWeight * pWeight, Mtl *pMtl, int nSubMat );

	/*!	\fn ClearSaveBuffers
	 *	\brief 清除保存缓存
	 */
	void				ClearSaveBuffers(void);

	/*!	\fn ExportAnimationElement
	 *	\brief 输出动画元素
	 */
	void				ExportAnimationElement(void);

	/*!	\fn ExportAnimationMesh
	 *	\param pNode 节点指针
	 *	\param t 当前节点的时间序号
 	 *	\param iAniType 动画类型
	 *	\brief 输出动画元素网格
	 */
	void				ExportAnimationMesh( INode *pNode, TimeValue t, int iAniType );
	
	/*!	\fn MakeAnimationList
	 *	\brief 产生动画列表
	 */
	void				MakeAnimationList(void);

	/*!	\fn MakeGeometry
	 *	\brief 产生图元
	 */
	bool				MakeGeometry(void);

	/*!	\fn FillNodeList
	 *	\param pNode 节点指针
 	 *	\param lstNodeList 节点队列
	 *	\brief 填充节点列表
	 */
	void				FillNodeList( INode *pNode, vector< INode* > &lstNodeList );

	/*!	\fn SaveEffectFile
	 *	\param strFilename 效果文件名
	 *	\brief 保存效果文件
	 */
	void				SaveEffectFile( string strFilename );

	/*!	\fn GetSaveFileNamePath
	 *	\brief 获取保存文件路径
	 */
	string				GetSaveFileNamePath(void);

	/*!	\fn SetVertexWeightList
	 *	\param vecWeightList 权重列表
 	 *	\param pNode 节点指针
	 *	\brief 保存顶点权重列表
	 */
	int					SetVertexWeightList( vector< RWeight > & vecWeightList, INode * pNode );

	/*!	\fn GetPhysiqueMod
	 *	\param pNode 节点指针
 	 *	\return 修改器指针
	 *	\brief 获取修改器指针
	 */
	Modifier *			GetPhysiqueMod( INode *pNode );

	/*!	\fn GetBoneID
	 *	\param pNode 节点指针
 	 *	\return 获取当前节点的骨骼ID
	 *	\brief 获取骨骼ID
	 */
	int					GetBoneID( INode * pNode );

	/*!	\fn GetSkinMod
	 *	\param pNode 节点指针
 	 *	\return 皮肤修改器指针
	 *	\brief 获取皮肤修改器
	 */
	Modifier *			GetSkinMod( INode * pNode );

	/*!	\fn IsBone
	 *	\param pNode 节点指针
	 *	\brief 是否是骨骼节点
	 */
	BOOL				IsBone(INode* pNode);

	/*!	\fn ExportBone
	 *	\param pNode 节点指针
	 *	\brief 输出骨骼
	 */
	void				ExportBone( INode *pNode );

	/*!	\fn SetAnimationKeyList
	 *	\param pBoneData 骨骼数据指针
	 *	\param pNode 节点指针
	 *	\brief 设置关键动画列表
	 */
	void				SetAnimationKeyList( BoneData * pBoneData, INode * pNode );

	//////////////////////////////////////////////////////////////////////////
	// 写入XML文件
	void r3dWriteFloatMatrixToXML( FILE *fp, float *fMatrix );
	void r3dWriteEffectTextureToXML( FILE *fp, vector< string > &lstTexture );
	bool r3dWriteEffectMeshToXML( FILE *fp, vector< Red3DMaxEffectUtil2008::RMesh* > &lstMesh );
	void r3dWriteEffectBoneToXML(FILE *fp, vector< BoneData * > &lstBone, bool bBoneType);
	void r3dWriteEffectXML( string strFileName );
	//////////////////////////////////////////////////////////////////////////
};

static Red3DMaxEffectUtil2008 theRed3DMaxEffectUtil2008;


class Red3DMaxEffectUtil2008ClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 	{ return &theRed3DMaxEffectUtil2008; }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return UTILITY_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return Red3DMaxEffectUtil2008_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("Red3DMaxEffectUtil2008"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};

static Red3DMaxEffectUtil2008ClassDesc Red3DMaxEffectUtil2008Desc;
ClassDesc2* GetRed3DMaxEffectUtil2008Desc() { return &Red3DMaxEffectUtil2008Desc; }

void OnCommand( HWND hWnd, WPARAM wParam, LPARAM lParam );


//--- Red3DMaxEffectUtil2008 -------------------------------------------------------
Red3DMaxEffectUtil2008::Red3DMaxEffectUtil2008() : m_bVertexAni(true)
{
	iu = NULL;
	ip = NULL;	
	hPanel = NULL;
}

Red3DMaxEffectUtil2008::~Red3DMaxEffectUtil2008()
{
	Cleanup();
}

void Red3DMaxEffectUtil2008::Cleanup()
{
	// 清除模型的所有内存
	if ( !m_lstMesh.empty() )
	{
		for ( int i = 0; i < (int) m_lstMesh.size(); ++i )
		{
			if ( m_lstMesh[ i ] )
				delete m_lstMesh[ i ];
		}
		m_lstMesh.clear();
	}

	// 清除骨骼列表内存
	if ( !m_vecBoneList.empty() )
	{
		for ( int i = 0; i < (int) m_vecBoneList.size(); ++i )
		{
			if ( m_vecBoneList[ i ] )
				delete m_vecBoneList[ i ];
		}
		m_vecBoneList.clear();
	}
}

void Red3DMaxEffectUtil2008::BeginEditParams(Interface* ip,IUtil* iu) 
{
	this->iu = iu;
	this->ip = ip;
	hPanel = ip->AddRollupPage(
		hInstance,
		MAKEINTRESOURCE(IDD_PANEL),
		DlgProc,
		GetString(IDS_PARAMS),
		0);
}
	
void Red3DMaxEffectUtil2008::EndEditParams(Interface* ip,IUtil* iu) 
{
	this->iu = NULL;
	this->ip = NULL;
	ip->DeleteRollupPage(hPanel);
	hPanel = NULL;
}

void Red3DMaxEffectUtil2008::Init(HWND hWnd)
{
	int AssertMode = 0;
	int ErrortMode = 0;
	AssertMode |= _CRTDBG_MODE_DEBUG;
	ErrortMode |= _CRTDBG_MODE_DEBUG;
	AssertMode |= _CRTDBG_MODE_WNDW;
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ASSERT, AssertMode );
	_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode( _CRT_ERROR, ErrortMode );
	_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode( _CRT_WARN, ErrortMode );
	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDERR);
}

void Red3DMaxEffectUtil2008::Destroy(HWND hWnd)
{

}

INT_PTR CALLBACK Red3DMaxEffectUtil2008::DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
	{
		case WM_INITDIALOG:
			theRed3DMaxEffectUtil2008.Init(hWnd);
			break;

		case WM_DESTROY:
			theRed3DMaxEffectUtil2008.Destroy(hWnd);
			break;

		case WM_COMMAND:
			OnCommand( hWnd, wParam, lParam );
			break;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
			theRed3DMaxEffectUtil2008.ip->RollupMouseMessage(hWnd,msg,wParam,lParam); 
			break;

		default:
			return 0;
	}
	return 1;
}

void OnCommand( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	switch( LOWORD( wParam ) )
	{
		case IDC_DOEXPORT:
			{		
				/*! < 唯一的输出 */
				theRed3DMaxEffectUtil2008.Export();				
			}
			break;
	}
}

/*!	\fn ComputeTangentVector
 *	\param vecPosA 点A位置
 *	\param vecPosB 点B位置
 *	\param vecPosC 点C位置
 *	\param vecTexA 点A纹理位置
 *	\param vecTexB 点B纹理位置
 *	\param vecTexC 点C纹理位置
 *	\param vecS 切线向量U,输出
 *	\param vecT 切线向量V,输出
 *	\brief 计算切线向量
 */
bool ComputeTangentVector( Point3 vecPosA, Point3 vecPosB, Point3 vecPosC, Point2 vecTexA, Point2 vecTexB, Point2 vecTexC, Point3 *vecS, Point3 *vecT )	
{
	// 计算两个向量AB,AC
	Point3 vAB = vecPosB - vecPosA;
	Point3 vAC = vecPosC - vecPosA;

	// 叉积计算法线方向
	Point3 vecNormal = ( vecPosB - vecPosA ) ^ ( vecPosC - vecPosB );
	vecNormal.Normalize();		// 法线标准化

	Point3 n = vecNormal;

	// 法线n与向量AB的点积
	Point3 vProjAB = vAB - ( ( n % vAB ) * n );
	Point3 vProjAC = vAC - ( ( n % vAC ) * n );

	// 两条在2维空间的向量坐标
	float duAB = vecTexB.x - vecTexA.x;
	float duAC = vecTexC.x - vecTexA.x;

	float dvAB = vecTexB.y - vecTexA.y;
	float dvAC = vecTexC.y - vecTexA.y;

	bool bCheck = false;

	// 两个向量的点积, 计算两个向量的点积
	if( duAC * dvAB > duAB * dvAC )
	{
		//duAC = -duAC;
		//duAB = -duAB;

		bCheck = true;
	}
	
	// 切线向量
	Point3 vTangent = duAC * vProjAB - duAB * vProjAC;
	vTangent.Normalize();		// 切线向量标准化

	// 检查
	if( bCheck )
	{
		vecS[ 0 ] = -vTangent;
		vecS[ 1 ] = -vTangent;
		vecS[ 2 ] = -vTangent;
	}
	else	
	{
		vecS[ 0 ] = vTangent;
		vecS[ 1 ] = vTangent;
		vecS[ 2 ] = vTangent;
	}

	/*
	if( duAC * dvAB > duAB * dvAC )
	{
		dvAC = -dvAC;
		dvAB = -dvAB;
	}
	*/

	vTangent = dvAC * vProjAB - dvAB * vProjAC;
	vTangent.Normalize();
	
	if( bCheck )
	{
		vecT[ 0 ] = -vTangent;
		vecT[ 1 ] = -vTangent;
		vecT[ 2 ] = -vTangent;
	}
	else	
	{
		vecT[ 0 ] = vTangent;
		vecT[ 1 ] = vTangent;
		vecT[ 2 ] = vTangent;
	}

	if( bCheck )
	{
		int a=0;
	}

	return bCheck;
}

/*!	\fn Export
 *	\brief 输出
 */
void Red3DMaxEffectUtil2008::Export()
{
	Cleanup();

	m_lstMAXMatrial.resize( 0 );
	m_lstTexture.resize( 0 );

	m_lstMesh.resize( 0 );
	m_lstSaveTextureName.resize( 0 );

	m_lstFinalPosition.resize( 0 );
	m_lstFinalNormal.resize( 0 );
	m_lstFinalTexture.resize( 0 );
	m_lstFinalColor.resize( 0 );
	m_lstFinalIndex.resize( 0 );
	m_lstFinalTextureName.resize( 0 );

	// 清楚缓存
	ClearSaveBuffers();
	
	string strFilename = GetSaveFileNamePath();
	if ( strFilename == "" )
		return;

	// 动画时间相关
	Interval range = ip->GetAnimRange();									// 得到动画范围
	m_nFirstFrame = ( int ) ( range.Start() / GetTicksPerFrame() );			// 得到第一帧
	m_nLastFrame = ( int ) ( range.End() / GetTicksPerFrame() );			// 得到最后一帧
	m_nFrameSpeed = ( int ) ( GetFrameRate() );								// 得到帧速度
	m_nTicksPerFrame = ( int ) ( GetTicksPerFrame() );						// 得到每帧所花的Tick

	// 材质
	MakeMaterialList( ip->GetRootNode() );									// 添加材质

	// 纹理
	MakeTextureList();														// 通过材质添加纹理

	// 获取图元
	if ( MakeGeometry() == NULL )
	{
		MessageBox( NULL, "Exportlist empty", "max2008", MB_OK );
		return;
	}
	// 动画
	MakeAnimationList();

	// 保存为效果文件
	SaveEffectFile( strFilename );	
}

bool Red3DMaxEffectUtil2008::MakeMaterialList( INode *pNode )
{
	Mtl *pMtl = pNode->GetMtl();

	if( pMtl )
	{
		AddUniqueVector(m_lstMAXMatrial, pMtl );								// 纹理材质只能是唯一
	}

	for( int cNode = 0; cNode < pNode->NumberOfChildren(); cNode++ )
		MakeMaterialList( pNode->GetChildNode( cNode ) );	

	return true;
}

bool Red3DMaxEffectUtil2008::MakeTextureList(void)
{
	m_lstFinalTextureName.resize( 0 );

	// 遍历材质表
	for( UINT cMat=0; cMat < m_lstMAXMatrial.size(); cMat++ )
	{
		RTextureMat AddNode;

		AddNode.m_nMatID = cMat;																		// 材质ID
		AddNode.m_nMatSubID = -1;																		// 材质子ID
		AddNode.m_strTextureName = RHelper::GetDiffuseTextureMap( m_lstMAXMatrial[ cMat ] );			// 获取纹理路径	

		// 如果节点的纹理图不为空则压入
		if( !( AddNode.m_strTextureName == "" ) )
			m_lstTexture.push_back( AddNode );

		// 获取子材质纹理
		int nSubMat = m_lstMAXMatrial[ cMat ]->NumSubMtls();
		for( int cSubMat=0; cSubMat < nSubMat; cSubMat++ )
		{
			AddNode.m_nMatID = cMat;
			AddNode.m_nMatSubID = cSubMat;
			AddNode.m_strTextureName = RHelper::GetDiffuseTextureMap( m_lstMAXMatrial[ cMat ]->GetSubMtl( cSubMat ) );
			if( !( AddNode.m_strTextureName == "" ) )
				m_lstTexture.push_back( AddNode );
		}
	}

	// 只储存纹理图的路径
	for( UINT cTexture = 0; cTexture < m_lstTexture.size(); cTexture++ )
		m_lstFinalTextureName.push_back( m_lstTexture[ cTexture ].m_strTextureName );
	return true;	
}

// 导出网格信息
void Red3DMaxEffectUtil2008::ExportMesh( INode *pNode )
{
	// 如果是骨骼则退出此节点
	if ( IsBone( pNode ) )
		return;

	char * szName = pNode->GetName();

	// 图元的名称中不能有"Nub"与"nub"的字符串
	if ( strstr( szName, "Nub" ) != NULL || strstr( szName, "nub" ) != NULL )
		return;

	// 获取材质指针,如果不存在材质则直接退出
	// 如果是特效皮肤节点则必须有材质
	Mtl *pMtl = pNode->GetMtl();
	if( pMtl == NULL )
		return;	

	// 去当前图元是否为反
	int v1, v2, v3;	
	bool bNegScale = RHelper::TMNegParity( pNode->GetObjectTM( 0 ) );

	// 顶点扰序改变
	if( bNegScale )
	{
		v1 = 0; v2 = 1; v3 = 2;
	}
	else
	{
		v1 = 2; v2 = 1; v3 = 0;
	}

	bool bNeedDel;

	// 从对象获取图元
	TriObject *pTri = RHelper::GetTriObjectFromNode( pNode, 0, bNeedDel );
	if( pTri == NULL )	
		return;

	// 从图元对象获取模型
	Mesh *pMesh = &pTri->GetMesh();
	pMesh->buildNormals();								// 计算顶点与面法线

	INode *pParent = pNode->GetParentNode();			// 获取对象的父对象
	INode *pRoot = ip->GetRootNode();					// 获取根节点

	// 生成一个RMesh内存
	RMesh *pMeshObject = new RMesh;
	pMeshObject->m_nNode = (int)m_lstMesh.size();		// 模型的索引
	pMeshObject->m_pNode = pNode;						// 对象节点指针

	strcpy( pMeshObject->m_szName, pNode->GetName() );	// 模型的名称

	// 其父节点不等于根节点获取父节点的ID
	if( pParent != pRoot )
		pMeshObject->m_nParent = GetMeshID( pNode->GetParentNode() );
	else	// 如果其父节点是根节点则设置父ID为-1
		pMeshObject->m_nParent = -1;	

	// 如果父节点不为空并且父节点并非根节点
	// 此节点的局部矩阵 = 当前节点的世界矩阵 * 父节点的逆矩阵 = 当前节点在父空间的矩阵
	if( pParent && !pParent->IsRootNode())
		RHelper::MAXtoD3D( pNode->GetNodeTM( 0 ) * Inverse( pNode->GetParentTM( 0 ) ), pMeshObject->m_matLocal );
	else	// 如果是相对于根节点而言,其模型节点的矩阵即为它的局部矩阵
		RHelper::MAXtoD3D( pNode->GetNodeTM( 0 ), pMeshObject->m_matLocal  );
	// 模型的世界矩阵
	RHelper::MAXtoD3D( pNode->GetNodeTM( 0 ), pMeshObject->m_matWorld  );
	
	pMeshObject->m_nBillboardType = RHelper::GetBillboardType( pMeshObject->m_szName );				// 获取广告牌类型
	pMeshObject->m_nAnimationType = RHelper::GetAnimationType( pMeshObject->m_szName );				// 获取动画类型
	
	vector< Point3 > vecPositionList;
	vector< Point2 > vecTextureList;
	vector< Point3 > vecNormalList;
	vector< color > clrColorList;
	vector< DWORD > vecFaceList;
	vector< DWORD > vecTFaceList;
	vector< WORD > vecMatList;
	vector< DWORD > vecCFaceList;

	int nVertex	= pMesh->getNumVerts();			// 顶点的数量
	int nFace = pMesh->getNumFaces();			// 图元的数量
	int nTexture = pMesh->getNumTVerts();		// 纹理的数量
	int nColor = pMesh->numCVerts;				// 像素点的数量

	Point3 p3Position, p3Normal, p3Texture, p3Color;
	Point3 vecPosition, vecNormal;
	Point2 vecTexture;	

	Matrix3 mtWorld = pNode->GetObjectTM( 0 );	// 节点的世界矩阵
	Matrix3 mtInvTM = Inverse( mtWorld );		// 世界的逆矩阵
	
	// 每个顶点的位置
	for( int cVertex = 0; cVertex < nVertex; cVertex++ )
	{
		// 这里的每个顶点都是相对于自身局部坐标系的坐标
		// 如果是骨骼动画,则将世界矩阵右乘模型的顶点向量换算到世界空间内
		if ( pMeshObject->m_nAnimationType & REAT_BONE )
			vecPosition = RHelper::MAXtoR3D( mtWorld * pMesh->verts[ cVertex ] );
		// 模型动画仅需要记录相对于世界空间的位置信息
		else
			vecPosition = RHelper::MAXtoR3D( pMesh->verts[ cVertex ] );

		// 压入位置队列
		vecPositionList.push_back( vecPosition );
	}

	// 纹理的UV坐标
	for( int cTexture=0; cTexture < nTexture; cTexture++ )
	{
		p3Texture = pMesh->tVerts[ cTexture ];
		vecTexture.x = p3Texture.x;
		vecTexture.y = 1.0f - p3Texture.y;				// 为什么要1减去它,从这里知道这个值的范围必定在0-1之间
		vecTextureList.push_back( vecTexture );
	}


	// 遍历像素
	for( int cColor=0; cColor < nColor; cColor++ )
	{
		p3Color = pMesh->vertCol[ cColor ];						// 获取一个像素
		color clr;
		clr.a = 0x0;											// 没有透明效果
		clr.r = ( unsigned char )( p3Color.x * 255.0f );
		clr.g = ( unsigned char )( p3Color.y * 255.0f );
		clr.b = ( unsigned char )( p3Color.z * 255.0f );

		clrColorList.push_back( clr );
	}

	// 图元遍历
	for( int cFace = 0; cFace < nFace; cFace++ )					
	{
		// 压入每个面的三个顶点的索引,再加上当前面的材质索引号
		vecFaceList.push_back( pMesh->faces[ cFace ].v[ v1 ] );
		vecFaceList.push_back( pMesh->faces[ cFace ].v[ v2 ] );
		vecFaceList.push_back( pMesh->faces[ cFace ].v[ v3 ] );
		vecMatList.push_back( pMesh->faces[ cFace ].getMatID() );				

		// 三个顶点的纹理坐标
		vecTFaceList.push_back( pMesh->tvFace[ cFace ].t[ v1 ] );
		vecTFaceList.push_back( pMesh->tvFace[ cFace ].t[ v2 ] );
		vecTFaceList.push_back( pMesh->tvFace[ cFace ].t[ v3 ] );		

		// 压入顶点色
		if( nColor > 0 )
		{
			vecCFaceList.push_back( pMesh->vcFace[ cFace ].t[ v1 ] );
			vecCFaceList.push_back( pMesh->vcFace[ cFace ].t[ v2 ] );
			vecCFaceList.push_back( pMesh->vcFace[ cFace ].t[ v3 ] );
		}
	}

	// 如果是骨骼动画则记录权重
	vector< RWeight > vecWeightList;
	int iUsedWeightCount = 0;
	// 如果是骨骼动画
	if ( pMeshObject->m_nAnimationType & REAT_BONE )
		iUsedWeightCount = SetVertexWeightList( vecWeightList, pNode );			// 所有顶点受骨骼的数量

	Point3 vecSavePosition[ 3 ], vecSaveNormal[ 3 ], vecSaveTangentU[ 3 ], vecSaveTangentV[ 3 ];
	Point2 vecSaveTexture[ 3 ];

	color clrSaveColor[ 3 ];

	RWeight	weight[ 3 ];

	mtWorld.NoTrans();

	// 遍历这个节点的所有图元
	for( int cFace=0; cFace < nFace ; cFace++ )
	{	
		// 获取图元的3个顶点位置
		vecSavePosition[ 0 ] = vecPositionList[ vecFaceList[ cFace * 3 + 0 ] ];
		vecSavePosition[ 1 ] = vecPositionList[ vecFaceList[ cFace * 3 + 1 ] ];
		vecSavePosition[ 2 ] = vecPositionList[ vecFaceList[ cFace * 3 + 2 ] ];

		// 获取图元的3个顶点的纹理坐标
		vecSaveTexture[ 0 ] = vecTextureList[ vecTFaceList[ cFace * 3 + 0 ] ];
		vecSaveTexture[ 1 ] = vecTextureList[ vecTFaceList[ cFace * 3 + 1 ] ];
		vecSaveTexture[ 2 ] = vecTextureList[ vecTFaceList[ cFace * 3 + 2 ] ];

		// 获取图元的3个顶点的权重
		if ( !vecWeightList.empty() )
		{
			weight[ 0 ] = vecWeightList[ vecFaceList[ cFace * 3 + 0 ] ];
			weight[ 1 ] = vecWeightList[ vecFaceList[ cFace * 3 + 1 ] ];
			weight[ 2 ] = vecWeightList[ vecFaceList[ cFace * 3 + 2 ] ];
		}

		Face* f;
		f = &pMesh->faces[ cFace ];	

		// 获取图元的3个顶点的法线
		vecSaveNormal[ 0 ] = RHelper::GetVertexNormal( pMesh, cFace, pMesh->getRVertPtr( f->getVert( v1 ) ) );
		vecSaveNormal[ 1 ] = RHelper::GetVertexNormal( pMesh, cFace, pMesh->getRVertPtr( f->getVert( v2 ) ) );
		vecSaveNormal[ 2 ] = RHelper::GetVertexNormal( pMesh, cFace, pMesh->getRVertPtr( f->getVert( v3 ) ) );

		Point3 vNormal[ 3 ];

		// 保存三条法线
		vNormal[ 0 ].x = vecSaveNormal[ 0 ].x;
		vNormal[ 0 ].y = vecSaveNormal[ 0 ].y;
		vNormal[ 0 ].z = vecSaveNormal[ 0 ].z;

		vNormal[ 1 ].x = vecSaveNormal[ 1 ].x;
		vNormal[ 1 ].y = vecSaveNormal[ 1 ].y;
		vNormal[ 1 ].z = vecSaveNormal[ 1 ].z;

		vNormal[ 2 ].x = vecSaveNormal[ 2 ].x;
		vNormal[ 2 ].y = vecSaveNormal[ 2 ].y;
		vNormal[ 2 ].z = vecSaveNormal[ 2 ].z;

		//Point3 vTransNormal[ 3 ];

		vecSaveNormal[ 0 ] = RHelper::MAXtoR3D( vNormal[ 0 ] );
		vecSaveNormal[ 1 ] = RHelper::MAXtoR3D( vNormal[ 1 ] );
		vecSaveNormal[ 2 ] = RHelper::MAXtoR3D( vNormal[ 2 ] );

		//vecSaveNormal[ 0 ].x = vTransNormal[ 0 ].x;
		//vecSaveNormal[ 0 ].y = vTransNormal[ 0 ].z;
		//vecSaveNormal[ 0 ].z = vTransNormal[ 0 ].y;

		//vecSaveNormal[ 1 ].x = vTransNormal[ 1 ].x;
		//vecSaveNormal[ 1 ].y = vTransNormal[ 1 ].z;
		//vecSaveNormal[ 1 ].z = vTransNormal[ 1 ].y;

		//vecSaveNormal[ 2 ].x = vTransNormal[ 2 ].x;
		//vecSaveNormal[ 2 ].y = vTransNormal[ 2 ].z;
		//vecSaveNormal[ 2 ].z = vTransNormal[ 2 ].y;

		//vecSaveNormal[ 0 ].Normalize();
		//vecSaveNormal[ 1 ].Normalize();
		//vecSaveNormal[ 2 ].Normalize();

		// 获取3个顶点的顶点色
		if( nColor > 0 )
		{
			clrSaveColor[ 0 ] = clrColorList[ vecCFaceList[ cFace * 3 + 0 ] ];
			clrSaveColor[ 1 ] = clrColorList[ vecCFaceList[ cFace * 3 + 1 ] ];
			clrSaveColor[ 2 ] = clrColorList[ vecCFaceList[ cFace * 3 + 2 ] ];
		}
		else
		{
			clrSaveColor[ 0 ].c = 0xFFFFFFFF;
			clrSaveColor[ 1 ].c = 0xFFFFFFFF;
			clrSaveColor[ 2 ].c = 0xFFFFFFFF;
		}

		if ( !vecWeightList.empty() )
			MakeSaveVertex( vecSavePosition, vecSaveNormal, vecSaveTexture, clrSaveColor, weight, pMtl, vecMatList[ cFace ] );
		else
			MakeSaveVertex( vecSavePosition, vecSaveNormal, vecSaveTexture, clrSaveColor, NULL, pMtl, vecMatList[ cFace ] );
	}
	MakeR3DMesh( pMeshObject );

	m_lstMesh.push_back( pMeshObject );
	ClearSaveBuffers();
}

// 获取模型的ID
int Red3DMaxEffectUtil2008::GetMeshID( INode *pNode )
{
	for( UINT cMesh = 0; cMesh < m_lstMesh.size(); cMesh++ )
	{
		if( m_lstMesh[ cMesh ]->m_pNode == pNode )
			return cMesh;
	}
	return -1;
}

/*!	\fn MakeR3DMesh
 *	\param pMesh 3DsMAX模型指针
 *	\brief 产生R3D引擎所需的模型
 */
void Red3DMaxEffectUtil2008::MakeR3DMesh( RMesh *pMesh )
{
	int nVertexOffset = 0;
	int nIndexOffset = 0;
	int nTotalVertex = 0;
	int nTotalIndex = 0;	

	// 遍历图元
	// 一个模型最多有50个图元
	for( int cPrimitive=0; cPrimitive < MAX_NODE; cPrimitive++ )
	{
		// 如果当前图元的保存位置为空则继续
		if( m_lstSavePosition[ cPrimitive ].empty() )
			continue;

		pMesh->m_lstVertexCount.push_back( (int)m_lstSavePosition[ cPrimitive ].size() );			// 顶点的数量
		pMesh->m_lstIndexCount.push_back( (int)m_lstSaveIndex[ cPrimitive ].size() );				// 索引的数量

		pMesh->m_lstVertexOffset.push_back( nTotalVertex );											// 记录此图元在顶点缓冲中的偏移
		pMesh->m_lstIndexOffset.push_back( nTotalIndex );											// 记录此图元在顶点索引缓冲中的偏移

		pMesh->m_lstTextureID.push_back( FindVector( m_lstFinalTextureName, m_lstSaveTextureName[ cPrimitive ] ) );

		// 增加顶点与其索引总个数
		nTotalVertex += (int)m_lstSavePosition[ cPrimitive ].size();
		nTotalIndex += (int)m_lstSaveIndex[ cPrimitive ].size();

		Point3 *pNormal = new Point3[ m_lstSavePosition[ cPrimitive ].size() ];						// 当前图元的面法线

		// 遍历每个顶点
		for( UINT cVertex=0; cVertex < m_lstSavePosition[ cPrimitive ].size(); cVertex++ )
		{
			pMesh->m_lstPosition.push_back( m_lstSavePosition[ cPrimitive ][ cVertex ] );				// 压入顶点的位置
			pMesh->m_lstTextureCoord.push_back( m_lstSaveTexture[ cPrimitive ][ cVertex ] );			// 压入顶点的UV坐标
			pMesh->m_lstColor.push_back( m_lstSaveColor[ cPrimitive ][ cVertex ] );						// 压入顶点的颜色
			pNormal[ cVertex ] = m_lstSaveNormal[ cPrimitive ][ cVertex ];								// 压入顶点的法线方向
			// 如果顶点权重表不为空,则压入顶点的权重
			if ( !m_lstSaveWeight[ cPrimitive ].empty() )
				pMesh->m_vecWeightList.push_back( m_lstSaveWeight[ cPrimitive ][ cVertex ] );
		}

		// 保存这个图元的顶点索引
		for( UINT cIndex=0; cIndex < m_lstSaveIndex[ cPrimitive ].size(); cIndex++ )
		{
			pMesh->m_lstIndex.push_back( m_lstSaveIndex[ cPrimitive ][ cIndex ] );
		}
		
		int nVertexIndex[ 3 ];
		Point3 vecPosition[ 3 ];
		Point2 vecTextureCoordinate[ 3 ];
		Point3 vecS[ 3 ], vecT[ 3 ];

		// 图元中的顶点都对应一对切线向量UV
		Point3 *pTangentU = new Point3[ m_lstSavePosition[ cPrimitive ].size() ];
		Point3 *pTangentV = new Point3[ m_lstSavePosition[ cPrimitive ].size() ];		
		memset( pTangentU, 0, sizeof( Point3 ) * m_lstSavePosition[ cPrimitive ].size() );
		memset( pTangentV, 0, sizeof( Point3 ) * m_lstSavePosition[ cPrimitive ].size() );

		// 遍历所有图元
		for( int cPolygon=0; cPolygon < (int)m_lstSaveIndex[ cPrimitive ].size() / 3; cPolygon++ )
		{
			// 获取图元的三个顶点的索引
			nVertexIndex[ 0 ] = pMesh->m_lstIndex[ cPolygon * 3 + 0 ];
			nVertexIndex[ 1 ] = pMesh->m_lstIndex[ cPolygon * 3 + 1 ];
			nVertexIndex[ 2 ] = pMesh->m_lstIndex[ cPolygon * 3 + 2 ];

			// 三个顶点的位置
			vecPosition[ 0 ] = pMesh->m_lstPosition[ nVertexIndex[ 0 ] ];
			vecPosition[ 1 ] = pMesh->m_lstPosition[ nVertexIndex[ 1 ] ];
			vecPosition[ 2 ] = pMesh->m_lstPosition[ nVertexIndex[ 2 ] ];

			// 三个顶点的纹理坐标
			vecTextureCoordinate[ 0 ] = pMesh->m_lstTextureCoord[ nVertexIndex[ 0 ] ];
			vecTextureCoordinate[ 1 ] = pMesh->m_lstTextureCoord[ nVertexIndex[ 1 ] ];
			vecTextureCoordinate[ 2 ] = pMesh->m_lstTextureCoord[ nVertexIndex[ 2 ] ];

			// 初始化UV切线向量
			vecS[ 0 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecS[ 1 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecS[ 2 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecT[ 0 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecT[ 1 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecT[ 2 ] = Point3( 0.0f, 0.0f, 0.0f );

			// 计算UV向量
			ComputeTangentVector( vecPosition[ 0 ], vecPosition[ 1 ], vecPosition[ 2 ], 
								 vecTextureCoordinate[ 0 ], vecTextureCoordinate[ 1 ], vecTextureCoordinate[ 2 ],
								 vecT, vecS );

			// 输出3个顶点的切线向量UV
			pTangentU[ nVertexIndex[ 0 ] ] += vecS[ 0 ];
			pTangentU[ nVertexIndex[ 1 ] ] += vecS[ 1 ];
			pTangentU[ nVertexIndex[ 2 ] ] += vecS[ 2 ];

			pTangentV[ nVertexIndex[ 0 ] ] += vecT[ 0 ];
			pTangentV[ nVertexIndex[ 1 ] ] += vecT[ 1 ];
			pTangentV[ nVertexIndex[ 2 ] ] += vecT[ 2 ];			
		}

		color clrNormal, clrTangentU, clrTangentV;

		// 统一每个顶点的切线向量与法向量到统一的度量衡
		for( UINT cVertex=0; cVertex < m_lstSavePosition[ cPrimitive ].size(); cVertex++ )
		{			
			pNormal[ cVertex ].Normalize();
			pTangentU[ cVertex ].Normalize();
			pTangentV[ cVertex ].Normalize();

			clrNormal.b = ( unsigned char ) ( ( pNormal[ cVertex ].x + 1.0f ) * 0.5f * 255.0f );
			clrNormal.g = ( unsigned char ) ( ( pNormal[ cVertex ].y + 1.0f ) * 0.5f * 255.0f );
			clrNormal.r = ( unsigned char ) ( ( pNormal[ cVertex ].z + 1.0f ) * 0.5f * 255.0f );

			clrTangentU.b = ( unsigned char ) ( ( pTangentU[ cVertex ].x + 1.0f ) * 0.5f * 255.0f );
			clrTangentU.g = ( unsigned char ) ( ( pTangentU[ cVertex ].y + 1.0f ) * 0.5f * 255.0f );
			clrTangentU.r = ( unsigned char ) ( ( pTangentU[ cVertex ].z + 1.0f ) * 0.5f * 255.0f );

			clrTangentV.b = ( unsigned char ) ( ( pTangentV[ cVertex ].x + 1.0f ) * 0.5f * 255.0f );
			clrTangentV.g = ( unsigned char ) ( ( pTangentV[ cVertex ].y + 1.0f ) * 0.5f * 255.0f );
			clrTangentV.r = ( unsigned char ) ( ( pTangentV[ cVertex ].z + 1.0f ) * 0.5f * 255.0f );			

			pMesh->m_lstNormal.push_back( clrNormal );
			pMesh->m_lstTangentU.push_back( clrTangentU );
			pMesh->m_lstTangentV.push_back( clrTangentV );
		}
		
		delete [] pTangentU;
		delete [] pTangentV;
		delete [] pNormal;
	}	

	pMesh->m_nVertexCount = nTotalVertex;				// 顶点的数量
	pMesh->m_nIndexCount = nTotalIndex;					// 索引的数量
}

/*!	\fn MakeSaveVertex	
 *	\param pPosition 图元三个顶点的位置列表
 *	\param pNormal 图元的三个顶点的法线列表
 *	\param pTexture 图元的三个顶点的纹理列表
 *	\param pColor 图元三个顶点的顶点色列表
 *	\param pWeight 图元三个顶点受权重的指针
 *	\param pMtl 图元的材质指针
 *	\param nSubMat 图元的材质ID
 *	\brief 保存到顶点保存缓存
 */
bool Red3DMaxEffectUtil2008::MakeSaveVertex( Point3 *pPosition, Point3 *pNormal, Point2 *pTexture, color *pColor, RWeight * pWeight, Mtl *pMtl, int nSubMat )
{
	// 找到材质索引
	int nMat = FindVector( m_lstMAXMatrial, pMtl );
	if( nMat == -1 )
		return false;

	int nTextureIndex = -1;

	// 遍历纹理列表找到与之相对的材质并记录纹理索引

	// 第一次判断主纹理ID与子纹理ID相同的情况
	for( UINT cTexture=0; cTexture < m_lstTexture.size(); cTexture++ )
	{
		if( m_lstTexture[ cTexture ].m_nMatID == nMat &&
			m_lstTexture[ cTexture ].m_nMatSubID == nSubMat )
		{
			nTextureIndex = cTexture;
			break;
		}
	}

	// 如果没有找到则进入第二次遍历,这次仅寻找纹理主ID的匹配情况
	if( nTextureIndex == -1 )
	{
		for( UINT cTexture=0; cTexture < m_lstTexture.size(); cTexture++ )
		{
			if( m_lstTexture[ cTexture ].m_nMatID == nMat )				
			{
				nTextureIndex = cTexture;
				break;
			}
		}
		if( nTextureIndex == -1 )
			return false;
	}

	// 通过nTextureIndex从m_lstTexture中获取纹理的文件名
	// 

    int AddVertexIndex = -1;			// 图元索引
	AddVertexIndex = FindVector( m_lstSaveTextureName, m_lstTexture[ nTextureIndex ].m_strTextureName );
	if( AddVertexIndex == -1 )
	{
		// 如果没有找到则增加纹理图名称
		m_lstSaveTextureName.push_back( m_lstTexture[ nTextureIndex ].m_strTextureName );
		AddVertexIndex = (int)m_lstSaveTextureName.size() - 1;
	}

	// 遍历图元的三个顶点
	for( int cVertex=0; cVertex < 3; cVertex++ )
	{
		bool bAlready = false;
		/*
		UINT cCheck;
		for( cCheck=0; cCheck < m_lstSavePosition[ AddVertexIndex ].size(); cCheck++ )
		{
			if( m_lstSavePosition[ AddVertexIndex ][ cCheck ] == pPosition[ cVertex ] &&
				m_lstSaveNormal[ AddVertexIndex ][ cCheck ] == pNormal[ cVertex ] &&
				m_lstSaveTexture[ AddVertexIndex ][ cCheck ] == pTexture[ cVertex ] )
			{
				bAlready = true;
				break;
			}
		}
 
		if( bAlready )
		{
			m_lstSaveIndex[ AddVertexIndex ].push_back( cCheck );
		}
		else*/
		{
			// AddVertexIndex这个数值是图元在模型中的索引
			m_lstSavePosition[ AddVertexIndex ].push_back( pPosition[ cVertex ] );			// 一个顶点一个位置队列
			m_lstSaveNormal[ AddVertexIndex ].push_back( pNormal[ cVertex ] );				// 一个顶点一个法线
			m_lstSaveTexture[ AddVertexIndex ].push_back( pTexture[ cVertex ] );			// 一个顶点一个纹理UV坐标
			m_lstSaveColor[ AddVertexIndex ].push_back( pColor[ cVertex ] );				// 一个顶点一个像素值
			m_lstSaveIndex[ AddVertexIndex ].push_back( (WORD)m_lstSavePosition[ AddVertexIndex ].size() - 1 );	// 顶点的索引
			if ( pWeight )																	// 顶点的权值
				m_lstSaveWeight[ AddVertexIndex ].push_back( pWeight[ cVertex ] );
		}
	}	
	return true;
}

/*!	\fn ClearSaveBuffers
 *	\brief 清除保存缓存
 */
void Red3DMaxEffectUtil2008::ClearSaveBuffers(void)
{
	// 一个模型最多50个图元
	for( int cNode = 0; cNode < MAX_NODE; cNode++ )
	{
		// 如果此图元的位置节点为空则继续
		if( m_lstSavePosition[ cNode ].empty() )
			continue;
		m_lstSavePosition[ cNode ].resize( 0 );
		m_lstSaveNormal [ cNode ].resize( 0 );
		m_lstSaveTexture[ cNode ].resize( 0 );
		m_lstSaveColor[ cNode ].resize( 0 );
		m_lstSaveIndex[ cNode ].resize( 0 );
		m_lstSaveWeight[ cNode ].resize( 0 );
	}
}

/*!	\fn ExportAnimationElement
 *	\brief 输出动画元素
 *	\details 遍历所有网格模型,并且遍历每帧,把每个模型在每帧下的 "旋转" "平移" "缩放" 三种状态保存到当前模型的
 *			 某个变量
 */
void Red3DMaxEffectUtil2008::ExportAnimationElement(void)
{
	RMesh *pMesh;

	// 遍历网格模型
	for( UINT cMesh = 0; cMesh < m_lstMesh.size(); cMesh++ )
	{
		// 获取一个模型
		pMesh = m_lstMesh[ cMesh ];
		TimeValue	start = ip->GetAnimRange().Start();
		TimeValue	end = ip->GetAnimRange().End();
		int			delta = GetTicksPerFrame();
		TimeValue	t;
		Matrix3		tm;
		AffineParts ap;
		Point3		pos, prevPos;
		int nFrame = 1;
		
		Quat qPrev;
		qPrev.Identity();

		Point3		scale;

		// 遍历每帧
		for( t = start; t < end + delta; t += delta )
		{
			//Matrix3 mtTM = pMesh->m_pNode->GetNodeTM( t ) * Inverse( pMesh->m_pNode->GetParentTM( t ) );
			Matrix3 mtTM = pMesh->m_pNode->GetNodeTM( t );		// 获取节点的世界矩阵
			
			RHelper::MAXtoD3D( mtTM );
			decomp_affine( mtTM, &ap );

			// 得到位置
			pos = ap.t;
			Point3 vecPosKey;

			vecPosKey.x = pos.x;
			vecPosKey.y = pos.y;
			vecPosKey.z = pos.z;

			pMesh->m_lstAnimationPositionKey.push_back( vecPosKey );			// 位置信息

			//Quat q = ap.q / qPrev;
			Quat q = ap.q;
			//qPrev = q;
			
			Point4 vecRotateKey;
			vecRotateKey.x = -q.x;
			vecRotateKey.y = -q.y;
			vecRotateKey.z = -q.z;
			vecRotateKey.w = q.w;
			pMesh->m_lstAnimationRotateKey.push_back( vecRotateKey );			// 旋转信息

			scale = ap.k;
			Point3 vecScaleKey;
			vecScaleKey.x = scale.x;
			vecScaleKey.y = scale.y;
			vecScaleKey.z = scale.z;			

			pMesh->m_lstAnimationScaleKey.push_back( vecScaleKey );				// 比例信息

			float fVisibility = pMesh->m_pNode->GetVisibility( t );		
			pMesh->m_lstAnimationVisibilityKey.push_back( fVisibility );		// 可见范围信息

			nFrame++;			
		}
		//D3DXQUATERNION qtRotation;

		/*for( int cRotation=1; cRotation < pMesh->m_lstAnimationRotateKey.GetCount(); cRotation++ )
		{			
			memcpy( &qtRotation, &pMesh->m_lstAnimationRotateKey[ cRotation ], sizeof( D3DXQUATERNION ) );			 
			D3DXQuaternionMultiply( (D3DXQUATERNION*)&pMesh->m_lstAnimationRotateKey[ cRotation ], (D3DXQUATERNION*)&pMesh->m_lstAnimationRotateKey[ cRotation - 1], &qtRotation );			
		}*/
	}
}

/*

void Red3DMaxEffectUtil2008::ExportAnimationRotate(void)
{
	return;
	RMesh *pMesh;
	for( int cMesh = 0; cMesh < m_lstMesh.GetCount(); cMesh++ )
	{
		pMesh = m_lstMesh[ cMesh ];
		TimeValue	start = ip->GetAnimRange().Start();
		TimeValue	end = ip->GetAnimRange().End();
		int			delta = GetTicksPerFrame();
		TimeValue	t;
		Matrix3		tm;
		AffineParts ap;
		Quat		prevQ;
		prevQ.Identity();
		for( t = start; t < end + delta; t += delta )
		{
			tm = pMesh->m_pNode->GetNodeTM( t );
			RHelper::MAXtoD3D( tm );

			matrix mat;
			mat.MakeIdent();
			 
			mat._11 = tm.GetRow( 0 ).x;
			mat._12 = tm.GetRow( 0 ).y;
			mat._13 = tm.GetRow( 0 ).z;

			mat._21 = tm.GetRow( 1 ).x;
			mat._22 = tm.GetRow( 1 ).y;
			mat._23 = tm.GetRow( 1 ).z;

			mat._31 = tm.GetRow( 2 ).x;
			mat._32 = tm.GetRow( 2 ).y;
			mat._33 = tm.GetRow( 2 ).z;

			mat._41 = tm.GetRow( 3 ).x;
			mat._42 = tm.GetRow( 3 ).y;
			mat._43 = tm.GetRow( 3 ).z;

			vector3 vecScale;
			D3DXQUATERNION quat;
			vector3 vecTrans;
			D3DXMatrixDecompose( vecScale, &quat, vecTrans, mat );
			
			Quat qTemp;
			qTemp.x = quat.x;
			qTemp.y = quat.y;
			qTemp.z = quat.z;
			qTemp.w = quat.w;

			Quat q = qTemp / prevQ;
	
			prevQ = qTemp;
			vector4 vecRotateKey;
			vecRotateKey.x = -q.x;
			vecRotateKey.y = -q.y;
			vecRotateKey.z = -q.z;
			vecRotateKey.w = q.w;
			pMesh->m_lstAnimationRotateKey.Add( vecRotateKey );
		}

		D3DXQUATERNION qtRotation;

		for( int cRotation=1; cRotation < pMesh->m_lstAnimationRotateKey.GetCount(); cRotation++ )
		{			
			memcpy( &qtRotation, &pMesh->m_lstAnimationRotateKey[ cRotation ], sizeof( D3DXQUATERNION ) );			 
			D3DXQuaternionMultiply( (D3DXQUATERNION*)&pMesh->m_lstAnimationRotateKey[ cRotation ], (D3DXQUATERNION*)&pMesh->m_lstAnimationRotateKey[ cRotation - 1], &qtRotation );			
		}
	}
}

void Red3DMaxEffectUtil2008::ExportAnimationScale(void)
{
	RMesh *pMesh;
	for( int cMesh = 0; cMesh < m_lstMesh.GetCount(); cMesh++ )
	{
		pMesh = m_lstMesh[ cMesh ];
		TimeValue	start = ip->GetAnimRange().Start();
		TimeValue	end = ip->GetAnimRange().End();
		int			delta = GetTicksPerFrame();
		TimeValue	t;
		Matrix3		tm;
		AffineParts ap;
		Point3		scale, prevScale;
		int nFrame = 1;
		for( t = start; t < end + delta; t += delta )
		{
            tm = pMesh->m_pNode->GetNodeTM( t );
			RHelper::MAXtoD3D( tm );
			decomp_affine( tm, &ap );
			scale = ap.k;
			
			vector3 vecScaleKey;
			vecScaleKey.x = scale.x;
			vecScaleKey.y = scale.y;
			vecScaleKey.z = scale.z;			
			pMesh->m_lstAnimationScaleKey.Add( vecScaleKey );
			prevScale = scale;
			nFrame++;
		}
	}
}

void Red3DMaxEffectUtil2008::ExportAnimationVisibility(void)
{
	RMesh *pMesh;
	for( int cMesh = 0; cMesh < m_lstMesh.GetCount(); cMesh++ )
	{
		pMesh = m_lstMesh[ cMesh ];
		TimeValue	start = ip->GetAnimRange().Start();
		TimeValue	end = ip->GetAnimRange().End();
		int			delta = GetTicksPerFrame();
		TimeValue	t;
		Matrix3		tm;
		AffineParts ap;
		float		fVisibility;
		int nFrame = 1;
		for( t = start; t < end + delta; t += delta )
		{			
			nFrame++;
		}
	}
}
*/

/*!	\fn ExportAnimationMesh
 *	\param pNode 对象节点
 *	\param t 动画时间节点
 *	\param iAniType 动画类型
 *	\brief 输出动画模型
 */
void Red3DMaxEffectUtil2008::ExportAnimationMesh( INode *pNode, TimeValue t, int iAniType )
{
	Mtl *pMtl = pNode->GetMtl();
	if( pMtl == NULL )
		return;	

	// 节点的朝向
	int v1, v2, v3;	
	bool bNegScale = RHelper::TMNegParity( pNode->GetNodeTM( t ) );

	if( bNegScale )
	{
		v1 = 0; v2 = 1; v3 = 2;
	}
	else
	{
		v1 = 2; v2 = 1; v3 = 0;
	}

	// 从节点中当前时间获取三角图元对象
	bool bNeedDel;
	TriObject *pTri = RHelper::GetTriObjectFromNode( pNode, t, bNeedDel );
	if( pTri == NULL )	
		return;

	// 从三角图元对象中获取网格模型,建立法线
	Mesh *pMesh = &pTri->GetMesh();
	pMesh->buildNormals();

	// 获取当前节点的父节点与场景根节点
	INode *pParent = pNode->GetParentNode();
	INode *pRoot = ip->GetRootNode();

	// 获取当前节点的模型ID
	int nMesh = GetMeshID( pNode );
	if( nMesh == -1 ) 
		return;

	// 分配一个网格动画内存
	RMeshAnimation *pMeshObject = new RMeshAnimation;				
	
	vector< Point3 > vecPositionList;			// 位置队列,记录了每个顶点的位置
	vector< Point2 > vecTextureList;			// 纹理坐标
	vector< Point3 > vecNormalList;				// 法线列表
	vector< color > clrColorList;				// 顶点颜色列表
	vector< DWORD > vecFaceList;				// 图元列表
	vector< DWORD > vecTFaceList;				//
	vector< WORD > vecMatList;					// 材质列表,按照三角图元为单位
	vector< DWORD > vecCFaceList;				// 

	int nVertex	= pMesh->getNumVerts();			// 获取模型顶点数量
	int nFace = pMesh->getNumFaces();			// 获取当前模型面数
	int nTexture = pMesh->getNumTVerts();		// 获取当前纹理计数
	int nColor = pMesh->numCVerts;				// 获取当前模型顶点颜色数量

	Point3 p3Position, p3Normal, p3Texture, p3Color;
	Point3 vecPosition, vecNormal;
	Point2 vecTexture;		

	Matrix3 mtWorld = pNode->GetObjectTM( 0 );	// 获取当前节点的世界矩阵	
	Matrix3 mtInvTM = Inverse( mtWorld );		// 获取当前节点的世界逆矩阵
	
	// 遍历一个对象所有顶点的在这帧时的位置
	for( int cVertex = 0; cVertex < nVertex; cVertex++ )
	{
		vecPosition = RHelper::MAXtoR3D( pMesh->verts[ cVertex ] );			// 将3dsmax矩阵转换为r3d的矩阵
		vecPositionList.push_back( vecPosition );							// 压入位置队列
	}

	// 遍历一个对象所有顶点的在这帧时的UV位置
	for( int cTexture=0; cTexture < nTexture; cTexture++ )
	{
		p3Texture = pMesh->tVerts[ cTexture ];								// 取出顶点的纹理结构
		// 将纹理限定在[0 1]之间
		vecTexture.x = p3Texture.x;
		vecTexture.y = 1.0f - p3Texture.y;
		vecTextureList.push_back( vecTexture );								// 压入一个顶点当前的纹理
	}

	// 遍历一个对象所有顶点的在这帧时颜色
	for( int cColor=0; cColor < nColor; cColor++ )
	{
		p3Color = pMesh->vertCol[ cColor ];									// 取出顶点的颜色
		color clr;
		clr.a = 0x0;
		clr.r = ( unsigned char )( p3Color.x * 255.0f );
		clr.g = ( unsigned char )( p3Color.y * 255.0f );
		clr.b = ( unsigned char )( p3Color.z * 255.0f );

		clrColorList.push_back( clr );										// 从0-1的范围扩展到0-255范围并压入列表
	}

	// 三角图元遍历
	for( int cFace = 0; cFace < nFace; cFace++ )					
	{
		// 压入三个顶点的索引
		vecFaceList.push_back( pMesh->faces[ cFace ].v[ v1 ] );
		vecFaceList.push_back( pMesh->faces[ cFace ].v[ v2 ] );
		vecFaceList.push_back( pMesh->faces[ cFace ].v[ v3 ] );

		// 压入当前图元的材质ID
		vecMatList.push_back( pMesh->faces[ cFace ].getMatID() );				

		vecTFaceList.push_back( pMesh->tvFace[ cFace ].t[ v1 ] );
		vecTFaceList.push_back( pMesh->tvFace[ cFace ].t[ v2 ] );
		vecTFaceList.push_back( pMesh->tvFace[ cFace ].t[ v3 ] );		

		// 如果颜色大于0
		if( nColor > 0 )
		{		
			vecCFaceList.push_back( pMesh->vcFace[ cFace ].t[ v1 ] );
			vecCFaceList.push_back( pMesh->vcFace[ cFace ].t[ v2 ] );
			vecCFaceList.push_back( pMesh->vcFace[ cFace ].t[ v3 ] );
		}

	}

	Point3 vecSavePosition[ 3 ], vecSaveNormal[ 3 ], vecSaveTangentU[ 3 ], vecSaveTangentV[ 3 ];
	Point2 vecSaveTexture[ 3 ];
	color clrSaveColor[ 3 ];

	// 清空平移向量
	mtWorld.NoTrans();

	// 遍历三角图元
	for( int cFace=0; cFace < nFace ; cFace++ )
	{
		// vecFaceList中保存的是顶点索引
		// vecSavePosition保存了顶点的三维坐标
		vecSavePosition[ 0 ] = vecPositionList[ vecFaceList[ cFace * 3 + 0 ] ];
		vecSavePosition[ 1 ] = vecPositionList[ vecFaceList[ cFace * 3 + 1 ] ];
		vecSavePosition[ 2 ] = vecPositionList[ vecFaceList[ cFace * 3 + 2 ] ];

		// vecSavePosition保存了顶点的纹理坐标
		vecSaveTexture[ 0 ] = vecTextureList[ vecTFaceList[ cFace * 3 + 0 ] ];
		vecSaveTexture[ 1 ] = vecTextureList[ vecTFaceList[ cFace * 3 + 1 ] ];
		vecSaveTexture[ 2 ] = vecTextureList[ vecTFaceList[ cFace * 3 + 2 ] ];

		// 取出当前图元
		Face* f;
		f = &pMesh->faces[ cFace ];

		// 获取这三个顶点的三个顶点法线
		vecSaveNormal[ 0 ] = RHelper::GetVertexNormal( pMesh, cFace, pMesh->getRVertPtr( f->getVert( v1 ) ) );
		vecSaveNormal[ 1 ] = RHelper::GetVertexNormal( pMesh, cFace, pMesh->getRVertPtr( f->getVert( v2 ) ) );
		vecSaveNormal[ 2 ] = RHelper::GetVertexNormal( pMesh, cFace, pMesh->getRVertPtr( f->getVert( v3 ) ) );

		// 转换格式到r3d向量并且重新保存到vecSaveNormal
		Point3 vNormal[ 3 ];

		vNormal[ 0 ].x = vecSaveNormal[ 0 ].x;
		vNormal[ 0 ].y = vecSaveNormal[ 0 ].y;
		vNormal[ 0 ].z = vecSaveNormal[ 0 ].z;

		vNormal[ 1 ].x = vecSaveNormal[ 1 ].x;
		vNormal[ 1 ].y = vecSaveNormal[ 1 ].y;
		vNormal[ 1 ].z = vecSaveNormal[ 1 ].z;

		vNormal[ 2 ].x = vecSaveNormal[ 2 ].x;
		vNormal[ 2 ].y = vecSaveNormal[ 2 ].y;
		vNormal[ 2 ].z = vecSaveNormal[ 2 ].z;

		//Point3 vTransNormal[ 3 ];

		vecSaveNormal[ 0 ] = RHelper::MAXtoR3D( vNormal[ 0 ] );
		vecSaveNormal[ 1 ] = RHelper::MAXtoR3D( vNormal[ 1 ] );
		vecSaveNormal[ 2 ] = RHelper::MAXtoR3D( vNormal[ 2 ] );

		//vecSaveNormal[ 0 ].x = vTransNormal[ 0 ].x;
		//vecSaveNormal[ 0 ].y = vTransNormal[ 0 ].z;
		//vecSaveNormal[ 0 ].z = vTransNormal[ 0 ].y;

		//vecSaveNormal[ 1 ].x = vTransNormal[ 1 ].x;
		//vecSaveNormal[ 1 ].y = vTransNormal[ 1 ].z;
		//vecSaveNormal[ 1 ].z = vTransNormal[ 1 ].y;

		//vecSaveNormal[ 2 ].x = vTransNormal[ 2 ].x;
		//vecSaveNormal[ 2 ].y = vTransNormal[ 2 ].z;
		//vecSaveNormal[ 2 ].z = vTransNormal[ 2 ].y;

		//vecSaveNormal[ 0 ].Normalize();
		//vecSaveNormal[ 1 ].Normalize();
		//vecSaveNormal[ 2 ].Normalize();

		// 如果顶点颜色大于0则记录这三个对应顶点的顶点颜色
		if( nColor > 0 )
		{		
			clrSaveColor[ 0 ] = clrColorList[ vecCFaceList[ cFace * 3 + 0 ] ];
			clrSaveColor[ 1 ] = clrColorList[ vecCFaceList[ cFace * 3 + 1 ] ];
			clrSaveColor[ 2 ] = clrColorList[ vecCFaceList[ cFace * 3 + 2 ] ];
		}
		else
		{
			clrSaveColor[ 0 ].c = 0xFFFFFFFF;
			clrSaveColor[ 1 ].c = 0xFFFFFFFF;
			clrSaveColor[ 2 ].c = 0xFFFFFFFF;
		}

		// vecSavePosition			图元三个顶点的位置列表
		// vecSaveNormal			图元的三个顶点的法线列表
		// vecSaveTexture			图元的三个顶点的纹理列表
		// clrSaveColor				图元三个顶点的顶点色列表
		// NULL						图元三个顶点受权重的指针
		// pMtl						图元的材质指针
		// vecMatList[ cFace ]		图元的材质ID
		// 产生保存顶点
		MakeSaveVertex( vecSavePosition, vecSaveNormal, vecSaveTexture, clrSaveColor, NULL, pMtl, vecMatList[ cFace ] );
	}

	// 产生r3d网格动画
	MakeR3DMeshAnimation( pMeshObject );

	// 压入当前帧模型动画结构
	m_lstMesh[ nMesh ]->m_lstMeshAnimation.push_back( pMeshObject );
	ClearSaveBuffers();
}

void Red3DMaxEffectUtil2008::MakeR3DMeshAnimation( RMeshAnimation *pMesh )
{
	int nVertexOffset = 0;
	int nIndexOffset = 0;
	int nTotalVertex = 0;
	int nTotalIndex = 0;

	for( int cPrimitive=0; cPrimitive < MAX_NODE; cPrimitive++ )
	{
		if( m_lstSavePosition[ cPrimitive ].empty() )
			continue;

		Point3 *pNormal = new Point3[ m_lstSavePosition[ cPrimitive ].size() ];

		for( UINT cVertex=0; cVertex < m_lstSavePosition[ cPrimitive ].size(); cVertex++ )
		{
			pMesh->m_lstPosition.push_back( m_lstSavePosition[ cPrimitive ][ cVertex ] );			
			pMesh->m_lstTextureCoord.push_back( m_lstSaveTexture[ cPrimitive ][ cVertex ] );
			pMesh->m_lstColor.push_back( m_lstSaveColor[ cPrimitive ][ cVertex ] );

			pNormal[ cVertex ] = m_lstSaveNormal[ cPrimitive ][ cVertex ];
		}		
		
		int nVertexIndex[ 3 ];
		Point3 vecPosition[ 3 ];
		Point2 vecTextureCoordinate[ 3 ];
		Point3 vecS[ 3 ], vecT[ 3 ];

		Point3 *pTangentU = new Point3[ m_lstSavePosition[ cPrimitive ].size() ];
		Point3 *pTangentV = new Point3[ m_lstSavePosition[ cPrimitive ].size() ];		
		memset( pTangentU, 0, sizeof( Point3 ) * m_lstSavePosition[ cPrimitive ].size() );
		memset( pTangentV, 0, sizeof( Point3 ) * m_lstSavePosition[ cPrimitive ].size() );

		for( int cPolygon=0; cPolygon < (int)m_lstSaveIndex[ cPrimitive ].size() / 3; cPolygon++ )
		{
			nVertexIndex[ 0 ] = m_lstSaveIndex[ cPrimitive ][ cPolygon * 3 + 0 ];
			nVertexIndex[ 1 ] = m_lstSaveIndex[ cPrimitive ][ cPolygon * 3 + 1 ];
			nVertexIndex[ 2 ] = m_lstSaveIndex[ cPrimitive ][ cPolygon * 3 + 2 ];

			vecPosition[ 0 ] = pMesh->m_lstPosition[ nVertexIndex[ 0 ] ];
			vecPosition[ 1 ] = pMesh->m_lstPosition[ nVertexIndex[ 1 ] ];
			vecPosition[ 2 ] = pMesh->m_lstPosition[ nVertexIndex[ 2 ] ];

			vecTextureCoordinate[ 0 ] = pMesh->m_lstTextureCoord[ nVertexIndex[ 0 ] ];
			vecTextureCoordinate[ 1 ] = pMesh->m_lstTextureCoord[ nVertexIndex[ 1 ] ];
			vecTextureCoordinate[ 2 ] = pMesh->m_lstTextureCoord[ nVertexIndex[ 2 ] ];

			vecS[ 0 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecS[ 1 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecS[ 2 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecT[ 0 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecT[ 1 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecT[ 2 ] = Point3( 0.0f, 0.0f, 0.0f );

			ComputeTangentVector( vecPosition[ 0 ], vecPosition[ 1 ], vecPosition[ 2 ], 
								 vecTextureCoordinate[ 0 ], vecTextureCoordinate[ 1 ], vecTextureCoordinate[ 2 ],
								 vecT, vecS );

			pTangentU[ nVertexIndex[ 0 ] ] += vecS[ 0 ];
			pTangentU[ nVertexIndex[ 1 ] ] += vecS[ 1 ];
			pTangentU[ nVertexIndex[ 2 ] ] += vecS[ 2 ];

			pTangentV[ nVertexIndex[ 0 ] ] += vecT[ 0 ];
			pTangentV[ nVertexIndex[ 1 ] ] += vecT[ 1 ];
			pTangentV[ nVertexIndex[ 2 ] ] += vecT[ 2 ];			
		}

		color clrNormal, clrTangentU, clrTangentV;

		for( UINT cVertex=0; cVertex < m_lstSavePosition[ cPrimitive ].size(); cVertex++ )
		{			
			pNormal[ cVertex ].Normalize();
			pTangentU[ cVertex ].Normalize();
			pTangentV[ cVertex ].Normalize();

			// 计算顶点法线
			clrNormal.b = ( unsigned char ) ( ( pNormal[ cVertex ].x + 1.0f ) * 0.5f * 255.0f );
			clrNormal.g = ( unsigned char ) ( ( pNormal[ cVertex ].y + 1.0f ) * 0.5f * 255.0f );
			clrNormal.r = ( unsigned char ) ( ( pNormal[ cVertex ].z + 1.0f ) * 0.5f * 255.0f );

			clrTangentU.b = ( unsigned char ) ( ( pTangentU[ cVertex ].x + 1.0f ) * 0.5f * 255.0f );
			clrTangentU.g = ( unsigned char ) ( ( pTangentU[ cVertex ].y + 1.0f ) * 0.5f * 255.0f );
			clrTangentU.r = ( unsigned char ) ( ( pTangentU[ cVertex ].z + 1.0f ) * 0.5f * 255.0f );

			clrTangentV.b = ( unsigned char ) ( ( pTangentV[ cVertex ].x + 1.0f ) * 0.5f * 255.0f );
			clrTangentV.g = ( unsigned char ) ( ( pTangentV[ cVertex ].y + 1.0f ) * 0.5f * 255.0f );
			clrTangentV.r = ( unsigned char ) ( ( pTangentV[ cVertex ].z + 1.0f ) * 0.5f * 255.0f );			

			pMesh->m_lstNormal.push_back( clrNormal );				// 压入顶点法线
			pMesh->m_lstTangentU.push_back( clrTangentU );			// 压入切线U
			pMesh->m_lstTangentV.push_back( clrTangentV );			// 压入切线V
		}

		nTotalVertex += (int)m_lstSavePosition[ cPrimitive ].size();
		nTotalIndex += (int)m_lstSaveIndex[ cPrimitive ].size();
		
		delete [] pTangentU;
		delete [] pTangentV;		
		delete [] pNormal;
	}	
}

// 保存动画模型
void Red3DMaxEffectUtil2008::MakeAnimationList(void)
{
	RMesh *pMesh;

	// 遍历模型列表
	for( UINT cMesh = 0; cMesh < m_lstMesh.size(); cMesh++ )
	{		
		pMesh = m_lstMesh[ cMesh ];

		// 如果是模型动画
		if ( pMesh->m_nAnimationType & REAT_MESH || pMesh->m_nAnimationType & REAT_TEXTURE )
		{
			TimeValue	start = ip->GetAnimRange().Start();			// 动画起始
			TimeValue	end = ip->GetAnimRange().End();				// 动画结束
			int			delta = GetTicksPerFrame();					// 一帧多少个Ticks
			TimeValue	t;
			// 遍历每帧
			for( t = start; t < end + delta; t += delta )
			{
				// 输出动画模型
				ExportAnimationMesh( pMesh->m_pNode, t, pMesh->m_nAnimationType );
			}
		}
	}

	// 输出动画元素
	ExportAnimationElement();
	//ExportAnimationRotate();
	//ExportAnimationScale();
	//ExportAnimationVisibility();
}

// 产生图元
bool Red3DMaxEffectUtil2008::MakeGeometry(void)
{
	// 首先将"e_"开头的NODE也就是属于特效粒子的节点都填充到lstExportList中
	vector< INode* > lstExportList;										// 输出INode列表
	FillNodeList( ip->GetRootNode(), lstExportList );					// 填充节点列表

	// 如果输出列表为空则返回
	if( lstExportList.empty() ) 
		return false;

	// 遍历节点列表输出骨骼节点
	for( UINT cNode = 0; cNode < lstExportList.size(); cNode++ )
	{
		ExportBone( lstExportList[ cNode ] );
	}

	// 遍历节点列表输出网格节点
	for( UINT cNode = 0; cNode < lstExportList.size(); cNode++ )
	{
		ExportMesh( lstExportList[ cNode ] );
	}

	return true;
}

/*! \fn FillNodeList
 *	\param pNode 图元节点
 *	\param lstNodeList 输出节点列表
 *	\brief 填充节点列表
 */
void Red3DMaxEffectUtil2008::FillNodeList( INode *pNode, vector< INode* > &lstNodeList )
{
	// 图元节点的名称必须以"e_"开头或者是骨骼节点
	if( strncmp( pNode->GetName(), "e_", 2 ) == 0 || IsBone( pNode ) )
		lstNodeList.push_back( pNode );
	for( int cChild = 0; cChild < pNode->NumberOfChildren(); cChild++ )
		FillNodeList( pNode->GetChildNode( cChild ), lstNodeList );
}

// 写入特效纹理信息到XML中
void Red3DMaxEffectUtil2008::r3dWriteEffectTextureToXML( FILE *fp, vector< string > &lstTexture ) {
	// 写入纹理信息
	int nTextureCount = (int)lstTexture.size();			// 获取纹理数量
	if (nTextureCount > 0) {
		fprintf(fp, "<Texture Size=\"%d\">\r\n", nTextureCount);
		for (int cTexture = 0; cTexture < nTextureCount; cTexture++) {
			fprintf(fp, "<TextureFileName>%s</TextureFilename>\r\n", lstTexture[ cTexture ].c_str());
		}
		fprintf(fp, "</Texture>\r\n");
	}
}

void Red3DMaxEffectUtil2008::r3dWriteFloatMatrixToXML( FILE *fp, float *fMatrix ) {
	fprintf(fp, "<R0>%f %f %f %f</R0>\r\n", fMatrix[0], fMatrix[1], fMatrix[2], fMatrix[3]);
	fprintf(fp, "<R1>%f %f %f %f</R1>\r\n", fMatrix[4], fMatrix[5], fMatrix[6], fMatrix[7]);
	fprintf(fp, "<R2>%f %f %f %f</R2>\r\n", fMatrix[8], fMatrix[9], fMatrix[10], fMatrix[11]);
	fprintf(fp, "<R3>%f %f %f %f</R3>\r\n", fMatrix[12], fMatrix[13], fMatrix[14], fMatrix[15]);
}

// 写入模型信息到XML中
bool Red3DMaxEffectUtil2008::r3dWriteEffectMeshToXML( FILE *fp, vector< Red3DMaxEffectUtil2008::RMesh* > &lstMesh ) {
	bool bBoneType = false;
	int nMeshCount = (int)lstMesh.size();												// 模型数量
	if (nMeshCount <= 0) {
		return false;
	}

	fprintf(fp, "<Mesh Size=\"%d\">\r\n", nMeshCount);
	for (int cMesh = 0; cMesh < nMeshCount; cMesh++) {
		RMesh *pMesh = lstMesh[ cMesh ];												// 获取网格模型
		fprintf(fp, "<MeshNode>");
		fprintf(fp, "<FileName>%s</FileName>", pMesh->m_szName);
		fprintf(fp, "<NodeID>%d</NodeID>", pMesh->m_nNode);
		fprintf(fp, "<ParentID>%d</ParentID>", pMesh->m_nParent);

		// 写入模型对象的世界矩阵，本地矩阵，以及世界逆矩阵
		float fSave[ 16 ];
		RHelper::Matrix3ToFloat16( pMesh->m_matWorld, fSave );
		fprintf(fp, "<WorldMatrix>\r\n");
		r3dWriteFloatMatrixToXML(fp, fSave);
		fprintf(fp, "</WorldMatrix>\r\n");

		RHelper::Matrix3ToFloat16( pMesh->m_matLocal, fSave );
		fprintf(fp, "<LocalMatrix>\r\n");
		r3dWriteFloatMatrixToXML(fp, fSave);
		fprintf(fp, "</LocalMatrix>\r\n");

		RHelper::Matrix3ToFloat16( pMesh->m_matInvWorld, fSave );
		fprintf(fp, "<InvWorldMatrix>\r\n");
		r3dWriteFloatMatrixToXML(fp, fSave);
		fprintf(fp, "</InvWorldMatrix>\r\n");

		// 写入顶点数量，索引数据，广告牌类型，动画类型
		fprintf(fp, "<VertexCount>%d</VertexCount>\r\n", pMesh->m_nVertexCount);
		fprintf(fp, "<IndexCount>%d</IndexCount>\r\n", pMesh->m_nIndexCount);
		fprintf(fp, "<BillboardType>%d</BillboardType>\r\n", pMesh->m_nBillboardType);
		fprintf(fp, "<AnimationType>%d</AnimationType>\r\n", pMesh->m_nAnimationType);

		// 确定动画的类型
		if( pMesh->m_nAnimationType & REAT_MESH || pMesh->m_nAnimationType & REAT_TEXTURE )
			pMesh->m_bMeshAnimation = true;
		else
			pMesh->m_bMeshAnimation = false;

		fprintf(fp, "<IsMeshAnimation>%d</IsMeshAnimation>\r\n", pMesh->m_bMeshAnimation);

		int nTrack = (int)pMesh->m_lstAnimationPositionKey.size();								// 关键帧帧的数量
		if( nTrack > 0 ) {
			fprintf(fp, "<AnimationPositionKey>%d</AnimationPositionKey>\r\n", nTrack);				// 写入关键帧数量
		}

		// 写入图元信息
		int nPrimitive = (int)pMesh->m_lstTextureID.size();										// 每个图元对应一个纹理
		if (nPrimitive > 0) {
			fprintf(fp, "<PrimitiveCount>%d</PrimitiveCount>\r\n", nPrimitive);

			// 写入纹理ID,一个图元对应一块纹理
			fprintf(fp, "<TextureIDList Size=\"%d\">\r\n", nPrimitive);
			for (int cTextureID = 0; cTextureID < nPrimitive; cTextureID++) {
				fprintf(fp, "<ID>%d</ID>\r\n", pMesh->m_lstTextureID[ cTextureID ]);
			}
			fprintf(fp, "</TextureIDList>\r\n");
		}

		// 写入位置坐标
		if (pMesh->m_lstPosition.size() > 0) {
			fprintf(fp, "<PositionList Size=\"%d\">\r\n", pMesh->m_lstPosition.size());
			for (int cPosition = 0; cPosition < pMesh->m_lstPosition.size(); cPosition++) {
				fprintf(fp, "<ID>%f %f %f</ID>\r\n", pMesh->m_lstPosition[ cPosition ].x, pMesh->m_lstPosition[ cPosition ].y, pMesh->m_lstPosition[ cPosition ].z);
			}
			fprintf(fp, "</PositionList>\r\n");
		}

		// 写入UV坐标
		if (pMesh->m_lstTextureCoord.size() > 0) {
			fprintf(fp, "<UVList Size=\"%d\">\r\n", pMesh->m_lstTextureCoord.size());
			for (int cUV = 0; cUV < pMesh->m_lstTextureCoord.size(); cUV++) {
				fprintf(fp, "<ID>%f %f %f</ID>\r\n", pMesh->m_lstTextureCoord[ cUV ].x, pMesh->m_lstPosition[ cUV ].y);
			}
			fprintf(fp, "</UVList>\r\n");
		}

		// 写入顶点颜色列表
		if (pMesh->m_lstColor.size() > 0) {
			fprintf(fp, "<ColorList Size=\"%d\">\r\n", pMesh->m_lstColor.size());
			for (int cColor = 0; cColor < pMesh->m_lstColor.size(); cColor++) {
				fprintf(fp, "<Color>%c %c %c %c</Color>\r\n", pMesh->m_lstColor[ cColor ].r, pMesh->m_lstColor[ cColor ].g, pMesh->m_lstColor[ cColor ].b, pMesh->m_lstColor[ cColor ].a);
			}
			fprintf(fp, "</ColorList>\r\n");
		}

		// 写入图元顶点的法线颜色列表
		if (pMesh->m_lstNormal.size() > 0) {
			fprintf(fp, "<NormalList Size=\"%d\">\r\n", pMesh->m_lstNormal.size());
			for (int cNormal = 0; cNormal < pMesh->m_lstNormal.size(); cNormal++) {
				fprintf(fp, "<Normal>%c %c %c %c</Normal>\r\n", pMesh->m_lstNormal[ cNormal ].r, pMesh->m_lstNormal[ cNormal ].g, pMesh->m_lstNormal[ cNormal ].b, pMesh->m_lstNormal[ cNormal ].a);
			}
			fprintf(fp, "</NormalList>\r\n");
		}

		// 写入切线U的坐标
		if (pMesh->m_lstTangentU.size() > 0) {
			fprintf(fp, "<TangentUList Size=\"%d\">\r\n", pMesh->m_lstTangentU.size());
			for (int cTangentU = 0; cTangentU < pMesh->m_lstTangentU.size(); cTangentU++) {
				fprintf(fp, "<TangentU>%c %c %c %c</TangentU>\r\n", pMesh->m_lstTangentU[ cTangentU ].r, pMesh->m_lstTangentU[ cTangentU ].g, pMesh->m_lstTangentU[ cTangentU ].b, pMesh->m_lstTangentU[ cTangentU ].a);
			}
			fprintf(fp, "</TangentUList>\r\n");
		}

		// 写入切线V的坐标
		if (pMesh->m_lstTangentV.size() > 0) {
			fprintf(fp, "<TangentVList Size=\"%d\">\r\n", pMesh->m_lstTangentV.size());
			for (int cTangentV = 0; cTangentV < pMesh->m_lstTangentV.size(); cTangentV++) {
				fprintf(fp, "<TangentV>%c %c %c %c</TangentV>\r\n", pMesh->m_lstTangentV[ cTangentV ].r, pMesh->m_lstTangentV[ cTangentV ].g, pMesh->m_lstTangentV[ cTangentV ].b, pMesh->m_lstTangentV[ cTangentV ].a);
			}
			fprintf(fp, "</TangentVList>\r\n");
		}

		// 写入图元索引
		if (pMesh->m_lstIndex.size() > 0) {
			fprintf(fp, "<IndexList Size=\"%d\">\r\n", pMesh->m_lstIndex.size());
			for (int cIndex = 0; cIndex < pMesh->m_lstIndex.size(); cIndex++) {
				fprintf(fp, "<Index>%d</Index>\r\n", pMesh->m_lstIndex[ cIndex ]);
			}
			fprintf(fp, "</IndexList>\r\n");
		}

		// 骨骼动画
		if ( pMesh->m_nAnimationType & REAT_BONE )
		{
			vector< DWORD > vecBoneIndex;
			vector< DWORD > vecBoneWeight;
			vector< int > vecBoneTable;

			bBoneType = true;

			// 获取权重列的数量
			int iWeightSize = (int) pMesh->m_vecWeightList.size();
			if (iWeightSize > 0) {
				// 遍历权重列表
				for ( int iWeight = 0; iWeight < iWeightSize; ++iWeight )
				{
					unsigned char nIndex[ 4 ];
					unsigned char nWeight[ 4 ];

					// 获取一个权重值
					RWeight & kWeight = pMesh->m_vecWeightList[ iWeight ];

					// 一个模型顶点受4个骨骼的影响
					int iBone = 0;
					for (; iBone < 4; ++iBone )
					{
						// 检验骨骼合法值,kWeight.vecBoneIndex.size()最多等于4
						if ( iBone < (int) kWeight.vecBoneIndex.size() )
						{
							bool bFind = false;
							int iTable = 0;
							// 遍历骨骼表
							for ( ; iTable < (int) vecBoneTable.size(); ++iTable )
							{
								if ( vecBoneTable[ iTable ] == kWeight.vecBoneIndex[ iBone ]  )
								{
									bFind = true;
									break;
								}
							}

							// 没有找到则加入到骨骼表中
							if ( bFind == false )
							{
								vecBoneTable.push_back( kWeight.vecBoneIndex[ iBone ] );
								iTable = (int)vecBoneTable.size() - 1;
							}

							// 增加骨骼索引与权重
							nIndex[ iBone ] = iTable;
							nWeight[ iBone ] = (unsigned char)(kWeight.vecWeight[ iBone ] * 255.0f);
						}
						else
						{
							nIndex[ iBone ] = 0;
							nWeight[ iBone ] = 0;
						}
					}

					// 压入队列
					vecBoneIndex.push_back( *((DWORD *) nIndex) );
					vecBoneWeight.push_back( *((DWORD *) nWeight) );
				}/* end for */
			}

			// 写入骨骼表
			int iBoneTableSize = (int) vecBoneTable.size();
			if ( iBoneTableSize > 0 ) {
				fprintf(fp, "<BoneAnimation>\r\n");
					// 骨骼表
					fprintf(fp, "<BoneTable Size=\"%d\">\r\n", iBoneTableSize);
					for (int cBoneTable = 0; cBoneTable < iBoneTableSize; cBoneTable++) {
						fprintf(fp, "<Table>%d</Table>\r\n", vecBoneTable[ cBoneTable ]);
					}
					fprintf(fp, "</BoneTable>\r\n");

					// 骨骼索引
					fprintf(fp, "<BoneIndex Size=\"%d\">\r\n", iWeightSize);
					for (int cBoneIndex = 0; cBoneIndex < iWeightSize; cBoneIndex++) {
						fprintf(fp, "<Index>%d</Index>\r\n", vecBoneIndex[ cBoneIndex ]);
					}
					fprintf(fp, "</BoneIndex>\r\n");

					// 骨骼权重
					fprintf(fp, "<BoneWeight Size=\"%d\">\r\n", iWeightSize);
					for (int cBoneWeight = 0; cBoneWeight < iWeightSize; cBoneWeight++) {
						fprintf(fp, "<Weight>%d</Weight>\r\n", vecBoneWeight[ cBoneWeight ]);
					}
					fprintf(fp, "</BoneWeight>\r\n");
				fprintf(fp, "</BoneAnimation>\r\n");
			}
		}
		// 关键帧动画
		else if( pMesh->m_nAnimationType & REAT_MESH || pMesh->m_nAnimationType & REAT_TEXTURE ) {
			int nAnimationMeshTrack = (int)pMesh->m_lstMeshAnimation.size();
			if (nAnimationMeshTrack > 0) {
				fprintf(fp, "<MeshOrTextureAnimation Size=\"%d\">\r\n", nAnimationMeshTrack);

				// 取出每帧
				RMeshAnimation* pMeshAnimation;
				for( int cTrack=0; cTrack < nAnimationMeshTrack; cTrack++ )
				{
					pMeshAnimation = pMesh->m_lstMeshAnimation[ cTrack ];

					// 每个顶点位置
					int iAniSize = (int) pMeshAnimation->m_lstPosition.size();
					if ( iAniSize == 0 )
					{
						pMeshAnimation->m_lstPosition.resize( pMesh->m_nVertexCount );
						pMeshAnimation->m_lstColor.resize( pMesh->m_nVertexCount );
						pMeshAnimation->m_lstNormal.resize( pMesh->m_nVertexCount );
						pMeshAnimation->m_lstTangentU.resize( pMesh->m_nVertexCount );
						pMeshAnimation->m_lstTangentV.resize( pMesh->m_nVertexCount );
						pMeshAnimation->m_lstTextureCoord.resize( pMesh->m_nVertexCount );
					}
					// 动画变换位置与顶点数至少相同
					else if ( iAniSize < pMesh->m_nVertexCount )
					{
						// 如果动画变换位置小于顶点数则增加一些动画位置
						int iCount = pMesh->m_nVertexCount - iAniSize;
						for ( int iAdd = 0; iAdd < iCount; ++iAdd  )
						{
							pMeshAnimation->m_lstPosition.push_back( Point3(0,0,0) );
							pMeshAnimation->m_lstColor.push_back( color(0,0,0,0) );
							pMeshAnimation->m_lstNormal.push_back( color(0,0,0,0) );
							pMeshAnimation->m_lstTangentU.push_back( color(0,0,0,0) );
							pMeshAnimation->m_lstTangentV.push_back( color(0,0,0,0) );
							pMeshAnimation->m_lstTextureCoord.push_back( Point2(0.0f,0.0f) );
						}
					}

					// 写入每个顶点的位置
					fprintf(fp, "<PositionList Size=\"%d\">\r\n", pMeshAnimation->m_lstPosition.size());
					for (int cPosition = 0; cPosition < pMeshAnimation->m_lstPosition.size(); cPosition++) {
						fprintf(fp, "<Position>%f %f %f</Position>\r\n", pMeshAnimation->m_lstPosition[cPosition].x, pMeshAnimation->m_lstPosition[cPosition].y, pMeshAnimation->m_lstPosition[cPosition].z);
					}
					fprintf(fp, "</PositionList>\r\n");

					// 写入顶点颜色
					fprintf(fp, "<ColorList Size=\"%d\">\r\n", pMeshAnimation->m_lstColor.size());
					for (int cColor = 0; cColor < pMeshAnimation->m_lstColor.size(); cColor++) {
						fprintf(fp, "<Color>%c %c %c %c</Color>\r\n", pMeshAnimation->m_lstColor[cColor].r, pMeshAnimation->m_lstColor[cColor].g, pMeshAnimation->m_lstColor[cColor].b, pMeshAnimation->m_lstColor[cColor].a);
					}
					fprintf(fp, "</ColorList>\r\n");

					// 写入顶点法线
					fprintf(fp, "<NormalList Size=\"%d\">\r\n", pMeshAnimation->m_lstNormal.size());
					for (int cNormal = 0; cNormal < pMeshAnimation->m_lstNormal.size(); cNormal++) {
						fprintf(fp, "<Normal>%c %c %c %c</Normal>\r\n", pMeshAnimation->m_lstNormal[cNormal].r, pMeshAnimation->m_lstNormal[cNormal].g, pMeshAnimation->m_lstNormal[cNormal].b, pMeshAnimation->m_lstNormal[cNormal].a);
					}
					fprintf(fp, "</NormalList>\r\n");

					// 写入切线U
					fprintf(fp, "<TangentUList Size=\"%d\">\r\n", pMeshAnimation->m_lstTangentU.size());
					for (int cTangentU = 0; cTangentU < pMeshAnimation->m_lstTangentU.size(); cTangentU++) {
						fprintf(fp, "<TangentU>%c %c %c %c</TangentU>\r\n", pMeshAnimation->m_lstTangentU[cTangentU].r, pMeshAnimation->m_lstTangentU[cTangentU].g, pMeshAnimation->m_lstTangentU[cTangentU].b, pMeshAnimation->m_lstTangentU[cTangentU].a);
					}
					fprintf(fp, "</TangentUList>\r\n");

					// 写入切线V
					fprintf(fp, "<TangentVList Size=\"%d\">\r\n", pMeshAnimation->m_lstTangentV.size());
					for (int cTangentV = 0; cTangentV < pMeshAnimation->m_lstTangentV.size(); cTangentV++) {
						fprintf(fp, "<TangentV>%c %c %c %c</TangentV>\r\n", pMeshAnimation->m_lstTangentV[cTangentV].r, pMeshAnimation->m_lstTangentV[cTangentV].g, pMeshAnimation->m_lstTangentV[cTangentV].b, pMeshAnimation->m_lstTangentV[cTangentV].a);
					}
					fprintf(fp, "</TangentVList>\r\n");

					// 写入UV坐标
					fprintf(fp, "<UVList Size=\"%d\">\r\n", pMeshAnimation->m_lstTextureCoord.size());
					for (int cUV = 0; cUV < pMeshAnimation->m_lstTextureCoord.size(); cUV++) {
						fprintf(fp, "<UV>%d %d</UV>\r\n", pMeshAnimation->m_lstTextureCoord[cUV].x, pMeshAnimation->m_lstTextureCoord[cUV].y);
					}
					fprintf(fp, "</UVList>\r\n");
				}/* end for */
				fprintf(fp, "</MeshOrTextureAnimation>\r\n");
			}/* end if */
		}

		// 关键帧数量不为0并且非骨骼动画
		if( pMesh->m_lstAnimationPositionKey.size() > 0 && !(pMesh->m_nAnimationType & REAT_BONE) ) {
			fprintf(fp, "<MeshKeyFrame>\r\n");

			// 写入关键帧位置
			fprintf(fp, "<PositionKey Size=\"%d\">\r\n", pMesh->m_lstAnimationPositionKey.size());
			for (int cPosition = 0; cPosition < pMesh->m_lstAnimationPositionKey.size(); cPosition++) {
				fprintf(fp, "<Position>%f %f %f</Position>\r\n", pMesh->m_lstAnimationPositionKey[cPosition].x, pMesh->m_lstAnimationPositionKey[cPosition].y, pMesh->m_lstAnimationPositionKey[cPosition].z);
			}
			fprintf(fp, "</PositionKey>\r\n");

			// 写入关键帧旋转
			fprintf(fp, "<RotateKey Size=\"%d\">\r\n", pMesh->m_lstAnimationRotateKey.size());
			for (int cRotate = 0; cRotate < pMesh->m_lstAnimationRotateKey.size(); cRotate++) {
				fprintf(fp, "<Rotate>%f %f %f %f</Rotate>\r\n", pMesh->m_lstAnimationRotateKey[cRotate].x, pMesh->m_lstAnimationRotateKey[cRotate].y, pMesh->m_lstAnimationRotateKey[cRotate].z, pMesh->m_lstAnimationRotateKey[cRotate].w);
			}
			fprintf(fp, "</RotateKey>\r\n");

			// 写入关键帧比例
			fprintf(fp, "<ScaleKey Size=\"%d\">\r\n", pMesh->m_lstAnimationScaleKey.size());
			for (int cScale = 0; cScale < pMesh->m_lstAnimationScaleKey.size(); cScale++) {
				fprintf(fp, "<Scale>%f %f %f</Scale>\r\n", pMesh->m_lstAnimationScaleKey[cScale].x, pMesh->m_lstAnimationScaleKey[cScale].y, pMesh->m_lstAnimationScaleKey[cScale].z);
			}
			fprintf(fp, "</ScaleKey>\r\n");

			// 写入关键帧能见范围
			fprintf(fp, "<VisibilityKey Size=\"%d\">\r\n", pMesh->m_lstAnimationVisibilityKey.size());
			for (int cVisibility = 0; cVisibility < pMesh->m_lstAnimationVisibilityKey.size(); cVisibility++) {
				fprintf(fp, "<Visibility>%f</Visibility>\r\n", pMesh->m_lstAnimationVisibilityKey[cVisibility]);
			}
			fprintf(fp, "</VisibilityKey>\r\n");

			fprintf(fp, "</MeshKeyFrame>\r\n");
		}

		fprintf(fp, "</MeshNode>");
	}
	fprintf(fp, "</Mesh>\r\n");

	return bBoneType;
}

// 写入骨骼信息到XML文件
void Red3DMaxEffectUtil2008::r3dWriteEffectBoneToXML(FILE *fp, vector< BoneData * > &lstBone, bool bBoneType) {
	// 写入骨骼数量
	int iBoneSize = bBoneType ? (int) lstBone.size() : 0;
	fwrite( &iBoneSize, sizeof(int), 1, fp );
	fprintf(fp, "<BoneList Size=\"%d\">", iBoneSize);
	if (iBoneSize > 0) {
		// 遍历骨骼写入骨骼数据
		for ( int iBone = 0; iBone < iBoneSize; ++iBone )
		{
			BoneData * pBoneData = m_vecBoneList[ iBone ];
			fprintf(fp, "<Name>%d</Name>\r\n", pBoneData->strBoneName.c_str());
			fprintf(fp, "<ParentID>%d</ParentID>\r\n", pBoneData->iParentID);

			float fSave[ 16 ];
			RHelper::Matrix3ToFloat16( pBoneData->matWorld, fSave );
			fprintf(fp, "<WorldMatrix>\r\n");
			r3dWriteFloatMatrixToXML(fp, fSave);
			fprintf(fp, "</WorldMatrix>\r\n");

			RHelper::Matrix3ToFloat16( pBoneData->matLocal, fSave );
			fprintf(fp, "<LocalMatrix>\r\n");
			r3dWriteFloatMatrixToXML(fp, fSave);
			fprintf(fp, "</LocalMatrix>\r\n");

			int iSize = (int)pBoneData->vecPositionList.size();
			
			fprintf(fp, "<PositionList Size=\"%d\">\r\n", iSize);
			for (int cPosition = 0; cPosition < iSize; cPosition++) {
				fprintf(fp, "<Position>%f %f %f</Position>\r\n", pBoneData->vecPositionList[cPosition].x, pBoneData->vecPositionList[cPosition].y, pBoneData->vecPositionList[cPosition].z);
			}
			fprintf(fp, "</PositionList>\r\n");

			fprintf(fp, "<RotationList Size=\"%d\">\r\n", iSize);
			for (int cRotation = 0; cRotation < iSize; cRotation++) {
				fprintf(fp, "<Rotation>%f %f %f %f</Rotation>\r\n", pBoneData->vecRotationList[cRotation].x, pBoneData->vecRotationList[cRotation].y, pBoneData->vecRotationList[cRotation].z, pBoneData->vecRotationList[cRotation].w);
			}
			fprintf(fp, "</RotationList>\r\n");

			fprintf(fp, "<RotationList Size=\"%d\">\r\n", iSize);
			for (int cScale = 0; cScale < iSize; cScale++) {
				fprintf(fp, "<Scale>%f %f %f</Scale>\r\n", pBoneData->vecScaleList[cScale].x, pBoneData->vecScaleList[cScale].y, pBoneData->vecScaleList[cScale].z);
			}
			fprintf(fp, "</RotationList>\r\n");
		}
	}
	fprintf(fp, "</BoneList>");
}

// 写入特效XML文件
void Red3DMaxEffectUtil2008::r3dWriteEffectXML( string strFileName ) {
	FILE *fp = fopen( strFileName.c_str(), "wb" );
	if( fp == NULL )
		return;

	r3dWriteEffectTextureToXML(fp, m_lstFinalTextureName);							// 写入纹理信息
	bool b = r3dWriteEffectMeshToXML(fp, m_lstMesh);								// 写入模型信息
	r3dWriteEffectBoneToXML(fp, m_vecBoneList, b);									// 写入骨骼信息
	
	fclose(fp);
	return;
}

/*! \fn SaveEffectFile
 *	\brief 保存r3e文件
 */
void Red3DMaxEffectUtil2008::SaveEffectFile( string strFilename )
{
	// 写入XML文件后缀为.XME
	string strXME = strFilename;
	RHelper::ReplaceExt(strXME, "XME");
	r3dWriteEffectXML(strXME);

	FILE *fp = fopen( strFilename.c_str(), "wb" );
	if( fp == NULL )
		return;

	// 写入纹理数量
	int nTextureCount = (int)m_lstFinalTextureName.size();
	fwrite( &nTextureCount, sizeof( int ), 1, fp );

	// 依次写入纹理路径
	for( int cTexture=0; cTexture < nTextureCount; cTexture++ )
	{
		int iLen = (int) m_lstFinalTextureName[ cTexture ].length();
		fwrite( &iLen, sizeof( int ), 1, fp );
		fwrite( m_lstFinalTextureName[ cTexture ].c_str(), sizeof( char ) * iLen, 1, fp );
	}

	int nMeshCount = (int)m_lstMesh.size();
	bool bBoneType = false;

	// 模型数量
	fwrite( &nMeshCount, sizeof( int ), 1, fp );

	// 写入模型信息
	for( int cMesh=0; cMesh < nMeshCount; cMesh++ )
	{
		RMesh *pMesh = m_lstMesh[ cMesh ];												// 获取网格模型
		fwrite( &pMesh->m_nNode, sizeof( int ), 1, fp );								// 节点对象ID
		fwrite( &pMesh->m_nParent, sizeof( int ), 1, fp );								// 父节点的ID
		int iMeshNameLen = (int) strlen( pMesh->m_szName );								
		fwrite( &iMeshNameLen, sizeof( int ), 1, fp );									// 网格的名称的长度
		fwrite( &pMesh->m_szName, sizeof( char ) * iMeshNameLen, 1, fp );				// 网格的名称

		// 写入模型对象的世界矩阵，本地矩阵，以及世界逆矩阵
		float fSave[ 16 ];
		RHelper::Matrix3ToFloat16( pMesh->m_matWorld, fSave );
		fwrite( fSave, sizeof( float ) * 16, 1, fp );
		RHelper::Matrix3ToFloat16( pMesh->m_matLocal, fSave );
		fwrite( fSave, sizeof( float ) * 16, 1, fp );
		RHelper::Matrix3ToFloat16( pMesh->m_matInvWorld, fSave );
		fwrite( fSave, sizeof( float ) * 16, 1, fp );
		
		// 写入顶点数量，索引数据，广告牌类型，动画类型
		fwrite( &pMesh->m_nVertexCount, sizeof( int ), 1, fp );
		fwrite( &pMesh->m_nIndexCount, sizeof( int ), 1, fp );
		fwrite( &pMesh->m_nBillboardType, sizeof( int ), 1, fp );
		fwrite( &pMesh->m_nAnimationType, sizeof( int ), 1, fp );

		// 确定动画的类型
		if( pMesh->m_nAnimationType & REAT_MESH || pMesh->m_nAnimationType & REAT_TEXTURE )
			pMesh->m_bMeshAnimation = true;
		else
			pMesh->m_bMeshAnimation = false;

		fwrite( &pMesh->m_bMeshAnimation, sizeof( bool ), 1, fp );
		int nTrack = (int)pMesh->m_lstAnimationPositionKey.size();			// 关键帧帧的数量

		if( nTrack == 0 )
		{
			MessageBoxA( NULL, "关键帧为空.", 0, 0 );
		}
		// 写入关键帧数量
		fwrite( &nTrack, sizeof( int ), 1, fp );

		int nPrimitive = (int)pMesh->m_lstTextureID.size();					// 每个图元对应一个纹理

		// 写入图元信息
		fwrite( &nPrimitive, sizeof( int ), 1, fp );

		fwrite( &pMesh->m_lstTextureID[ 0 ], sizeof( int ) * nPrimitive, 1, fp );

		fwrite( &pMesh->m_lstPosition[ 0 ], sizeof( Point3 ) * pMesh->m_lstPosition.size(), 1, fp );
		fwrite( &pMesh->m_lstTextureCoord[ 0 ], sizeof( Point2 ) * pMesh->m_lstTextureCoord.size(), 1, fp );
		fwrite( &pMesh->m_lstColor[ 0 ], sizeof( color ) * pMesh->m_lstColor.size(), 1, fp );
		fwrite( &pMesh->m_lstNormal[ 0 ], sizeof( color ) * pMesh->m_lstNormal.size(), 1, fp );
		fwrite( &pMesh->m_lstTangentU[ 0 ], sizeof( color ) * pMesh->m_lstTangentU.size(), 1, fp );
		fwrite( &pMesh->m_lstTangentV[ 0 ], sizeof( color ) * pMesh->m_lstTangentV.size(), 1, fp );
		fwrite( &pMesh->m_lstIndex[ 0 ], sizeof( WORD ) * pMesh->m_lstIndex.size(), 1, fp );

		// 骨骼动画
		if ( pMesh->m_nAnimationType & REAT_BONE )
		{
			bBoneType = true;
			vector< DWORD > vecBoneIndex;
			vector< DWORD > vecBoneWeight;

			vector< int > vecBoneTable;

			// 获取权重列的数量
			int iWeightSize = (int) pMesh->m_vecWeightList.size();

			// 遍历权重
			for ( int iWeight = 0; iWeight < iWeightSize; ++iWeight )
			{
				unsigned char nIndex[ 4 ];
				unsigned char nWeight[ 4 ];

				// 获取一个权重
				RWeight & kWeight = pMesh->m_vecWeightList[ iWeight ];

				// 一个模型顶点受4个骨骼的影响
				int iBone = 0;
				for ( ; iBone < 4; ++iBone )
				{
					// 当前
					if ( iBone < (int) kWeight.vecBoneIndex.size() )
					{
						bool bFind = false;
						int iTable = 0;
						for ( ; iTable < (int) vecBoneTable.size(); ++iTable )
						{
							if ( vecBoneTable[ iTable ] == kWeight.vecBoneIndex[ iBone ]  )
							{
								bFind = true;
								break;
							}
						}

						if ( bFind == false )
						{
							vecBoneTable.push_back( kWeight.vecBoneIndex[ iBone ] );
							iTable = (int)vecBoneTable.size() - 1;
						}

						nIndex[ iBone ] = iTable;
						nWeight[ iBone ] = (unsigned char)(kWeight.vecWeight[ iBone ] * 255.0f);
					}
					else
					{
						nIndex[ iBone ] = 0;
						nWeight[ iBone ] = 0;
					}
				}

				vecBoneIndex.push_back( *((DWORD *) nIndex) );
				vecBoneWeight.push_back( *((DWORD *) nWeight) );
			}

			// 写入骨骼表
			int iBoneTableSize = (int) vecBoneTable.size();
			fwrite( &iBoneTableSize, sizeof(int), 1, fp );

			if ( iBoneTableSize > 0 )
			{
				fwrite( &vecBoneTable[ 0 ], sizeof(int), iBoneTableSize, fp );
				fwrite( &vecBoneIndex[ 0 ], sizeof(DWORD), iWeightSize, fp );
				fwrite( &vecBoneWeight[ 0 ], sizeof(DWORD), iWeightSize, fp );
			}
		}
		// 但关键帧动画
		else if( pMesh->m_nAnimationType & REAT_MESH || pMesh->m_nAnimationType & REAT_TEXTURE )
		{
			// 关键帧的数量
			int nAnimationMeshTrack = (int)pMesh->m_lstMeshAnimation.size();
			fwrite( &nAnimationMeshTrack, sizeof( int ), 1, fp );

			RMeshAnimation* pMeshAnimation;
			for( int cTrack=0; cTrack < nAnimationMeshTrack; cTrack++ )
			{
				pMeshAnimation = pMesh->m_lstMeshAnimation[ cTrack ];

				// 关键帧动画的帧数
				int iAniSize = (int) pMeshAnimation->m_lstPosition.size();
				if ( iAniSize == 0 )
				{
					pMeshAnimation->m_lstPosition.resize( pMesh->m_nVertexCount );
					pMeshAnimation->m_lstColor.resize( pMesh->m_nVertexCount );
					pMeshAnimation->m_lstNormal.resize( pMesh->m_nVertexCount );
					pMeshAnimation->m_lstTangentU.resize( pMesh->m_nVertexCount );
					pMeshAnimation->m_lstTangentV.resize( pMesh->m_nVertexCount );
					pMeshAnimation->m_lstTextureCoord.resize( pMesh->m_nVertexCount );
				}
				// 关键帧数与顶点数至少相同
				else if ( iAniSize < pMesh->m_nVertexCount )
				{
					int iCount = pMesh->m_nVertexCount - iAniSize;
					for ( int iAdd = 0; iAdd < iCount; ++iAdd  )
					{
						pMeshAnimation->m_lstPosition.push_back( Point3(0,0,0) );
						pMeshAnimation->m_lstColor.push_back( color(0,0,0,0) );
						pMeshAnimation->m_lstNormal.push_back( color(0,0,0,0) );
						pMeshAnimation->m_lstTangentU.push_back( color(0,0,0,0) );
						pMeshAnimation->m_lstTangentV.push_back( color(0,0,0,0) );
						pMeshAnimation->m_lstTextureCoord.push_back( Point2(0.0f,0.0f) );
					}
				}

				fwrite( &pMeshAnimation->m_lstPosition[ 0 ], sizeof( Point3 ) * pMeshAnimation->m_lstPosition.size(), 1, fp );
				fwrite( &pMeshAnimation->m_lstColor[ 0 ], sizeof( color ) * pMeshAnimation->m_lstColor.size(), 1, fp );				
				fwrite( &pMeshAnimation->m_lstNormal[ 0 ], sizeof( color ) * pMeshAnimation->m_lstNormal.size(), 1, fp );
				fwrite( &pMeshAnimation->m_lstTangentU[ 0 ], sizeof( color ) * pMeshAnimation->m_lstTangentU.size(), 1, fp );
				fwrite( &pMeshAnimation->m_lstTangentV[ 0 ], sizeof( color ) * pMeshAnimation->m_lstTangentV.size(), 1, fp );
				fwrite( &pMeshAnimation->m_lstTextureCoord[ 0 ], sizeof( Point2 ) * pMeshAnimation->m_lstTextureCoord.size(), 1, fp );
			}
		}

		if( pMesh->m_lstAnimationPositionKey.size() > 0 && !(pMesh->m_nAnimationType & REAT_BONE) )
		{		
			fwrite( &pMesh->m_lstAnimationPositionKey[ 0 ], sizeof( Point3 ) * pMesh->m_lstAnimationPositionKey.size(), 1, fp );
			fwrite( &pMesh->m_lstAnimationRotateKey[ 0 ], sizeof( Point4 ) * pMesh->m_lstAnimationRotateKey.size(), 1, fp );
			fwrite( &pMesh->m_lstAnimationScaleKey[ 0 ], sizeof( Point3 ) * pMesh->m_lstAnimationScaleKey.size(), 1, fp );
			fwrite( &pMesh->m_lstAnimationVisibilityKey[ 0 ], sizeof( float ) * pMesh->m_lstAnimationVisibilityKey.size(), 1, fp );
		}
	}

	// 写入骨骼数量
	int iBoneSize = bBoneType ? (int) m_vecBoneList.size() : 0;
	fwrite( &iBoneSize, sizeof(int), 1, fp );
	// 遍历骨骼写入骨骼数据
	for ( int iBone = 0; iBone < iBoneSize; ++iBone )
	{
		BoneData * pBoneData = m_vecBoneList[ iBone ];
		int iLen = (int) pBoneData->strBoneName.length();
		fwrite( &iLen, sizeof(int), 1, fp );
		fwrite( pBoneData->strBoneName.c_str(), sizeof(char) * iLen, 1, fp );

		fwrite( &pBoneData->iParentID, sizeof(int), 1, fp );
		float fSave[ 16 ];
		RHelper::Matrix3ToFloat16( pBoneData->matWorld, fSave );
		fwrite( fSave, sizeof( float ) * 16, 1, fp );
		RHelper::Matrix3ToFloat16( pBoneData->matLocal, fSave );
		fwrite( fSave, sizeof( float ) * 16, 1, fp );
		
		int iSize = (int) pBoneData->vecPositionList.size();
		fwrite( &iSize, sizeof(int), 1, fp );
		fwrite( &pBoneData->vecPositionList[ 0 ], sizeof(Point3), iSize, fp );
		fwrite( &pBoneData->vecRotationList[ 0 ], sizeof(Point4), iSize, fp );
		fwrite( &pBoneData->vecScaleList[ 0 ], sizeof(Point3), iSize, fp );
	}

	fclose( fp );

	MessageBoxA( NULL, _T("Export Complete!"), _T("Effect Mesh"), MB_OK );
}

string Red3DMaxEffectUtil2008::GetSaveFileNamePath(void)
{
	char szFileName[ MAX_PATH ] = "";
	char szFileTitle[ MAX_PATH ] = "";
	OPENFILENAME	ofn;
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
	ofn.lStructSize		= sizeof( OPENFILENAME );
	ofn.hwndOwner		= hPanel;
	ofn.lpstrFile		= szFileName;
	ofn.lpstrFileTitle	= szFileTitle;
	ofn.nMaxFile		= MAX_PATH;
	ofn.nMaxFileTitle	= MAX_PATH;
	ofn.lpstrFilter	= "Red3D Effect Mesh File(*.r3d_effect_mesh)\0*.r3d_effect_mesh\0";
	ofn.lpstrDefExt	= "r3d_effect_mesh";
    if( GetSaveFileName( &ofn ) != 0 )
	{
		return szFileName;
	}
	return "";
}

/*!	\fn SetVertexWeightList
 *	\param vecWeightList 输出权重列表
 *	\param pNode 当前的节点对象
 *	\brief 设置顶点权重列表
 */
int Red3DMaxEffectUtil2008::SetVertexWeightList( vector< RWeight > & vecWeightList, INode * pNode )
{
	INT i, j;

	// 获取物理模型修改器
	Modifier* pMod = GetPhysiqueMod(pNode);
	if( pMod )
	{
		// 获取物理修改器接口指针
		IPhysiqueExport* pPhyInterface = (IPhysiqueExport*)pMod->GetInterface(I_PHYINTERFACE);
		if( pPhyInterface == NULL )
			return FALSE;

		// 获取物理修改器的内容
		IPhyContextExport *pContextInterface = (IPhyContextExport*)pPhyInterface->GetContextInterface(pNode);
		if(pContextInterface == NULL)
			return FALSE;
		pContextInterface->ConvertToRigid(TRUE);
		pContextInterface->AllowBlending(TRUE);

		// 遍历此NODE的所有节点
		INT iVtxCount = pContextInterface->GetNumberVertices();
		for ( i = 0; i < iVtxCount; i++ )
		{
			// 获取每个顶点的物理信息
			IPhyVertexExport *pVtxInterface = (IPhyVertexExport*)pContextInterface->GetVertexInterface(i);
			if ( pVtxInterface == NULL )
				return FALSE;//continue;

			// 获取点的类型
			INT iVtxType = pVtxInterface->GetVertexType();

			// 如果是骨骼
			// 从代码来看有两种骨骼形式,一种是可进行融合操作的,一种是不可以的

			RWeight VertexWeight;
			if ( iVtxType == RIGID_TYPE )	// NON BLEND TYPE
			{
				// 获取骨骼节点
				INode *pBoneNode = ((IPhyRigidVertex*)pVtxInterface)->GetNode();								// 获取当前顶点受到的骨骼影响
				if(pBoneNode == NULL)
					return FALSE;

				// 骨骼ID
				INT iBoneID = GetBoneID( pBoneNode );
				if ( iBoneID < 0 )
				{
					char szMsg[100];
					sprintf(szMsg, "%s骨骼的ID出错!", pNode->GetName(), i);
					MessageBoxA( NULL, szMsg, "Red3DMaxEffectUtil2008", MB_OK | MB_ICONERROR );
					return FALSE;
				}

				// 压入骨骼的名称,索引,默认权重
				VertexWeight.vecBoneName.push_back( pBoneNode->GetName() );
				VertexWeight.vecBoneIndex.push_back( iBoneID );
				VertexWeight.vecWeight.push_back( 1.0f );

				// 压入权重队列
				vecWeightList.push_back( VertexWeight );
			}
			else if ( iVtxType == RIGID_BLENDED_TYPE )	// BLEND TYPE
			{
				// 可融合的刚体顶点
				IPhyBlendedRigidVertex * pVtxBlendedInterface = (IPhyBlendedRigidVertex *) pVtxInterface;		// 获取当前顶点受到的骨骼影响

				// 遍历骨骼
				INT iBoneCount = pVtxBlendedInterface->GetNumberNodes();
				for ( j = 0; j < iBoneCount; j++ )
				{
					INode *pBoneNode = pVtxBlendedInterface->GetNode(j);
					INT iBoneID = GetBoneID( pBoneNode );
					if(iBoneID < 0)
					{
						char szMsg[100];
						sprintf(szMsg, "%s骨骼的ID出错!", pNode->GetName(), i);
						MessageBoxA( NULL, szMsg, "Red3DMaxEffectUtil2008", MB_OK | MB_ICONERROR );
						return FALSE;
					}

					bool bSame = false;

					// 遍历一个顶点的权重结构
					for ( int k = 0; k < (int) VertexWeight.vecBoneIndex.size(); ++k )
					{
						// 如果出现重复,则增加它权重的数值
						if ( VertexWeight.vecBoneIndex[ k ] == iBoneID )
						{
							VertexWeight.vecWeight[ k ] += pVtxBlendedInterface->GetWeight(j);
							bSame = true;
							break;
						}
					}

					// 如果不相同则增加
					if ( bSame == false )
					{
						VertexWeight.vecBoneName.push_back( pBoneNode->GetName() );
						VertexWeight.vecBoneIndex.push_back( iBoneID );
						VertexWeight.vecWeight.push_back( pVtxBlendedInterface->GetWeight(j) );
					}
				}

				// 压入一个权重
				vecWeightList.push_back( VertexWeight );
			}
			else
			{
				// 直接压入一个权重
				vecWeightList.push_back( VertexWeight );
			}
		}

		// 释放内存
		pPhyInterface->ReleaseContextInterface(pContextInterface);
		pMod->ReleaseInterface(I_PHYINTERFACE, pPhyInterface);


		// 遍历所有顶点,一个顶点对应一个顶点权重
		set<INT> BoneIndices;
		for ( i = 0; i < (int)vecWeightList.size(); i++ )
		{
			// 遍历一个顶点的骨骼
			for ( j = 0; j < (int)vecWeightList[ i ].vecBoneIndex.size(); j++ )
			{
				INT iBoneIndex = vecWeightList[ i ].vecBoneIndex[ j ];
				BoneIndices.insert(iBoneIndex);
			}
		}

		// 返回总共骨骼的数量
		return (int) BoneIndices.size();
	}

	// 如果没有物理修改器直接返回皮肤修改器
	pMod = GetSkinMod(pNode);

	if ( pMod )
	{
		// 获取皮肤接口
		ISkin * pSkinningInterface = static_cast<ISkin*>(pMod->GetInterface(I_SKIN)); 
		if ( pSkinningInterface == NULL )
			return FALSE; 

		ISkinContextData *pContextData = pSkinningInterface->GetContextInterface( pNode );

		// 获取皮肤的顶点个数
		INT iVtxCount = pContextData->GetNumPoints();

		// 遍历顶点个数
		for ( i = 0; i < iVtxCount; i++ )
		{
			// 皮肤关联的骨骼数量
			INT iBoneCount = pContextData->GetNumAssignedBones( i );

			RWeight VertexWeight;

			// 遍历此顶点的骨骼
			for ( int j = 0; j < iBoneCount; j++ )
			{
				// 获取这个骨骼的节点
				INode * pBoneNode = pSkinningInterface->GetBone( pContextData->GetAssignedBone( i, j ) );
				INT iBoneID = GetBoneID( pBoneNode );		// 取出骨骼ID

				// 如果骨骼ID小于0
				if ( iBoneID < 0 )
				{
					char szMsg[100];
					sprintf(szMsg, "%s骨骼的ID出错!", pNode->GetName(), i);
					MessageBoxA( NULL, szMsg, "Red3DMaxEffectUtil2008", MB_OK | MB_ICONERROR );
					return FALSE;
				}

				bool bSame = false;
				for ( int k = 0; k < (int) VertexWeight.vecBoneIndex.size(); ++k )
				{
					// 如果一个骨骼重复影响一个顶点则增加它的权重值
					if ( VertexWeight.vecBoneIndex[ k ] == iBoneID )
					{
						VertexWeight.vecWeight[ k ] += pContextData->GetBoneWeight( i, j );
						bSame = true;
						break;
					}
				}

				// 如果没有则直接压入
				if ( bSame == false )
				{
					VertexWeight.vecBoneName.push_back( pBoneNode->GetName() );
					VertexWeight.vecBoneIndex.push_back( iBoneID );
					VertexWeight.vecWeight.push_back( pContextData->GetBoneWeight( i, j ) );
				}
			}

			vecWeightList.push_back( VertexWeight );
		}				


		// 计算骨骼的数量
		set<INT> BoneIndices;
		for ( i = 0; i < (int)vecWeightList.size(); i++ )
		{
			for ( j = 0; j < (int)vecWeightList[ i ].vecBoneIndex.size(); j++ )
			{
				INT iBoneIndex = vecWeightList[ i ].vecBoneIndex[ j ];
				BoneIndices.insert(iBoneIndex);
			}
		}

		return (int) BoneIndices.size();

	}

	return 0;
}

// 获取物理修改器
Modifier* Red3DMaxEffectUtil2008::GetPhysiqueMod(INode *pNode)
{
	Object *pObj = pNode->GetObjectRef();
	if( pObj == NULL ) return NULL;

	// 节点的名称
	char szName[ 256 ];
	strcpy( szName, pNode->GetName() );

	while( pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID && pObj )
	{
		// Yes -> Cast.
		IDerivedObject *pDerivedObj = static_cast<IDerivedObject*>( pObj );

		// 遍历当前对象的所有修改器栈
		int ModStackIndex = 0;
		while( ModStackIndex < pDerivedObj->NumModifiers() )
		{
			// 获取正确的修改器栈
			Modifier* pMod = pDerivedObj->GetModifier( ModStackIndex );
			Class_ID Temp = pMod->ClassID();
			if( pMod->ClassID() == Class_ID( PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B ) )				
				return pMod;

			// 下一个修改器栈
			ModStackIndex++;
		}

		pObj = pDerivedObj->GetObjRef();
	}

	return NULL;
}

// 获取骨骼ID
int Red3DMaxEffectUtil2008::GetBoneID( INode * pNode )
{
	// 遍历骨骼表
	for ( int i = 0; i < (int) m_vecBoneList.size(); ++i )
	{
		BoneData * pBone = m_vecBoneList[ i ];
		// 找到当前骨骼
		if ( pBone->pNode == pNode )
			return i;
	}

	return -1;
}

// 获取皮肤修改器
Modifier* Red3DMaxEffectUtil2008::GetSkinMod( INode *pNode )
{
	// 获取对象指针
	Object *pObj = pNode->GetObjectRef();
	if( pObj == NULL ) return NULL;

	// 节点名称
	char szName[ 256 ];
	strcpy( szName, pNode->GetName() );

	while( pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID && pObj )
	{
		// Yes -> Cast.
		IDerivedObject *pDerivedObj = static_cast<IDerivedObject*>( pObj );

		// 遍历修改器栈
		int ModStackIndex = 0;
		while( ModStackIndex < pDerivedObj->NumModifiers() )
		{
			// 获取修改器
			Modifier* pMod = pDerivedObj->GetModifier( ModStackIndex );

			// 获取修改器ID
			Class_ID Temp = pMod->ClassID();

			// 判断修改器的ID是否是"SKIN_CLASSID"
			if( pMod->ClassID() == SKIN_CLASSID )				
				return pMod;

			// 增加修改器栈索引
			ModStackIndex++;
		}

		// 获取下一个对象指针
		pObj = pDerivedObj->GetObjRef();
	}

	return NULL;
}

// 当前节点是否是骨骼
BOOL Red3DMaxEffectUtil2008::IsBone(INode* pNode)
{
	if(pNode == 0) return FALSE;

	/*
	ObjectState os = pNode->EvalWorldState(0);
	if(os.obj == 0) return FALSE;
	if(os.obj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0)) return FALSE;
	if(os.obj->ClassID() == Class_ID(BONE_CLASS_ID, 0)) return TRUE;

	Control* pCtrl = pNode->GetTMController();
	if(pCtrl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||
	pCtrl->ClassID() == BIPBODY_CONTROL_CLASS_ID) return TRUE;
	*/

	// 获取节点名称
	TCHAR* szName = pNode->GetName();
	if( _tcsstr( szName, _T("Nub") ) != NULL
		|| _tcsstr( szName, _T("nub") ) != NULL
		|| _tcsncmp( szName, _T("Scene Root"), sizeof( TCHAR ) * 10 ) == 0 )
	{
		return FALSE;
	}

	if( _tcsncmp( szName, _T("Bone"), sizeof( TCHAR ) * 4 ) == 0
		|| _tcsncmp( szName, _T("bone_"), sizeof( TCHAR ) * 5 ) == 0
		|| _tcsncmp( szName, _T("Bip0"), sizeof( TCHAR ) * 4 ) == 0
		|| _tcsncmp( szName, _T("bip0"), sizeof( TCHAR ) * 4 ) == 0
		|| _tcsncmp( szName, _T("B_"), sizeof( TCHAR ) * 2 ) == 0
		|| _tcsncmp( szName, _T("b_"), sizeof( TCHAR ) * 2 ) == 0
		|| _tcsncmp( szName, _T("NULL"), sizeof( TCHAR ) * 4 ) == 0 )
	{
		return TRUE;
	}

	return FALSE;
}

// 输出骨骼
void Red3DMaxEffectUtil2008::ExportBone( INode *pNode )
{
	// 如果不是骨骼直接退出
	if ( IsBone( pNode ) == FALSE )
		return;

	// 确认当前节点的法线朝向
	int v1, v2, v3;	
	bool bNegScale = RHelper::TMNegParity( pNode->GetObjectTM( 0 ) );			// 此点的方向是否相反

	if( bNegScale )
	{
		v1 = 0; v2 = 1; v3 = 2;
	}
	else
	{
		v1 = 2; v2 = 1; v3 = 0;
	}

	BoneData * pBoneData = new BoneData;							// 分配骨骼内存
	pBoneData->pNode = pNode;										// 指定骨骼的节点
	pBoneData->strBoneName = pNode->GetName();						// 节点名称赋值给骨骼名称

	INode *pParent = pNode->GetParentNode();						// 获取节点的父指针
	INode *pRoot = ip->GetRootNode();								// 获取根节点
	
	// 如果父节点不是根节点则获取父节点的ID
	// 如果是父节点是根节点则父节点的ID为-1
	if( pParent != pRoot )
		pBoneData->iParentID = GetBoneID( pNode->GetParentNode() );	
	else
		pBoneData->iParentID = -1;	

	// 存在父矩阵并且不是根节点
	if( pParent && !pParent->IsRootNode()) {
		// 使用当前节点的局部矩阵 * 父矩阵的逆矩阵 = 当前节点在父空间的矩阵
		RHelper::MAXtoD3D( pNode->GetNodeTM( 0 ) * Inverse( pNode->GetParentTM( 0 ) ), pBoneData->matLocal );
	} else {
		// 如果父节点是根节点则当前节点的局部矩阵已然是相对于世界矩阵而言的矩阵
		RHelper::MAXtoD3D( pNode->GetNodeTM( 0 ), pBoneData->matLocal  );
	}

	// 世界矩阵
	RHelper::MAXtoD3D( pNode->GetNodeTM( 0 ), pBoneData->matWorld  );

	// 设置当前节点的动画关键帧列表
	SetAnimationKeyList( pBoneData, pNode );

	// 增加一个骨骼
	m_vecBoneList.push_back( pBoneData );
}

void Red3DMaxEffectUtil2008::SetAnimationKeyList( BoneData * pBoneData, INode * pNode )
{
	TimeValue	start = ip->GetAnimRange().Start();			// 动画起始帧
	TimeValue	end = ip->GetAnimRange().End();				// 动画完成
	int			delta = GetTicksPerFrame();					// 获取帧间隔
	TimeValue	t;
	Matrix3		tm;
	AffineParts ap;
	Point3		pos, prevPos;
	int nFrame = 1;

	Quat qPrev;
	qPrev.Identity();

	Point3		scale;

	// 遍历当前骨骼的每帧情况
	for( t = start; t < end + delta; t += delta )
	{
		Matrix3 mtTM = pNode->GetNodeTM( t ) * Inverse( pNode->GetParentTM( t ) );

		RHelper::MAXtoD3D( mtTM );

		// 将当前变换矩阵分解为"平移","旋转","比例"三个分量
		decomp_affine( mtTM, &ap );
		pos = ap.t;						// 取出平移向量

		// 赋值当前的平移位置
		Point3 vecPosKey;
		vecPosKey.x = pos.x;
		vecPosKey.y = pos.y;
		vecPosKey.z = pos.z;

		// 压入到当前骨骼的平移位置队列
		pBoneData->vecPositionList.push_back( vecPosKey );

		//Quat q = ap.q / qPrev;
		//qPrev  = ap.q;

		Quat q = ap.q;					// 取出旋转四元组
		// ? 为什么这里要取反
		Point4 vecRotateKey;
		vecRotateKey.x = -q.x;
		vecRotateKey.y = -q.y;
		vecRotateKey.z = -q.z;
		vecRotateKey.w = q.w;

		// 压入当前旋转队列
		pBoneData->vecRotationList.push_back( vecRotateKey );

		// 取比例
		scale = ap.k;
		Point3 vecScaleKey;
		vecScaleKey.x = scale.x;
		vecScaleKey.y = scale.y;
		vecScaleKey.z = scale.z;

		pBoneData->vecScaleList.push_back( vecScaleKey );

		// 下一帧
		nFrame++;
	}
}