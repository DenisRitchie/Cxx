
function(MessageLog LOG)
    string(REPEAT "─" 100 LINE)
    message(STATUS ${LINE})
    list(APPEND CMAKE_MESSAGE_INDENT "\t")

    message(STATUS ${LOG})

    list(POP_BACK  CMAKE_MESSAGE_INDENT)
    message(STATUS ${LINE})
endfunction(MessageLog)

