MESSAGE("Build setup for BlackMagic DeckLink plugin...")

omni_option(
  Blackmagic_PATH "Path to BlackMagic Decklink SDK" 
    ${CMAKE_SOURCE_DIR}/../Blackmagic_DeckLink_SDK
)

if (NOT EXISTS ${Blackmagic_PATH} OR NOT IS_DIRECTORY ${Blackmagic_PATH})
  SET(plugin_input_Blackmagic_IGNORE TRUE)
  return()
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(Blackmagic_SDK_PATH ${Blackmagic_PATH}/Linux)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Mac") 
  set(Blackmagic_SDK_PATH ${Blackmagic_PATH}/Mac)
endif()

include_directories(${Blackmagic_SDK_PATH}/include)

set(plugin_input_Blackmagic_SOURCES ${Blackmagic_SDK_PATH}/include/DeckLinkAPIDispatch.cpp)
set(plugin_input_Blackmagic_HEADERS_MOC_PATH ${plugin_input_Blackmagic_PATH})

