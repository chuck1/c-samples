AC_INIT([lizard], [1.0], [charlesrymal@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])

AC_PROG_CXX

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

AC_LANG([C++])
AC_LANG_CPLUSPLUS
AC_CHECK_HEADER(cstdint)
AX_CXX_COMPILE_STDCXX_0X

CXXFLAGS='-Wall -std=gnu++0x'

AC_OUTPUT

