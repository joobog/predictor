cmake_minimum_required ( VERSION 2.8 )
# trace analyser
project ( predictor )

set ( Clustering_VERSION_MAJOR 1 )
set ( Clustering_VERSION_MINOR 0 )

#set ( CMAKE_CXX_COMPILER /usr/bin/clang++ )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3" )
#set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always" )
#set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register" )
#set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-local-addr" )

configure_file (
	"${PROJECT_SOURCE_DIR}/ta_config.h.in"
	"${PROJECT_BINARY_DIR}/ta_config.h"
	)

set ( ROOT_DIR "${PROJECT_SOURCE_DIR}/../.." )

#find_package ( Boost REQUIRED )	
#find_package ( PkgConfig )


#if ( PKG_CONFIG_FOUND )
	#pkg_check_modules ( GTKMM gtkmm-3.0 )
	#pkg_check_modules ( GLIB glib-2.0 )
	#pkg_check_modules ( GAIROMM cairomm-1.0 )
#endif ( PKG_CONFIG_FOUND )

set ( ANALYSIS "${PROJECT_SOURCE_DIR}/src/analysis" )

include_directories (
	${PROEJCT_BINARY_DIR}
	"${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/src/analysis"
	"${ROOT_DIR}/lib_src/Shark/include"
	"${ROOT_DIR}/lib/boost/include"
	)


link_directories (
	"${ROOT_DIR}/lib/shark/lib"
	"${ROOT_DIR}/lib/boost/lib"
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
	shark
	boost_iostreams 
	boost_serialization
	boost_program_options
	boost_system
	boost_filesystem
	) 
