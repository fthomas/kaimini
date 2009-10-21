find_path(Minuit2_INCLUDE_DIRS Minuit2/FCNBase.h PATH_SUFFIXES root)

find_library(Minuit2_LIBRARIES Minuit2 PATH_SUFFIXES
    root/5.08 root/5.10 root/5.12 root/5.14 root/5.16
    root/5.18 root/5.20 root/5.21 root/5.22 root/5.24)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Minuit2 DEFAULT_MSG
    Minuit2_INCLUDE_DIRS Minuit2_LIBRARIES)

get_filename_component(Minuit2_LIBRARY_DIRS ${Minuit2_LIBRARIES} PATH)

mark_as_advanced(
    Minuit2_INCLUDE_DIRS
    Minuit2_LIBRARY_DIRS
    Minuit2_LIBRARIES)
