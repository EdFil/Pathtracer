function(engine_architecture_folder_name folder_name)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits
        set(${folder_name} "x64" PARENT_SCOPE)
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4) # 32 bits
        set(${folder_name} "x86" PARENT_SCOPE)
    endif()
endfunction()

macro(engine_find_library return_lib name paths)
    engine_architecture_folder_name(ARCH)
    
    find_library(${return_lib} 
        NAMES ${name}
        PATHS ${paths}
        PATH_SUFFIXES lib/${ARCH}
        REQUIRED 
    )
endmacro()

macro(engine_find_dll return_dll name paths)
    engine_architecture_folder_name(ARCH)

    find_file(${return_dll}
        NAMES ${name}
        PATHS ${paths}
        PATH_SUFFIXES bin/${ARCH}
        REQUIRED 
    )
endmacro()
