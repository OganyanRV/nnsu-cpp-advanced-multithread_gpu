get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set(LATEX_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin/")

if ( USE_LATEX )
    message( STATUS "-- " ${ProjectId} )
    file(GLOB_RECURSE report_files "*.tex")

    get_filename_component(report_names ${report_files} NAME)
    add_latex_document(${report_names} IMAGE_DIRS oganyan_r_mark_components_src)

    add_custom_command(
            TARGET ${ProjectId}_pdf
            POST_BUILD
            COMMAND rm ${LATEX_OUTPUT_PATH}/*.aux
            COMMAND rm ${LATEX_OUTPUT_PATH}/*.log
            COMMAND rm ${LATEX_OUTPUT_PATH}/*.tex
    )

else( USE_LATEX )
    message( STATUS "-- ${ProjectId} - NOT BUILD!"  )
endif( USE_LATEX )