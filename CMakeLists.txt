cmake_minimum_required(VERSION 3.24)

option(USE_QML "Create QML types and modules" ON)
include(${CMAKE_CURRENT_BINARY_DIR}/local_conf.cmake OPTIONAL)

include(FetchContent)
FetchContent_Declare(
	ccommon
	GIT_REPOSITORY https://github.com/EddyTheCo/Common.git
	GIT_TAG v0.1.1
	)
FetchContent_MakeAvailable(ccommon)
version_from_git(
	LOG       OFF
	TIMESTAMP "%Y%m%d%H%M%S"
	)
project(IotaAccount VERSION ${VERSION} DESCRIPTION "Iota account methods " LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)
FetchContent_Declare(
	QBip39
	GIT_REPOSITORY https://github.com/EddyTheCo/QBip39.git
	GIT_TAG v0.2.0
	FIND_PACKAGE_ARGS 0.2 CONFIG
	)
FetchContent_MakeAvailable(QBip39)

FetchContent_Declare(
	QtSlip10	
	GIT_REPOSITORY https://github.com/EddyTheCo/Qslip10.git
	GIT_TAG v0.1.0 
	FIND_PACKAGE_ARGS 0.1 CONFIG  
	)
FetchContent_MakeAvailable(QtSlip10)

FetchContent_Declare(
	qed25519
	GIT_REPOSITORY https://github.com/EddyTheCo/Qed25519.git
	GIT_TAG v0.0.2
	FIND_PACKAGE_ARGS 0.0 CONFIG
	)
FetchContent_MakeAvailable(qed25519)

FetchContent_Declare(
	qblock
	GIT_REPOSITORY https://github.com/EddyTheCo/Qblock-IOTA.git
	GIT_TAG v0.3.2
	FIND_PACKAGE_ARGS 0.3 CONFIG
	)
FetchContent_MakeAvailable(qblock)

FetchContent_Declare(
	qbech32
	GIT_REPOSITORY https://github.com/EddyTheCo/Qbech32.git
	GIT_TAG v0.0.2
	FIND_PACKAGE_ARGS 0.0 CONFIG
	)
FetchContent_MakeAvailable(qbech32)


if(USE_QML)
	find_package(Qt6 COMPONENTS Core Gui Quick)
else()
	find_package(Qt6 COMPONENTS Core)
endif()

if (Qt6_FOUND)
	qt_standard_project_setup()
	add_library(account src/account.cpp include/account.hpp)
	if(USE_QML)
		qt6_add_qml_module(account
			URI  Esterv.Iota.Account
			VERSION 1.0
			QML_FILES
			"qml/AccountSettings.qml"
			RESOURCE_PREFIX
			"/esterVtech.com/imports"
			OUTPUT_TARGETS out_targets_var
			OUTPUT_DIRECTORY
			${CMAKE_BINARY_DIR}/Esterv/Iota/Account
			IMPORT_PATH ${CMAKE_BINARY_DIR}  ${CMAKE_INSTALL_LIBDIR}
			)


		add_library(${PROJECT_NAME}::accountplugin ALIAS accountplugin)

		target_link_libraries(account PUBLIC Qt6::Gui Qt6::Quick)
		target_compile_definitions(account PRIVATE USE_QML)
		install(TARGETS ${out_targets_var}
			EXPORT ${PROJECT_NAME}-config
			DESTINATION ${CMAKE_INSTALL_LIBDIR}
			COMPONENT account
			)
		install(DIRECTORY ${CMAKE_BINARY_DIR}/Esterv/Iota/Account
			DESTINATION ${CMAKE_INSTALL_LIBDIR}/Esterv/Iota
			COMPONENT account
			)
		install(TARGETS accountplugin
			EXPORT ${PROJECT_NAME}-config
			DESTINATION ${CMAKE_INSTALL_LIBDIR}/Esterv/Iota/Account
			COMPONENT account
			)

	endif(USE_QML)

	add_library(${PROJECT_NAME}::account ALIAS account)
	target_link_libraries(account PUBLIC Qt6::Core)
	target_compile_definitions(account PRIVATE WINDOWS_ACCOU)
	target_include_directories(account PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Esterv/${PROJECT_NAME}>")

	target_link_libraries(account PUBLIC QtSlip10::qslip10 qblock qbech32 qed25519)
	target_link_libraries(account PRIVATE QBip39::qbip39)

	install(TARGETS account
		EXPORT ${PROJECT_NAME}-config
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		COMPONENT account
		)
	install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Esterv/${PROJECT_NAME}
		COMPONENT account
		)

	install(EXPORT ${PROJECT_NAME}-config
		FILE ${PROJECT_NAME}-config.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Esterv
		NAMESPACE ${PROJECT_NAME}::
		COMPONENT ${PROJECT_NAME}
		)
	include(CMakePackageConfigHelpers)
	configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
		INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Esterv
		)
	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
		VERSION ${VERSION}
		COMPATIBILITY SameMajorVersion
		)
	install(FILES
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
		${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Esterv
		)
	export(EXPORT ${PROJECT_NAME}-config
		FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
		)
	if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
		add_subdirectory(examples)
		set(CPACK_PACKAGE_CONTACT "estervtech")
		include(CTest)
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${SEMVER}-${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_VERSION}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}")
		include(CPack)
	endif()

endif(Qt6_FOUND)


if(BUILD_DOCS)
	get_target_property(build_docs cmake_build_docs SOURCES)
	include(${build_docs})
endif()

