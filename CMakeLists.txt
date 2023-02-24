cmake_minimum_required(VERSION 3.16)

project(IOTA VERSION 0.1 LANGUAGES CXX)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick)

include(local_conf.cmake OPTIONAL)

qt6_add_qml_module(account
    URI  account
    VERSION 1.0
    SOURCES
    src/account.cpp include/account.hpp
    QML_FILES
    qml/AccountQml.qml
    qml/RestoreAccount.qml
    qml/ShowSeed.qml
    qml/window.qml
    RESOURCE_PREFIX
     "/esterVtech.com/imports"
    OUTPUT_TARGETS out_targets_var
    NO_IMPORT_SCAN
    IMPORTS MyDesigns
    IMPORT_PATH
    ${CMAKE_BINARY_DIR}/_deps/mydesigns-build
    OUTPUT_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/account
)



target_link_libraries(account PUBLIC Qt6::Gui Qt6::Quick)

target_include_directories(account PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    "$<INSTALL_INTERFACE:include>")


include(FetchContent)
FetchContent_Declare(
    qslip10
    GIT_REPOSITORY git@github.com:EddyTheCo/Qslip10.git
    GIT_TAG main
)
FetchContent_MakeAvailable(qslip10)

FetchContent_Declare(
    qaddr_bundle
    GIT_REPOSITORY git@github.com:EddyTheCo/QAddrBundle.git
    GIT_TAG main
)
FetchContent_MakeAvailable(qaddr_bundle)
FetchContent_Declare(
    qed25519
    GIT_REPOSITORY git@github.com:EddyTheCo/Qed25519.git
    GIT_TAG main
)
FetchContent_MakeAvailable(qed25519)

FetchContent_Declare(
    MyDesigns
    GIT_REPOSITORY git@github.com:EddyTheCo/MyDesigns.git
    GIT_TAG main
)
FetchContent_MakeAvailable(MyDesigns)

target_link_libraries(account PRIVATE  qed25519 )
target_link_libraries(account PUBLIC qslip10 qaddr_bundle MyDesigns)

target_compile_definitions(account
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

install(TARGETS account ${out_targets_var} EXPORT accountTargets DESTINATION bin )
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
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/qml ${CMAKE_CURRENT_BINARY_DIR}/)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH}  CACHE STRING "" FORCE)

