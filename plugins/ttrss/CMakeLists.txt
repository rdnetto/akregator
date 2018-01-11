include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}
    ${akregator_SOURCE_DIR}/interfaces
    ${akregator_BINARY_DIR}
    )

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g " )

########### next target ###############

set(connection_details_SRCS connection_details.cpp)
ki18n_wrap_ui(connection_details_SRCS ui/connection_details.ui)

set(akregator_ttrss_plugin_PART_SRCS
    ${connection_details_SRCS}
    connection_details.cpp
    feedstoragettrssimpl.cpp
    storagettrssimpl.cpp
    storagefactoryttrssimpl.cpp
    ttrssplugin.cpp
    )

add_library(akregator_ttrss_plugin MODULE ${akregator_ttrss_plugin_PART_SRCS})

set_target_properties(akregator_ttrss_plugin PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(akregator_ttrss_plugin
    KF5::Syndication
    akregatorinterfaces
    KF5::I18n
    KF5::CoreAddons
    )

install(TARGETS akregator_ttrss_plugin DESTINATION ${KDE_INSTALL_PLUGINDIR})

########### install files ###############

install(FILES akregator_ttrss_plugin.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
