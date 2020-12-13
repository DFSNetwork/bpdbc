project(bpdbc)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( bpdbc bpdbc bpdbc.cpp )
target_include_directories( bpdbc PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( bpdbc ${CMAKE_SOURCE_DIR}/../ricardian )