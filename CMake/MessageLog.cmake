
function(MessageLog LOG)
    string(REPEAT "─" 100 LINE)
    message(STATUS ${LINE})
    list(APPEND CMAKE_MESSAGE_INDENT "\t")

    message(STATUS ${LOG})

    list(POP_BACK  CMAKE_MESSAGE_INDENT)
    message(STATUS ${LINE})
endfunction(MessageLog)

function(MessageLogArray LOG)
    string(REPEAT "─" 100 LINE)
    message(STATUS ${LINE})
    list(APPEND CMAKE_MESSAGE_INDENT "\t")

    set(index 0)
    foreach(item IN ITEMS ${LOG})
        message(STATUS "[${index}]: ${item}")
        math(EXPR index "${index} + 1" OUTPUT_FORMAT DECIMAL)
    endforeach()

    list(POP_BACK  CMAKE_MESSAGE_INDENT)
    message(STATUS ${LINE})
endfunction(MessageLogArray)

