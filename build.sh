export LLVM_DIR=../../llvm-12.0.1/lib/cmake/llvm
cd build && cmake -DCMAKE_BUILD_TYPE=debug -DCMAKE_CXX_STANDARD=14 ../ && cmake --build . --verbose && cd -
ln -sf build/tools/driver/tinylang tinylang.exe
