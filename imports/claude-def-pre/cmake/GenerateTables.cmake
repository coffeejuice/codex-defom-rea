# Converts the keyword registry TSV and the keyword layout table into C++
# include files. Run in script mode (cmake -P).

function(dk_escape out str)
  string(REPLACE "\\" "\\\\" str "${str}")
  string(REPLACE "\"" "\\\"" str "${str}")
  set(${out} "${str}" PARENT_SCOPE)
endfunction()

file(MAKE_DIRECTORY ${OUT_DIR})

# keyword_registry.tsv: id name group_code group flag handler1 handler2 handler3
file(STRINGS ${REGISTRY} reg_lines)
set(kw "// Generated from docs/re/keyword_registry.tsv. Do not edit.\n")
set(first TRUE)
foreach(line IN LISTS reg_lines)
  if(first)
    set(first FALSE)
    continue()
  endif()
  string(REPLACE "\t" ";" cols "${line}")
  list(LENGTH cols n)
  if(n LESS 5)
    continue()
  endif()
  list(GET cols 0 id)
  list(GET cols 1 name)
  list(GET cols 2 group)
  list(GET cols 4 flag)
  string(APPEND kw "{${id}, \"${name}\", ${group}, ${flag}},\n")
endforeach()
file(WRITE ${OUT_DIR}/keyword_table.inc.tmp "${kw}")
file(COPY_FILE ${OUT_DIR}/keyword_table.inc.tmp ${OUT_DIR}/keyword_table.inc ONLY_IF_DIFFERENT)

# keyword_layouts.txt: kept verbatim, one string per line.
file(STRINGS ${LAYOUTS} lay_lines ENCODING UTF-8)
set(lay "// Generated from data/keyword_layouts.txt. Do not edit.\n")
foreach(line IN LISTS lay_lines)
  dk_escape(esc "${line}")
  string(APPEND lay "\"${esc}\",\n")
endforeach()
file(WRITE ${OUT_DIR}/layout_table.inc.tmp "${lay}")
file(COPY_FILE ${OUT_DIR}/layout_table.inc.tmp ${OUT_DIR}/layout_table.inc ONLY_IF_DIFFERENT)
