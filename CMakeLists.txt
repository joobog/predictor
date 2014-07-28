cmake_minimum_required ( VERSION 2.8 )
# trace analyser
project ( predictor )

set ( Clustering_VERSION_MAJOR 1 )
set ( Clustering_VERSION_MINOR 0 )

set ( CMAKE_CXX_COMPILER /usr/bin/clang++ )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp  -std=c++11 -O3 -Wno-deprecated-register" ) # -fdiagnostics-color=always" )

#set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --Wno-return-local-addr -std=c++11" ) # -fdiagnostics-color=always" )

configure_file (
	"${PROJECT_SOURCE_DIR}/ta_config.h.in"
	"${PROJECT_BINARY_DIR}/ta_config.h"
	)

find_package ( Boost REQUIRED )	
#find_package ( PkgConfig )


#if ( PKG_CONFIG_FOUND )
	#pkg_check_modules ( GTKMM gtkmm-3.0 )
	#pkg_check_modules ( GLIB glib-2.0 )
	#pkg_check_modules ( GAIROMM cairomm-1.0 )
#endif ( PKG_CONFIG_FOUND )

set ( ANALYSIS "${PROJECT_SOURCE_DIR}/src/analysis" )

include_directories (
	#${GTKMM_INCLUDE_DIRS}
	#${GLIB_INCLUDE_DIRS}
	#${CAIROMM_INCLUDE_DIRS}
	${PROEJCT_BINARY_DIR}
	"${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/src/analysis"
	"/home/joobog/lib/Shark/include"
	)


link_directories (
	#${GTKMM_LIBRARY_DIRS}
	#${GLIB_LIBRARY_DIRS}
	#${CAIROMM_LIBRARY_DIRS}
	"/home/joobog/lib/Shark/build/lib"
	)

add_executable ( predictor
	"src/main.cpp"
	"${ANALYSIS}/Predictor.cpp"
	"${ANALYSIS}/DTreePredictor.cpp"
	"${ANALYSIS}/ANNPredictor.cpp"
	"${ANALYSIS}/SVMPredictor.cpp"
	"${ANALYSIS}/Helpers.cpp"
	"${ANALYSIS}/Analyser.cpp"
	"${ANALYSIS}/Types.cpp"
	)

target_link_libraries ( predictor
	#${GTKMM_LIBRARIES}
	#${GLIB_LIBRARIES}
	#${CAIROMM_LIBRARIES}
	shark
	boost_iostreams 
	boost_serialization
	boost_program_options
	boost_system
	boost_filesystem
	) 
