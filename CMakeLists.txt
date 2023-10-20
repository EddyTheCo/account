cmake_minimum_required(VERSION 3.24)

include(${CMAKE_CURRENT_BINARY_DIR}/local_conf.cmake OPTIONAL)

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
project(AccountQml VERSION ${VERSION} DESCRIPTION "Account QML module" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)


find_package(Qt6 COMPONENTS Core Gui Quick)

FetchContent_Declare(
	qslip10
	GIT_REPOSITORY https://github.com/EddyTheCo/Qslip10.git
	GIT_TAG v0.0.2
	FIND_PACKAGE_ARGS 0.0 CONFIG
	)
FetchContent_MakeAvailable(qslip10)

FetchContent_Declare(
	qaddr_bundle
	GIT_REPOSITORY https://github.com/EddyTheCo/QAddrBundle.git
        GIT_TAG v0.2.2
	FIND_PACKAGE_ARGS 0.2 CONFIG
	)
FetchContent_MakeAvailable(qaddr_bundle)
FetchContent_Declare(
	qed25519
	GIT_REPOSITORY https://github.com/EddyTheCo/Qed25519.git
	GIT_TAG v0.0.2
	FIND_PACKAGE_ARGS 0.0 CONFIG
	)
FetchContent_MakeAvailable(qed25519)

FetchContent_Declare(
	MyDesigns
	GIT_REPOSITORY https://github.com/EddyTheCo/MyDesigns.git
        GIT_TAG v0.3.1
	FIND_PACKAGE_ARGS 0.3 CONFIG
	)
FetchContent_MakeAvailable(MyDesigns)


if (Qt6_FOUND)
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
		${CMAKE_BINARY_DIR}/account
		IMPORT_PATH ${CMAKE_BINARY_DIR}
		)

	target_link_libraries(account PUBLIC Qt6::Gui Qt6::Quick)

	target_compile_definitions(account PRIVATE WINDOWS_ACCOU)
	
	target_include_directories(account PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/account>")

	target_link_libraries(account PRIVATE  qed25519 )

	target_link_libraries(account PUBLIC qslip10 qaddr_bundle MyDesigns
		$<$<STREQUAL:$<TARGET_PROPERTY:MyDesigns,TYPE>,STATIC_LIBRARY>:MyDesignsplugin>
		)

	install(TARGETS account accountplugin ${out_targets_var}
		EXPORT accountTargets
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		)
	install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/account
		)

	install(DIRECTORY ${CMAKE_BINARY_DIR}/account/
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/QMLPlugins/account
		)

	install(EXPORT accountTargets
		FILE accountTargets.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/account
		)
	include(CMakePackageConfigHelpers)
	configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
		"${CMAKE_CURRENT_BINARY_DIR}/accountConfig.cmake"
		INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/account
		)
	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/accountConfigVersion.cmake"
		VERSION ${VERSION}
		COMPATIBILITY SameMajorVersion
		)
	install(FILES
		${CMAKE_CURRENT_BINARY_DIR}/accountConfig.cmake
		${CMAKE_CURRENT_BINARY_DIR}/accountConfigVersion.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/account
		)
	export(EXPORT accountTargets
		FILE "${CMAKE_CURRENT_BINARY_DIR}/accountTargets.cmake"
		)
	if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
		set(CPACK_PACKAGE_CONTACT "estervtech")
		include(CTest)
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${SEMVER}-${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_VERSION}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}")
		if(USE_THREADS)
			set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-MThread")
		endif(USE_THREADS)
		include(CPack)
	endif()

endif(Qt6_FOUND)


if(BUILD_DOCS)
	get_target_property(build_docs cmake_build_docs SOURCES)
	include(${build_docs})
endif()

