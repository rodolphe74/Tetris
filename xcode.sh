VCPKG_HOME=/Users/rodoc/develop/vcpkg
cmake -GXcode  -B ./xcode -S . "-DCMAKE_TOOLCHAIN_FILE=$VCPKG_HOME/scripts/buildsystems/vcpkg.cmake"
