cmake_minimum_required(VERSION 3.0)

project(plasma-colorscheme-cli)

set(QT_MIN_VERSION "5.0.0")
set(KF5_MIN_VERSION "5.0.0")

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core DBus)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Config)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

add_executable(plasma-theme main.cpp colorscheme.cpp colorscheme.h)

target_link_libraries(plasma-theme KF5::ConfigCore Qt5::DBus)