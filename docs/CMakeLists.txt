set(CREATE_HELP_CMD "qcollectiongenerator")
set(CREATE_HELP_ARGS "${CMAKE_CURRENT_SOURCE_DIR}/help.qhcp")
message(STATUS "Compile help with: ${CREATE_HELP_CMD} ${CREATE_HELP_ARGS}")
execute_process(
    COMMAND "${CREATE_HELP_CMD}" ${CREATE_HELP_ARGS}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    RESULT_VARIABLE CMD_ERROR
    OUTPUT_FILE CMD_OUTPUT
)
message(STATUS "Compile help error:" ${CMD_ERROR})
message(STATUS "Compile help message:" ${CMD_OUTPUT})
