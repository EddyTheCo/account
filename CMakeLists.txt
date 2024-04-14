cmake_minimum_required(VERSION 3.24)

option(USE_QML "Create QML types and modules" ON)
include(${CMAKE_CURRENT_BINARY_DIR}/local_conf.cmake OPTIONAL)

include(FetchContent)
FetchContent_Declare(
	ccommon
	GIT_REPOSITORY https://github.com/EddyTheCo/Common.git
	GIT_TAG v0.3.1
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

if(USE_QML)
	find_package(Qt6 COMPONENTS Core Gui Quick Qml)
else()
	find_package(Qt6 COMPONENTS Core)
endif()


FetchContent_Declare(
	QtBip39
	GIT_REPOSITORY https://github.com/EddyTheCo/QBip39.git
	GIT_TAG develop
	FIND_PACKAGE_ARGS 0.3 CONFIG
	)
FetchContent_MakeAvailable(QtBip39)

FetchContent_Declare(
	QtSlip10
	GIT_REPOSITORY https://github.com/EddyTheCo/Qslip10.git
	GIT_TAG develop
	FIND_PACKAGE_ARGS 0.2 CONFIG
	)
FetchContent_MakeAvailable(QtSlip10)

FetchContent_Declare(
	Qted25519
	GIT_REPOSITORY https://github.com/EddyTheCo/Qed25519.git
	GIT_TAG develop
	FIND_PACKAGE_ARGS 0.1 CONFIG
	)
FetchContent_MakeAvailable(Qted25519)

FetchContent_Declare(
	QtIotaBlock
	GIT_REPOSITORY https://github.com/EddyTheCo/Qblock-IOTA.git
	GIT_TAG develop
	FIND_PACKAGE_ARGS 0.4 CONFIG
	)
FetchContent_MakeAvailable(QtIotaBlock)

FetchContent_Declare(
	Qtbech32
	GIT_REPOSITORY https://github.com/EddyTheCo/Qbech32.git
	GIT_TAG develop
	FIND_PACKAGE_ARGS 0.1 CONFIG
	)
FetchContent_MakeAvailable(Qtbech32)

FetchContent_Declare(
	QtVault
	GIT_REPOSITORY https://github.com/EddyTheCo/QVault.git
	GIT_TAG develop
	FIND_PACKAGE_ARGS 0.2 CONFIG
	)
FetchContent_MakeAvailable(QtVault)


if (Qt6_FOUND)
	qt_standard_project_setup()
	add_library(account src/account.cpp include/account.hpp)
	set_target_properties(account PROPERTIES VERSION ${VERSION} SOVERSION ${VERSION_MAJOR})
	if(USE_QML)
		qt6_add_qml_module(account
			URI  Esterv.Iota.Account
			VERSION 1.0
			QML_FILES
			"qml/AccountSettings.qml"
			"qml/SetPassword.qml"
			"qml/ChangePassword.qml"
			RESOURCE_PREFIX
			"/esterVtech.com/imports"
			OUTPUT_TARGETS out_targets_var
			OUTPUT_DIRECTORY
			${CMAKE_BINARY_DIR}/Esterv/Iota/Account
			IMPORT_PATH ${CMAKE_BINARY_DIR}
			)


		add_library(${PROJECT_NAME}::accountplugin ALIAS accountplugin)

		target_link_libraries(account PUBLIC Qt6::Gui Qt6::Quick)
		target_compile_definitions(account PRIVATE USE_QML)
		install(TARGETS ${out_targets_var}
			EXPORT ${PROJECT_NAME}-config
			DESTINATION ${CMAKE_INSTALL_LIBDIR}
			COMPONENT account
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT account
			)
		install(DIRECTORY ${CMAKE_BINARY_DIR}/Esterv/Iota/Account
			DESTINATION ${CMAKE_INSTALL_LIBDIR}/Esterv/Iota
			COMPONENT account
			)
		install(TARGETS accountplugin
			EXPORT ${PROJECT_NAME}-config
			DESTINATION ${CMAKE_INSTALL_LIBDIR}/Esterv/Iota/Account
			COMPONENT account
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT account
			)
	endif(USE_QML)

	add_library(${PROJECT_NAME}::account ALIAS account)
	target_link_libraries(account PUBLIC Qt6::Core)
	target_compile_definitions(account PRIVATE WINDOWS_ACCOU)
	target_include_directories(account PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Esterv/>")

	target_link_libraries(account PUBLIC QtSlip10::qslip10 QtIotaBlock::qblock Qtbech32::qbech32 Qted25519::qed25519 QtVault::qvault)
	target_link_libraries(account PRIVATE QtBip39::qbip39)

	install(TARGETS account
		EXPORT ${PROJECT_NAME}-config
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		COMPONENT account
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT account
		)
	install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Esterv
		COMPONENT ${PROJECT_NAME}
		)

	build_exports()
	if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
		add_subdirectory(examples)
		build_cpack()
	endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

endif(Qt6_FOUND)


if(BUILD_DOCS)
	build_docs()
endif(BUILD_DOCS)

