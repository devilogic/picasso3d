#if !defined(__LOGICEXCEPT_H__)
#define __LOGICEXCEPT_H__

#if defined(__cplusplus)
extern "C"
{
#endif

	namespace picasso {

		namespace picasso_common {

#define __EXCEPTION_EXECUTE_HANDLER__       1
#define __EXCEPTION_CONTINUE_SEARCH__       0
#define __EXCEPTION_CONTINUE_EXECUTION__    -1

		}/* end picasso_common */

	}/* end picasso */

#if defined(__cplusplus)
}
#endif

#endif
