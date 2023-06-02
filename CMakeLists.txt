cmake_minimum_required(VERSION 3.16)

include(local_conf.cmake OPTIONAL)

include(FetchContent)
FetchContent_Declare(
	ccommon
	GIT_REPOSITORY https://github.com/EddyTheCo/Common.git
	GIT_TAG main
	)
FetchContent_MakeAvailable(ccommon)
version_from_git(
	LOG       OFF
	TIMESTAMP "%Y%m%d%H%M%S"
	)
project(IOTAQml VERSION ${VERSION} LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)


find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick)
qt_standard_project_setup()


qt6_add_qml_module(account
	URI  account
	VERSION 1.0
	SOURCES
	src/account.cpp include/account.hpp
	QML_FILES
	qml/AccountQml.qml
	qml/RestoreAccount.qml
	qml/ShowSeed.qml
	RESOURCE_PREFIX
	"/esterVtech.com/imports"
	OUTPUT_TARGETS out_targets_var
	OUTPUT_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}/account
	)

target_link_libraries(account PUBLIC Qt6::Gui Qt6::Quick)

target_include_directories(account PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/account>")


include(FetchContent)
FetchContent_Declare(
	qslip10
	GIT_REPOSITORY git@github.com:EddyTheCo/Qslip10.git
	GIT_TAG v0.0.1
	FIND_PACKAGE_ARGS 0 CONFIG
	)
FetchContent_MakeAvailable(qslip10)

FetchContent_Declare(
	qaddr_bundle
	GIT_REPOSITORY https://github.com/EddyTheCo/QAddrBundle.git
	GIT_TAG v0.0.1
	FIND_PACKAGE_ARGS 0 CONFIG
	)
FetchContent_MakeAvailable(qaddr_bundle)
FetchContent_Declare(
	qed25519
	GIT_REPOSITORY https://github.com/EddyTheCo/Qed25519.git
	GIT_TAG v0.0.1
	FIND_PACKAGE_ARGS 0 CONFIG
	)
FetchContent_MakeAvailable(qed25519)

FetchContent_Declare(
	MyDesigns
	GIT_REPOSITORY https://github.com/EddyTheCo/MyDesigns.git
	GIT_TAG v0.0.1
	FIND_PACKAGE_ARGS 0 CONFIG
	)
FetchContent_MakeAvailable(MyDesigns)

target_link_libraries(account PRIVATE  qed25519 )
target_link_libraries(account PUBLIC qslip10 qaddr_bundle MyDesigns)


install(TARGETS account ${out_targets_var}
	EXPORT account-config
	DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/account
	)

install(EXPORT account-config
	FILE accountTargets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	)
include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/accountConfig.cmake"
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/accountConfigVersion.cmake"
	VERSION ${VERSION}
	COMPATIBILITY SameMajorVersion
	)
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/accountConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/accountConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
	)
export(EXPORT account-config
	FILE "${CMAKE_CURRENT_BINARY_DIR}/account-config.cmake"
	)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	include(CTest)
	set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${SEMVER}-${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_VERSION}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}")
	if(NOT BUILD_SHARED_LIBS)
		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-static")
	endif(NOT BUILD_SHARED_LIBS)
	include(CPack)
endif()

