################################################################################
#    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
#                                                                              #
#              This software is distributed under the terms of the             #
#              GNU Lesser General Public Licence (LGPL) version 3,             #
#                  copied verbatim in the file "LICENSE"                       #
################################################################################

include(GTestHelper)

#############################
# FairMQ Testsuites/helpers #
#############################

add_testhelper(runTestDevice
    SOURCES
    helper/runTestDevice.cxx
    helper/devices/TestPairLeft.cxx
    helper/devices/TestPairRight.cxx
    helper/devices/TestPollIn.cxx
    helper/devices/TestPollOut.cxx
    helper/devices/TestPub.cxx
    helper/devices/TestPull.cxx
    helper/devices/TestPush.cxx
    helper/devices/TestRep.cxx
    helper/devices/TestReq.cxx
    helper/devices/TestSub.cxx
    helper/devices/TestTransferTimeout.cxx

    LINKS FairMQ
)


set(MQ_CONFIG "${CMAKE_BINARY_DIR}/test/testsuite_FairMQ.IOPatterns_config.json")
set(RUN_TEST_DEVICE "${CMAKE_BINARY_DIR}/test/testhelper_runTestDevice")
set(FAIRMQ_BIN_DIR ${CMAKE_BINARY_DIR}/fairmq)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/protocols/config.json.in ${MQ_CONFIG})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/protocols/runner.cxx.in ${CMAKE_CURRENT_BINARY_DIR}/protocols/runner.cxx)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TestEnvironment.h.in ${CMAKE_CURRENT_BINARY_DIR}/TestEnvironment.h)

add_testsuite(FairMQ.Protocols
    SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/protocols/runner.cxx
    protocols/_pair.cxx
    protocols/_poller.cxx
    protocols/_pub_sub.cxx
    protocols/_push_pull.cxx
    protocols/_req_rep.cxx
    protocols/_transfer_timeout.cxx
    protocols/_push_pull_multipart.cxx

    LINKS FairMQ
    DEPENDS testhelper_runTestDevice
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/protocols
             ${CMAKE_CURRENT_BINARY_DIR}
    TIMEOUT 30
    RUN_SERIAL ON
)

add_testsuite(FairMQ.Parts
    SOURCES
    parts/runner.cxx
    parts/_iterator_interface.cxx

    LINKS FairMQ
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/parts
             ${CMAKE_CURRENT_BINARY_DIR}
    TIMEOUT 5
)

add_testsuite(FairMQ.MessageResize
    SOURCES
    message_resize/runner.cxx
    message_resize/_message_resize.cxx

    LINKS FairMQ
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/message_resize
             ${CMAKE_CURRENT_BINARY_DIR}
    TIMEOUT 5
)

add_testsuite(FairMQ.Device
    SOURCES
    device/TestSender.h
    device/TestReceiver.h
    device/TestVersion.h
    device/runner.cxx
    device/_multiple_devices.cxx
    device/_device_version.cxx

    LINKS FairMQ
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/device
             ${CMAKE_CURRENT_BINARY_DIR}
    TIMEOUT 5
    RUN_SERIAL ON
)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/helper/plugins/dummy.h.in ${CMAKE_CURRENT_BINARY_DIR}/helper/plugins/dummy.h)
add_testlib(FairMQPlugin_test_dummy
    SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/helper/plugins/dummy.h
    helper/plugins/dummy.cxx

    LINKS FairMQ
    INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/helper/plugins
    HIDDEN
    VERSION ${VERSION}
)

set(VERSION_MAJOR 2)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/helper/plugins/dummy2.h.in ${CMAKE_CURRENT_BINARY_DIR}/helper/plugins/dummy2.h)
add_testlib(FairMQPlugin_test_dummy2
    SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/helper/plugins/dummy2.h
    helper/plugins/dummy2.cxx

    LINKS FairMQ
    INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/helper/plugins
    HIDDEN
    VERSION ${VERSION}
)

add_testsuite(FairMQ.Plugins
    SOURCES
    plugins/runner.cxx
    plugins/_plugin.cxx
    plugins/_plugin_manager.cxx

    LINKS FairMQ
    INCLUDES ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS FairMQPlugin_test_dummy FairMQPlugin_test_dummy2
    TIMEOUT 10
)

add_testsuite(FairMQ.PluginsPrelinked
    SOURCES
    plugins/runner.cxx
    plugins/_plugin_manager_prelink.cxx

    LINKS FairMQ FairMQPlugin_test_dummy FairMQPlugin_test_dummy2
    INCLUDES ${CMAKE_CURRENT_BINARY_DIR}
    TIMEOUT 10
)

add_testsuite(FairMQ.PluginServices
    SOURCES
    plugin_services/runner.cxx
    plugin_services/_config.cxx
    plugin_services/_control.cxx
    plugin_services/Fixture.h

    LINKS FairMQ
    INCLUDES ${CMAKE_CURRENT_BINARY_DIR}
    TIMEOUT 10
)

add_testsuite(FairMQ.EventManager
    SOURCES
    event_manager/runner.cxx
    event_manager/_event_manager.cxx

    LINKS FairMQ
    INCLUDES ${CMAKE_CURRENT_BINARY_DIR}
    TIMEOUT 10
)

add_testsuite(FairMQ.StateMachine
    SOURCES
    state_machine/runner.cxx
    state_machine/_state_machine.cxx

    LINKS FairMQ
    INCLUDES ${CMAKE_CURRENT_BINARY_DIR}
    TIMEOUT 10
)
