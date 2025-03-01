include(GoogleTest)

#..:: ApplicationTest ::..
add_executable(ApplicationTest 
 ${CMAKE_CURRENT_SOURCE_DIR}/application/test_main.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/application/useCases/test_StartServerUseCase.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/application/dto/test_StartServerDTO.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/application/serviceLocator/test_LoggerServiceLocator.cpp
)
target_link_libraries(ApplicationTest testftirc gtest_main)
target_include_directories(ApplicationTest PRIVATE 
  ${CMAKE_SOURCE_DIR}/pkg
  ${gtest_SOURCE_DIR}
)

# ..:: DomainTest ::..
add_executable(DomainTest 
 ${CMAKE_CURRENT_SOURCE_DIR}/domain/test_main.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/domain/channel/test_Channel.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/domain/channel/test_ChannelClientList.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/domain/channel/test_ChannelTopic.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/domain/client/test_Password.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/domain/message/test_Message.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/domain/message/test_MessageParser.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/domain/message/test_MessageParserPrefix.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/domain/shared/test_JenkinsHash.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/domain/shared/test_RFCChannelHash.cpp
)

target_link_libraries(DomainTest testftirc gtest_main)
target_include_directories(DomainTest PRIVATE 
  ${CMAKE_SOURCE_DIR}/pkg
  ${gtest_SOURCE_DIR}
)

#..:: InfrastructureTest ::..
add_executable(InfrastructureTest 
 ${CMAKE_CURRENT_SOURCE_DIR}/infra/test_main.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/infra/database/test_InmemoryChannelDatabase.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/infra/database/test_InmemoryClientDatabase.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/infra/logger/test_ConsoleLogger.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/infra/logger/test_FileLogger.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/infra/logger/test_MultiLogger.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/infra/socket/test_SocketHandler.cpp
)
target_link_libraries(InfrastructureTest testftirc gtest_main gmock_main)
target_include_directories(InfrastructureTest PRIVATE 
  ${CMAKE_SOURCE_DIR}/pkg
  ${gtest_SOURCE_DIR}
)

# ..:: PresentationTest ::..
# add_executable(PresentationTest 
#  ${CMAKE_CURRENT_SOURCE_DIR}/presentation/test_main.cpp
#  ${CMAKE_CURRENT_SOURCE_DIR}/presentation/test_entrypoint.cpp
# )
# target_link_libraries(PresentationTest testftirc gtest_main gmock_main)
# target_include_directories(PresentationTest PRIVATE 
#   ${CMAKE_SOURCE_DIR}/pkg
#   ${gtest_SOURCE_DIR}
# )

# ..:: GoogleTest ::..
gtest_add_tests(TARGET DomainTest)
gtest_add_tests(TARGET ApplicationTest)
gtest_add_tests(TARGET InfrastructureTest)
# gtest_add_tests(TARGET PresentationTest)