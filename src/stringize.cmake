### read in the contents of the shader file
file(READ ${SHADER_FILE} SHADER_CONTENTS)

### convert the shader contents into cmake list
# escape quotes
string(REPLACE "\"" "\\\"" SHADER_CONTENTS "${SHADER_CONTENTS}")
# escape semicolons
string(REGEX REPLACE ";" "\\\\;" SHADER_CONTENTS "${SHADER_CONTENTS}")
# no blank lines allowed in cmake lists...so hack
string(REGEX REPLACE "^\n" "__BLANK_LINE_HACK__\n" SHADER_CONTENTS "${SHADER_CONTENTS}")
# replace newlines with semicolons
string(REGEX REPLACE "\n" ";" SHADER_CONTENTS "${SHADER_CONTENTS}")

### create variable name from the shader file name
string(REPLACE "-" "_" SHADER_VAR ${SHADER_FILE})
string(REPLACE "." "_" SHADER_VAR ${SHADER_VAR})

### write out the header
file(WRITE ${HEADER_FILE} "static const char * ${SHADER_VAR} = \n")
foreach(_line ${SHADER_CONTENTS})
	if("${_line}" STREQUAL "__BLANK_LINE_HACK__")
		file(APPEND ${HEADER_FILE} "\"\\n\"\n")
	else("${_line}" STREQUAL "__BLANK_LINE_HACK__")
		file(APPEND ${HEADER_FILE} "\"${_line}\\n\"\n")
	endif("${_line}" STREQUAL "__BLANK_LINE_HACK__")
endforeach()
file(APPEND ${HEADER_FILE} ";")

