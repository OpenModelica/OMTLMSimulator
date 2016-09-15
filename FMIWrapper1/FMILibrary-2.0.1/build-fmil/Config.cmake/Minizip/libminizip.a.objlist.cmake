# Extract object files from the library
message(STATUS "Extracting object files from /home/robbr48/h/Dependencies/FMILibrary-2.0.1/build-fmil/Config.cmake/Minizip/libminizip.a")
EXECUTE_PROCESS(COMMAND /usr/bin/ar -x /home/robbr48/h/Dependencies/FMILibrary-2.0.1/build-fmil/Config.cmake/Minizip/libminizip.a                
                WORKING_DIRECTORY /home/robbr48/h/Dependencies/FMILibrary-2.0.1/build-fmil/Config.cmake/Minizip/libminizip.a.objdir)
# save the list of object files
EXECUTE_PROCESS(COMMAND ls . 
				OUTPUT_FILE /home/robbr48/h/Dependencies/FMILibrary-2.0.1/build-fmil/Config.cmake/Minizip/libminizip.a.objlist
                WORKING_DIRECTORY /home/robbr48/h/Dependencies/FMILibrary-2.0.1/build-fmil/Config.cmake/Minizip/libminizip.a.objdir)