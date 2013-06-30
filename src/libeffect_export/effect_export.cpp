#include "libeffect_export/effect_export.h"

namespace picasso {

	namespace picasso_effect {

		class_mesh::class_mesh : _used_bone_count(0) {
			_is_mesh_animation = FALSE;
		}
		
		class_mesh::~class_mesh {
			if ( _is_mesh_animation ) {
				for ( __integer i = 0; i < _mesh_animation_list.size(); i++ ) {
					delete _mesh_animation_list[ i ];
				}
			}
		}

		class_bone_data::class_bone_data() {
			_parent_id = -1;
		}

	}/* end picasso_effect namespace */

}/* end picasso namespace */
