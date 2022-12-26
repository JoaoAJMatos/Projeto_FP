# This file contains a set of utilities used to index the project

import os

DIR_SEPARATOR = ""
if os.name == "nt":
    DIR_SEPARATOR = "\\"
else:
    DIR_SEPARATOR = "/"

# Some headers recognized by C
C_STD_LIB_RECOGNIZED_HEADERS = ("assert.h", "complex.h", "ctype.h",
                                "errno.h", "fenv.h", "float.h",
                                "inttypes.h", "iso646.h", "limits.h",
                                "locale.h", "math.h", "setjmp.h",
                                "signal.h", "stdalign.h", "stdarg.h",
                                "stdatomic.h", "stdbit.h", "stdckdint.h",
                                "stdbool.h", "stddef.h", "stdint.h",
                                "stdio.h", "stdlib.h", "stdnoreturn.h",
                                "string.h", "tgmath.h", "threads.h",
                                "time.h", "uchar.h", "wchar.h", "wctype.h")

# Some header files recognized by Linux/MacOS
POSIX_STD_LIB_RECOGNIZED_HEADERS = ("netdb.h", "netinet/in.h", "arpa/inet.h",
                                    "sys/socket.h", "sys/types.h", "sys/stat.h",
                                    "fcntl.h", "unistd.h", "sys/wait.h",
                                    "sys/un.h", "sys/time.h", "sys/resource.h",
                                    "sys/sem.h", "sys/shm.h", "sys/msg.h",
                                    "sys/ipc.h", "sys/mman.h", "sys/select.h",
                                    "sys/epoll.h", "sys/timerfd.h", "sys/signalfd.h",
                                    "sys/eventfd.h", "sys/inotify.h", "sys/ioctl.h",
                                    "sys/uio.h", "sys/syscall.h", "sys/sysinfo.h",
                                    "sys/utsname.h", "sys/ptrace.h", "sys/personality.h",
                                    "sys/prctl.h", "sys/vfs.h", "sys/quota.h",
                                    "sys/mount.h", "sys/reboot.h", "sys/swap.h",
                                    "sys/user.h", "sys/auxv.h")

# Some header files recognized by Windows
WIN_STD_LIB_RECOGNIZED_HEADERS = ("windows.h", "winsock2.h", "ws2tcpip.h",
                                  "winbase.h", "wincon.h", "winuser.h",
                                  "wingdi.h", "winnt.h", "winnls.h",
                                  "winerror.h", "winreg.h", "winperf.h",
                                  "winnetwk.h", "winldap.h", "wininet.h",
                                  "winhttp.h", "winsvc.h", "winsock.h",
                                  "winspool.h", "winresrc.h")


# Get the default C compiler and check if it is installed
def get_default_c_compiler():
    # Get the default C compiler
    default_compiler = os.environ.get("CC")
    if default_compiler is None:
        default_compiler = "gcc"

    # Check if the default compiler is installed
    if not os.system(default_compiler + " --version") == 0:
        print("Error: No C compiler installed")
        exit(1)

    return default_compiler


# Get the current working directory
def get_cwd():
    return os.getcwd()


# Converts a relative path to an absolute path
def relative_to_absolute_path(relative_path):
    return os.path.abspath(relative_path)


# Fetches all the .c and .h files recursively from the given root directory
# using the os.walk function
def fetch_paths(path):
    c_files = []
    h_files = []
    for root, dirs, files in os.walk(path):
        # Ignore files in the cmake-build-debug directory
        if "cmake-build-debug" in root:
            continue

        # Ignore files in the CMakeFiles directory
        if "CMakeFiles" in root:
            continue

        for file in files:  # Add all the .c and .h files to the output arrays
            if file.endswith(".c"):
                c_files.append(os.path.join(root, file))
            elif file.endswith(".h"):
                h_files.append(os.path.join(root, file))

    return c_files, h_files


# Loads the file into a list of lines
def load_file(path):
    with open(path, "r") as f:
        return f.readlines()


