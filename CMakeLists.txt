cmake_minimum_required(VERSION 3.16)
project(IOTA-account LANGUAGES CXX)

include(local_conf.cmake OPTIONAL)

find_package(Qt6 REQUIRED COMPONENTS Core )

add_library(account account.cpp include/account.hpp)

target_include_directories(account PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	"$<INSTALL_INTERFACE:include>")
target_link_libraries(account PUBLIC
	Qt6::Core
	)
include(FetchContent)
FetchContent_Declare(
	qslip10
	GIT_REPOSITORY git@github.com:EddyTheCo/Qslip10.git
	GIT_TAG main
	)
FetchContent_MakeAvailable(qslip10)
FetchContent_Declare(
        qblock
        GIT_REPOSITORY git@github.com:EddyTheCo/Qblock-IOTA.git
        GIT_TAG main
        )
FetchContent_MakeAvailable(qblock)

FetchContent_Declare(
	qbech32
	GIT_REPOSITORY git@github.com:EddyTheCo/Qbech32.git
	GIT_TAG main
	)
FetchContent_MakeAvailable(qbech32)
FetchContent_Declare(
	qed25519
	GIT_REPOSITORY git@github.com:EddyTheCo/Qed25519.git
	GIT_TAG main
	)
FetchContent_MakeAvailable(qed25519)

target_link_libraries(account PUBLIC
	qslip10 qblock
	)
target_link_libraries(account PRIVATE
	qed25519 qbech32
	)

install(TARGETS account EXPORT accountTargets DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/)

install(EXPORT accountTargets
	FILE accountTargets.cmake
	DESTINATION lib/cmake/account
	)
include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/accountConfig.cmake"
	INSTALL_DESTINATION "lib/cmake/account"
	NO_SET_AND_CHECK_MACRO
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
	)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/accountConfigVersion.cmake"
	VERSION "0.1.1.0"
	COMPATIBILITY AnyNewerVersion
	)
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/accountConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/accountConfigVersion.cmake
	DESTINATION lib/cmake/account
	)
export(EXPORT accountTargets
	FILE "${CMAKE_CURRENT_BINARY_DIR}/accountTargets.cmake"
	)
