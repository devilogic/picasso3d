/**************************************************
 * 简介: 特效粒子导出类
 * 描述: 用于导出特效系统粒子在任意的导出插件
 *
 * 作者: devilogic
 * 联系方式: logic.yan@gmail.com
 **************************************************/
#if !defined(__EFFECT_EXPORT_H__)
#define __EFFECT_EXPORT_H__

#include "libcommon/base.h"
#include "libmath/color.h"

namespace picasso {

	namespace picasso_effect {

		const __integer __MAX_TRIANGLE__ 64;

		class class_effect_export {
		public:
			
			/*! \class class_effect_vertex
			 *  \brief 在导出时使用的内部特效顶点类
			 */
			class class_effect_vertex {
			public:
				Point3 _position;                 /*!< 位置 */
				Point2 _texture_coordinate;       /*!< 纹理坐标0-1范围内 */
				color _normal;                    /*!< 法线方向 */
				color _tangent_u;                 /*!< 切线U */
				color _tangent_v;                 /*!< 切线V */
			};

			/*! \class class_texture_mat
			 *  \brief 在导出时使用的内部纹理材质类
			 */
			class class_texture_mat {
			public:		
				std::string _texture_name;        /*!< 纹理名称 */
				__integer _mat_id;                /*!< 材质ID */
				__integer _mat_sub_id;            /*!< 子材质ID */
			};

			/*! \class class_mesh_animation
			 *  \brief 在导出时使用的模型动画类
			 *  \details 模型动画有两种，一种是位移动画，一种是纹理动画
			 */
			class class_mesh_animation {
			public:		
				std::vector< Point3 > _position;                        /*!< 位置信息 */
				std::vector< Point2 > _texture_coordinate;              /*!< 纹理坐标 */
				std::vector< color > _color;                            /*!< 颜色队列 */
				std::vector< color > _normal;                           /*!< 法线队列 */
				std::vector< color > _tangent_u;                        /*!< 切线U列表 */
				std::vector< color > _tangent_v;                        /*!< 切线V列表 */
			};

			/*! \class class_effect_vertex
			 *  \brief 在导出时使用的内部顶点权重类
			 */
			class class_weight {
			public:
				std::vector< std::string > _name_list;                  /*!< 骨骼名称 */
				std::vector< __integer > _index_list;                   /*!< 骨骼索引 */
				std::vector< __float > _weight_list;                    /*!< 骨骼权重 */
			};

			class class_mesh {
			/*! \class class_mesh
			 *  \brief 在导出时使用的内部特效顶点类
			 */
				__integer _id;                                      /*!< 节点ID */
				__integer _parent;                                  /*!< 父节点索引 */
				__integer _vertex_count;                            /*!< 顶点计数 */
				__integer _index_count;                             /*!< 索引计数 */
				__integer _animation_type;                          /*!< 动画类型 */
				__integer _billboard_type;                          /*!< 广告牌类型 */
				__bool _is_mesh_animation;                          /*!< 是否是模型动画 */

				__char _name[64];                                   /*!< 模型网格名称 */

				Matrix3	_world;                                     /*!< 世界矩阵 */
				Matrix3	_local;                                     /*!< 本地矩阵 */
				Matrix3	_inv_world;                                 /*!< 世界逆矩阵 */

				std::vector< __integer > _texture_id_list;          /*!< 纹理ID列表 */
				std::vector< __integer > _vertex_count_list;        /*!< 顶点计数列表 */
				std::vector< __integer > _vertex_offset_list;       /*!< 顶点偏移列表 */
				std::vector< __integer > _index_count_list;         /*!< 索引计数列表 */
				std::vector< __integer > _index_offset_list;        /*!< 索引偏移列表 */

				std::vector< Point3 > _position_list;               /*!< 位置列表 */
				std::vector< Point2 > _texture_coordinate_list;     /*!< 纹理坐标列表 */
				std::vector< color > _color_list;                   /*!< 颜色列表 */
				std::vector< color > _normal_list;                  /*!< 法线列表 */
				std::vector< color > _tangent_u_list;               /*!< 切线U列表 */
				std::vector< color > _tangent_v_list;               /*!< 切线V列表 */
				std::vector< __integer > _index_list;               /*!< 索引列表 */

				std::vector< Point3 > _animation_position_key_list;         /*!< 动画关键帧位置列表 */
				std::vector< Point4 > _animation_rotate_key_list;           /*!< 动画关键帧旋转列表 */
				std::vector< Point3 > _animation_scale_key_list;            /*!< 动画关键帧比例列表 */
				std::vector< __float > _animation_visibility_Key_list;      /*!< 动画关键帧可见度列表 */
		
				std::vector< class_mesh_animation* > _mesh_animation_list;  /*!< 网格动画列表 */
				std::vector< class_weight > _weight_list;                   /*!< 权重值列表 */
				__integer _used_bone_count;                                 /*!< 使用骨骼计数 */

			public:
				class_mesh();
				~class_mesh();
			};

			class class_bone_data {
			/*! \class class_effect_vertex
			 *  \brief 在导出时使用的内部特效顶点类
			 */
			public:
				class_bone_data();
			public:
				std::string	_name;      /*!< 骨骼名称 */
				__integer _parent_id;   /*!< 父骨骼ID */

