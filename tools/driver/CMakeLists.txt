set(LLVM_LINK_COMPONENTS
	Support
	Core
)

add_tinylang_tool(tinylang
	Driver.cpp
)

target_link_libraries(tinylang
	PRIVATE
	tinylangBasic
)

