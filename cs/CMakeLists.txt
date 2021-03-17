# Included in ../CmakeLists.txt if csharp build is selected.

find_package(SWIG 3.0 REQUIRED)

message("Generating SWIG C# files...")

# Cleaning
execute_process(COMMAND powershell "Remove-Item ${CMAKE_SOURCE_DIR}/cs/src/* -recurse -exclude .gitignore")

# SWIG execution
set ( EXECUTE_COMMAND "${SWIG_EXECUTABLE} -v -c++ -csharp -dllimport ${LIBRARY_NAME}.dll -namespace F2iConsulting.Fetpapi -o ${CMAKE_SOURCE_DIR}/swig/swigGeneratedCsWrapper.cpp -outdir ${CMAKE_SOURCE_DIR}/cs/src ${CMAKE_SOURCE_DIR}/swig/swigEtp1_2Include.i")
execute_process(COMMAND powershell  ${EXECUTE_COMMAND})
message("SWIG C# files have been generated.")

#Add the swig cpp file to the FesapiCpp
target_sources(${LIBRARY_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/swig/swigGeneratedCsWrapper.cpp)

# Create the Visual Studio project from a template file
set (CS_LIBRARY_NAME FetpapiCs)

#file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/cs/src" DOS_STYLE_SOURCE_DIR)
configure_file(${CMAKE_SOURCE_DIR}/cmake/fetpapiCs.csproj.template ${CMAKE_SOURCE_DIR}/cs/fetpapiCs.csproj) # Overwrite if different
configure_file(${CMAKE_SOURCE_DIR}/cmake/AssemblyInfo.cs ${CMAKE_SOURCE_DIR}/cs/Properties/AssemblyInfo.cs) # Overwrite if different