				Matrix3 _world;         /*!< 世界矩阵 */
				Matrix3 _local;         /*!< 局部矩阵 */

				std::vector< Point3 > _position_list;               /*!< 位置列表 */
				std::vector< Point4 > _rotation_list;               /*!< 旋转列表 */
				std::vector< Point3 > _scale_list;                  /*!< 比例列表 */
			};

			//////////////////////////////////////////////////

			std::vector< Mtl* > _MAXMatrial;					        /*!< 材质列表 */
			std::vector< class_texture_mat > _texture_list;	            /*!< 纹理列表,保存了纹理结构 */
			std::vector< class_mesh* > _mesh_lst;                       /*!< 模型列表 */
			std::vector< std::string > _texture_name_list;              /*!< 保存纹理图名称列表 */
			std::vector< Point3 > _save_position[__MAX_TRIANGLE__];     /*!< 图元位置信息 */
			std::vector< Point3 > _save_normal[__MAX_TRIANGLE__];       /*!< 图元法线信息 */
			std::vector< Point2 > _save_texture[__MAX_TRIANGLE__];      /*!< 图元纹理坐标信息 */
			std::vector< color > _save_color[__MAX_TRIANGLE__];         /*!< 图元颜色信息 */
			std::vector< __integer > _save_index[__MAX_TRIANGLE__];     /*!< 图元顶点索引信息 */
			std::vector< class_weight > _save_weight[__MAX_TRIANGLE__]; /*!< 图元权重信息 */
			std::vector< class_bone_data* > _bone_list;                 /*!< 骨骼列表 */
			// 以下向量标志了动画的最终输出
			std::vector< Point3 > _final_position;
			std::vector< Point3 > _final_normal;
			std::vector< Point2 > _final_texture;
			std::vector< color > _final_color;
			std::vector< __integer > _final_index;
			std::vector< std::string > _final_texture_name;

			__integer _first_frame;                    /*!< 第一帧 */
			__integer _last_frame;                     /*!< 最后帧 */
			__integer _frame_speed;                    /*!< 帧速 */
			__integer _ticks_per_frame;                /*!< 帧率 */
			__bool _is_vertex_animation;               /*!< 是否是顶点动画 */

		public:
			class_effect_export();
			virtual ~class_effect_export();

		public:
			virtual __void init();
			virtual __void release();
			virtual __void export();

		private:
			virtual __void export_bone( __void* node );

		private:
			/*!	\fn make_materia_list
			 *	\param pNode 节点指针
			 *	\brief 产生材质表
			 */
			__bool make_material_list( INode *pNode );

			/*!	\fn make_texture_list
			 *	\brief 产生纹理列表
			 */
			__bool make_texture_list();

			/*!	\fn export_mesh
			 *	\param pNode 节点指针
			 *	\brief 输出网格模型
			 */
			__void export_mesh( INode *pNode );

			/*!	\fn make_mesh
			 *	\param mesh 网格模型
			 *	\brief 产生picasso网格模型
			 */
			__void make_mesh( class_mesh* mesh );

			/*!	\fn make_mesh_animation
			 *	\param mesh 网格动画模型
			 *	\brief 产生picasso网格模型动画
			 */
			__void make_mesh_animation( class_mesh_animation* mesh );

			/*!	\fn make_animation_list
			 *	\brief 产生动画列表
			 */
			__void make_animation_list();

			/*!	\fn make_geometry
			 *	\brief 产生图元
			 */
			__bool make_geometry();

			/*!	\fn get_mesh_id
			 *	\param pNode 节点指针
			 *	\brief 获取网格模型ID
			 */
			__integer get_mesh_id( INode *pNode );

			/*!	\fn save_vertex
			 *	\param position_list 顶点位置队列
			 *	\param normal_list 法线队列
			 *	\param texture_list 纹理队列
			 *	\param color_list 颜色队列
			 *	\param weight_list 权重队列
			 *	\param mtl_list 材质队列
			 *	\param sub_mtl 子材质ID
			 *	\brief 保存顶点信息
			 */
			__bool save_vertex( Point3* position_list, Point3* normal_list, Point2* texture_list, color* color_list, class_weight* weight_list, Mtl* mtl_list, __integer sub_mat );

			/*!	\fn clear_save_buffers
			 *	\brief 清除保存缓存
			 */
			__void clear_save_buffers();

			/*!	\fn export_animation_element
			 *	\brief 输出动画元素
			 */
			__void export_animation_element();

			/*!	\fn export_animation_mesh
			 *	\param pNode 节点指针
			 *	\param t 当前节点的时间序号
			 *	\param iAniType 动画类型
			 *	\brief 输出动画元素网格
			 */
			__void export_animation_mesh( INode *pNode, __integer t, __integer animation_type );
	
			/*!	\fn fill_node_list
			 *	\param pNode 节点指针
			 *	\param node_list 节点队列
			 *	\brief 填充节点列表
			 */
			__void make_object_list( INode *pNode, std::vector< INode* >& lstNodeList );

			/*!	\fn make_vertex_weight_list
			 *	\param weight_list 权重列表
			 *	\param pNode 节点指针
			 *	\brief 保存顶点权重列表
			 */
			__integer make_vertex_weight_list( std::vector< class_weight >& weight_list, INode * pNode );

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

		__

			}/* end picasso_effect namespace */

}/* end picasso namespace */

#endif
