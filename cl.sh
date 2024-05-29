VCPKG_HOME=/Users/rodoc/develop/vcpkg
export CMAKE_EXPORT_COMPILE_COMMANDS=1 
cmake  -B ./build -S . "-DCMAKE_TOOLCHAIN_FILE=$VCPKG_HOME/scripts/buildsystems/vcpkg.cmake"