# Get all the preprocessor directives from a list of lines
# This function is used to extract the header guards and the include statements
# from the indexed files.
# The header guards must be removed, and all the pre processor directives must be copied to the top
# of the output file.
def get_preprocessor_directives(lines):
    directives = []
    for line in lines:
        if line.startswith("#"):  # Preprocessor directives start with "#"
            directives.append(line)

    return directives


# Get all the preprocessor directives before the first line of code
# This is needed because preprocessor directives that are not on the top of the original file,
# should be left as is. (for example, for the initialization of Winsock in projects intended
# to be compiled regardless of the OS, or any platform specific code)
def get_top_preprocessor_directives(lines):
    directives = []
    for line in lines:
        if line.startswith("#"):
            directives.append(line)
        else:
            break

    return directives


# Get all the local includes from a list of lines
# Local includes are denoted by #include "file.h" (with quotes)
def get_all_local_includes(preprocessor_directives):
    includes = []
    for directive in preprocessor_directives:
        # If the preprocessor directive is an include and it is a local include
        if directive.startswith("#include") and "\"" in directive:
            includes.append(directive)

    return includes


# Get all the std includes from a list of preprocessor directives
# Std includes are denoted by #include <file.h> (with angle brackets)
def get_std_includes(preprocessor_directives):
    std_includes = []
    for directive in preprocessor_directives:
        if directive.startswith("#include") and "<" in directive:
            std_includes.append(directive)

    return std_includes


# Returns the includes that are in the C standard library (stdlib.h, stdio.h, etc.)
# This is needed because we might link third party libraries that are not in the standard library
# These libraries must be linked when compiling the output C file.
def get_includes_from_stdlib(includes):
    stdlib_includes = []

    for include in includes:
        library = get_library_name_from_include(include)  # Get the library name from the include statement

        if library in C_STD_LIB_RECOGNIZED_HEADERS:  # If the library is in the C standard library
            stdlib_includes.append(include)
        elif library in WIN_STD_LIB_RECOGNIZED_HEADERS:  # If the library is in the Windows standard library
            stdlib_includes.append(include)
        elif library in POSIX_STD_LIB_RECOGNIZED_HEADERS:  # If the library is in the POSIX standard library
            stdlib_includes.append(include)

    return stdlib_includes


# Does the same as the above, except it returns the includes that are not in the standard library
def get_includes_outside_stdlib(includes):
    stdlib_includes = get_includes_from_stdlib(includes)

    return [include for include in includes if include not in stdlib_includes]


# Checks if the project contains a CMakeLists.txt file
# If it does, the process of indexing third party libraries is simplified
def project_has_cmake(root_dir):
    # Check if there is a CMakeLists.txt file in the root directory of the project
    if os.path.exists(root_dir + DIR_SEPARATOR + "CMakeLists.txt"):
        return True


# TODO: Make this function work
# If a CMakeLists.txt file is found, we can easily look for the third party libraries
# by looking for the "target_link_libraries" command or even the "link_libraries" command
def get_cmake_terget_linked_libraries(root_dir):
    cmake_file = root_dir + DIR_SEPARATOR + "CMakeLists.txt"
    lines = load_file(cmake_file)
    project_target = get_cmake_project_target(lines)

    libraries = []
    for line in lines:
        if "target_link_libraries" in line:
            pass  # Implement this

    return libraries


# Returns the project target name from a CMakeLists.txt file
# project(name [language])
def get_cmake_project_target(cmake_lines):
    for line in cmake_lines:
        if "project" in line:
            return line.split("project")[1].split(" ")[1].strip()


# Returns the name of the library in a given include statement
# (The text between the angle brackets)
def get_library_name_from_include(include):
    return include.split("<")[1].split(">")[0]


# Does the same as the above, except for a list of includes
def get_library_names_from_includes(includes):
    library_names = []
    for include in includes:
        library_names.append(get_library_name_from_include(include))

    return library_names


