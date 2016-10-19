cmake_minimum_required(VERSION 2.8.4)

IF(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
ENDIF(POLICY CMP0042)

project(poptrie)

install(FILES poptrie.h buddy.h DESTINATION include/poptrie)

add_library(poptrie SHARED poptrie.c poptrie6.c buddy.c)
add_library(poptrie_static STATIC poptrie.c poptrie6.c buddy.c)

add_executable(poptrie_test_basic tests/basic.c poptrie.c poptrie6.c buddy.c)
add_executable(poptrie_test_basic6 tests/basic6.c poptrie.c poptrie6.c buddy.c)

set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "poptrie")
set (CPACK_DEBIAN_PACKAGE_DESCRIPTION # The format of Description: http://www.debian.org/doc/debian-policy/ch-controlfields.html#s-f-Description
"poptrie
")
set (CPACK_PACKAGE_CONTACT "Hirochika Asai")
set (CPACK_PACKAGE_VERSION_MAJOR "1")
set (CPACK_PACKAGE_VERSION_MINOR "0")
set (CPACK_PACKAGE_VERSION_PATCH "0")
set (CPACK_SOURCE_GENERATOR "")
set (CPACK_STRIP_FILES "")
set (CPACK_GENERATOR "DEB")
set (CPACK_DEBIAN_PACKAGE_MAINTAINER "nobody")
include(CPack)
