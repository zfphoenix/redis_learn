FIND_PATH(HIREDIS_INCLUDE_DIR hiredis/hiredis.h
		DOC "The directory where hiredis.h resides")

FIND_LIBRARY ( HIREDIS_LIBRARY
		NAMES hiredis
		PATH  /usr/local/lib
		DOC "The hiredis library")

IF (HIREDIS_INCLUDE_DIR)
	SET(HIREDIS_FOUND 1 CACHE STRING "Set to 1 if hiredis is found, otherwise 0")
ELSE (HIREDIS_INCLUDE_DIR)
	SET(HIREDIS_FOUND 0 CACHE STRING "Set to 1 if hiredis is found, otherwise 0")
ENDIF (HIREDIS_INCLUDE_DIR)

MARK_AS_ADVANCED( HIREDIS_FOUND)

IF (HIREDIS_FOUND)
	MESSAGE(STATUS "find hiredis library..")
ELSE (HIREDIS_FOUND)
	MESSAGE(FATAL_ERROR "cannot find hiredis library, please install it..")
ENDIF (HIREDIS_FOUND)
