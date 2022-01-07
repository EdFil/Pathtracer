$REPO_PATH = (Join-Path $PSScriptRoot ".repo")
$BUILD_PATH = (Join-Path $PSScriptRoot ".build")
$INSTALL_PATH = $PSScriptRoot

if (!(Test-Path $REPO_PATH)) {
    git clone -b v1.1.0 --depth 1 https://github.com/Tencent/rapidjson.git $REPO_PATH
}

if (!(Test-Path $BUILD_PATH)) {
    New-Item $BUILD_PATH -Type Directory
}

cmake -B $BUILD_PATH $REPO_PATH -A x64 -DCMAKE_INSTALL_PREFIX="$INSTALL_PATH" `
    -DCMAKE_BUILD_TYPE=Release `
    -DRAPIDJSON_BUILD_DOC=FALSE `
    -DRAPIDJSON_BUILD_EXAMPLES=FALSE `
    -DRAPIDJSON_BUILD_TESTS=FALSE `
    -DRAPIDJSON_BUILD_CXX17=TRUE
cmake --build $BUILD_PATH 
cmake --install $BUILD_PATH