# Removes the header guards from a header file
# The header guards should be the first two preprocessor directives (#ifndef, #define), and the last one (#endif)
# Or in case of more modern compilers: (#pragma once)
# We should add support for both header guard types. If no "standard" header guards are found,
# we should look for #pragma once
def rm_header_guards(lines):
    # Array of lines that should be deleted in the file
    to_delete = []
    # Get all the preprocessor directives
    preprocessor_directives = get_preprocessor_directives(lines)

    if len(preprocessor_directives) >= 3:
        # Standard header guards
        if preprocessor_directives[0].startswith("#ifndef") and preprocessor_directives[1].startswith("#define") and \
                preprocessor_directives[-1].startswith("#endif"):
            to_delete.append(preprocessor_directives[0])
            to_delete.append(preprocessor_directives[1])
            to_delete.append(preprocessor_directives[-1])

    elif len(preprocessor_directives) >= 1:
        # Modern header guards
        if preprocessor_directives[0].startswith("#pragma once"):
            to_delete.append(preprocessor_directives[0])

    # Remove the header guards from the lines array
    if len(to_delete) > 0:
        for line in to_delete:
            lines.remove(line)

    return lines  # Return the lines array without the header guards


# Get needed includes
# Checks all the include statements in the file. If there are local includes that do not
# exist in the h_files list, they are added to the needed_includes list.
# This is needed because we need to make sure all files needed in the project are indexed,
# even if they are outside the project directory
def get_all_needed_includes(includes, h_files):
    needed_includes = []
    for include in includes:
        # Get the file path from the include statement
        file_name = include.split("\"")[1]
        abs_path = relative_to_absolute_path(file_name)  # Convert the relative path to an absolute path

        # Check if the file exists in the h_files list
        for file in h_files:
            if abs_path in file:
                needed_includes.append(file)

    return needed_includes


# Print an array of lines
def cat(lines):
    for line in lines:
        print(line, end="")


# Get the header file content.
# Returns the content of a header file without the header guards and the include statements
def get_header_file_content(file):
    lines = load_file(file)
    lines = rm_header_guards(lines)
    preprocessor_directives = get_preprocessor_directives(lines)
    includes = get_all_local_includes(preprocessor_directives)
    for include in includes:
        lines.remove(include)

    return lines


# Get the contents of a source file
def get_c_file_content(file):
    lines = load_file(file)
    preprocessor_directives = get_preprocessor_directives(lines)
    includes = get_all_local_includes(preprocessor_directives)
    for include in includes:
        lines.remove(include)

    return lines


# Gets all the prepocessor directives without the include statements
def get_preprocessor_directives_without_includes(lines):
    preprocessor_directives = get_preprocessor_directives(lines)
    includes = get_all_local_includes(preprocessor_directives)
    for include in includes:
        lines.remove(include)

    return lines


# This function deletes the comments from a file
# since comments are not needed in the output file
# WARN: This function does not delete all the comments, it only deletes
# lines that start with "//" or "/*", or the contents of a multiline comment
# Therefore, comments proceeded by code will not be deleted:
# Example:
#     
#     int a = 5; // This is a comment
# 
# The comment will not be deleted
def remove_comments(lines):
    to_delete = []

    for line in lines:
        if line.startswith("//"):
            to_delete.append(line)

        elif line.startswith("/*"):
            to_delete.append(line)
            for line in lines:
                if line.endswith("*/"):
                    to_delete.append(line)
                    break
                else:
                    to_delete.append(line)

    for line in to_delete:
        if line in lines:
            lines.remove(line)

    return lines


def delete_line_comments(lines):
    pass


def get_filename_from_path(path):
    return path.split("/")[-1]


def get_filename_without_extension(path):
    return get_filename_from_path(path).split(".")[0]


# Returns the extension of an executable file for the current OS
def get_executable_extension():
    if os.name == "nt":
        return ".exe"  # Windows
    else:
        return ""


def build_linker_flag_for_library(lib):
    return " -l" + lib


def build_linker_flags_for_libraries(libraries):
    flags = ""
    for lib in libraries:
        flags += build_linker_flag_for_library(lib)

    return flags
