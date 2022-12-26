from util import *
import os

import xml.etree.ElementTree as ET  # Used for writting the XML file for the CodeBlocks project
import xml.dom.minidom              # Used for formatting the XML file for the CodeBlocks project
import shutil                       # Used for copying the files to the CodeBlocks project directory


# ProjectContext
# 
# This class is used to store the context of a project.
# It's where all files are indexed and stored.
# The ProjectContext can then be used to write the output file.
# It contains information about the order which the headers should be included in the output file, 
# as well as many other useful information.
class ProjectContext:
    # Constructor
    #
    # @param path:          The path to the project directory
    # @param entry_point:   The entry point of the project (where the main function is)
    # @param output_file:   The output file to write the indexed project to
    def __init__(self, path, entry_point, output_file):
        self.path = relative_to_absolute_path(path)
        self.entry_point = self.path + DIR_SEPARATOR + entry_point
        self.output_file = output_file

        # Arrays of all the .c and .h files in the project
        self.c_files = []
        self.h_files = []

        self.indexed_files = []         # Files that have already been indexed
        self.indexed_includes = []      # All the include statements that have been indexed
        self.third_party_includes = []  # All the third party include statements that have been indexed

        self.preprocessor_directives = []  # All the preprocessor directives in all the files

        self.headers_output = []  # The content of all the header files
        self.c_output = []        # The content of all the c files

    def index_project(self):
        # Index the entry point first
        self.index_file(self.entry_point)

        # Get all the other file paths
        self.c_files, self.h_files = fetch_paths(self.path)

        # Index all the other files
        for file in self.c_files:  # C files
            if file != self.entry_point:
                self.index_file(file)

        for file in self.h_files:  # H files
            if file != self.entry_point:
                self.index_file(file)

    # Index a file
    def index_file(self, file):
        if file not in self.indexed_files:
            self.indexed_files.append(file)

            # Load the file
            lines = load_file(file)
            remove_comments(lines)

            # Get all the preprocessor directives to extract the local and std includes
            preprocessor_directives = get_top_preprocessor_directives(lines)
            local_includes = get_all_local_includes(preprocessor_directives)

            # Get the preprocessors directives without the include statements
            # Copy each element of the list into self.indexed_includes
            self.indexed_includes.extend(get_preprocessor_directives_without_includes(preprocessor_directives))

            # Index the needed includes:
            # All files that are included and are not already indexed must be indexed
            # This is needed for example if we include files that are outside the project directory
            needed_includes = get_all_needed_includes(local_includes, self.h_files)
            for include in needed_includes:
                self.index_file(include)

            # Add the content of the header file to the headers_output list
            # The content of the header file must not contain the header guards, nor the include statements
            if file.endswith(".h"):
                self.headers_output.append(get_header_file_content(file))
            elif file.endswith(".c"):
                self.c_output.append(get_c_file_content(file))

    def get_project_size(self):
        size = 0
        for file in self.indexed_files:
            size += os.path.getsize(file)
        return size

    # Writes the indexed project to the output file
    def write_output_file(self):
        # delete the output file if it already exists
        if os.path.exists(self.output_file):
            os.remove(self.output_file)

        with open(self.output_file, "w") as output_file:
            # Write the include statements first
            if len(self.indexed_includes) > 0:
                for line in self.indexed_includes:
                    output_file.write(line)

                output_file.write("\n\n")

            # Write the rest of the top-level preprocessor directives
            if len(self.preprocessor_directives) > 0:
                for line in self.preprocessor_directives:
                    output_file.write(line)

                output_file.write("\n\n")

            # Write the contents of all the header files
            for header in self.headers_output:
                for line in header:
                    output_file.write(line)

            output_file.write("\n\n")

            # Write the contents of all the c files
            for c_file in self.c_output:
                for line in c_file:
                    output_file.write(line)

    # Constructs a compiler command to be executed in order to correctly compile the indexed project
    # It takes into consideration all the third party libraries that should be linked to the executable
    def build_compiler_command(self):
        default_compiler = get_default_c_compiler()                                         # Get default compiler
        executable_extension = get_executable_extension()                                   # Get the extension for the executable
        executable_filename = get_filename_without_extension(self.output_file)              # Get the filename of the output file without the extension
        library_linker_flags = build_linker_flags_for_libraries(self.third_party_includes)  # Get the linker flags for the third party libraries

        # Build the compiler command
        compiler_command = default_compiler + " " + self.output_file + " -o " + executable_filename + executable_extension

        # Append the linker flags to the compiler command
        if len(library_linker_flags) > 0:
            compiler_command += " " + library_linker_flags

        return compiler_command


# This class is used to convert our multi file CMake project into a Codeblocks project
class CodeBlocksContextCreator:
    def __init__(self, path, entry_point, output_project_name):
        self.path = path
        self.entry_point = entry_point
        self.output_project_name = output_project_name

        self.c_files = []
        self.h_files = []

        self.output = []

    def index_project(self):
        self.c_files, self.h_files = fetch_paths(self.path)

    # Since CodeBlocks projects don't support actual folders, we need to refactor the file paths
    # in the #include statements to match the new file paths.
    # In codeblocks, all the files are in the root directory of the project. Although they can be
    # in virtual folders inside the IDE.
    def refactor_includes(self):
        pass

    def create_codeblocks_project_config(self):
        xml_config = ET.Element("CodeBlocks_project_file")

        xml_file_version = ET.SubElement(xml_config, "FileVersion")
        xml_file_version.set("major", "1")
        xml_file_version.set("minor", "6")

        # Create the project node
        xml_project = ET.SubElement(xml_config, "Project")

        xml_option = ET.SubElement(xml_project, "Option")
        xml_option.set("title", self.output_project_name)
        xml_option = ET.SubElement(xml_project, "Option")
        xml_option.set("pch_mode", "2")
        xml_option = ET.SubElement(xml_project, "Option")
        xml_option.set("compiler", get_default_c_compiler())

        xml_build = ET.SubElement(xml_project, "Build")
        xml_target = ET.SubElement(xml_build, "Target")
        xml_target.set("title", "Debug")

        xml_target_option = ET.SubElement(xml_target, "Option")
        xml_target_option.set("output", "bin/Debug/" + self.output_project_name)
        xml_target_option.set("prefix_auto", "1")
        xml_target_option.set("extension_auto", "1")

        xml_target_option2 = ET.SubElement(xml_target, "Option")
        xml_target_option2.set("object_output", "obj/Debug/")
        xml_target_option2.set("type", "1")
        xml_target_option2.set("compiler", get_default_c_compiler())

        xml_target_compiler = ET.SubElement(xml_target, "Compiler")
        xml_target_add = ET.SubElement(xml_target_compiler, "Add")
        xml_target_add.set("option", "-g")

        xml_target_release = ET.SubElement(xml_build, "Target")
        xml_target_release.set("title", "Release")

        xml_target_option_release = ET.SubElement(xml_target_release, "Option")
        xml_target_option_release.set("output", "bin/Release/" + self.output_project_name)
        xml_target_option_release.set("prefix_auto", "1")
        xml_target_option_release.set("extension_auto", "1")

        xml_target_option2_release = ET.SubElement(xml_target_release, "Option")
        xml_target_option2_release.set("object_output", "obj/Release/")

        xml_target_option2_release = ET.SubElement(xml_target_release, "Option")
        xml_target_option2_release.set("type", "1")

        xml_target_option2_release = ET.SubElement(xml_target_release, "Option")
        xml_target_option2_release.set("compiler", get_default_c_compiler())

        xml_target_compiler_release = ET.SubElement(xml_target_release, "Compiler")
        xml_target_add_release = ET.SubElement(xml_target_compiler_release, "Add")
        xml_target_add_release.set("option", "-O2")

        xml_target_linker = ET.SubElement(xml_target_release, "Linker")
        xml_target_add_linker = ET.SubElement(xml_target_linker, "Add")
        xml_target_add_linker.set("option", "-s")

        xml_compiler = ET.SubElement(xml_project, "Compiler")
        xml_add = ET.SubElement(xml_compiler, "Add")
        xml_add.set("option", "-Wall")

        # Add the files to the project
        xml_unit = ET.SubElement(xml_project, "Unit")
        xml_unit.set("filename", self.entry_point)  # Add the entry point first

        for file in self.c_files:
            xml_unit = ET.SubElement(xml_project, "Unit")
            xml_unit.set("filename", get_filename_from_path(file))
            xml_option = ET.SubElement(xml_unit, "Option")
            xml_option.set("compilerVar", "CC")

        for file in self.h_files:
            xml_unit = ET.SubElement(xml_project, "Unit")
            xml_unit.set("filename", get_filename_from_path(file))
            xml_option = ET.SubElement(xml_unit, "Option")
            xml_option.set("compilerVar", "CC")

        xml_option = ET.SubElement(xml_unit, "Option")
        xml_option.set("compilerVar", "CC")

        xml_extensions = ET.SubElement(xml_project, "Extensions")
        xml_envvars = ET.SubElement(xml_extensions, "envvars")
        xml_code_completion = ET.SubElement(xml_extensions, "code_completion")
        xml_lib_finder = ET.SubElement(xml_extensions, "lib_finder")
        xml_lib_finder.set("disable_auto", "1")
        xml_debugger = ET.SubElement(xml_extensions, "debugger")

        # Assemble the xml tree
        self.output = ET.tostring(xml_config, encoding="unicode", method="xml")

        # Pretty print the output
        self.output = xml.dom.minidom.parseString(self.output).toprettyxml(indent="   ")
        print(self.output)

    def copy_files_to_project(self):
        for file in self.c_files:
            shutil.copy(file, self.output_project_name)
        for file in self.h_files:
            shutil.copy(file, self.output_project_name)

    def build_codeblocks_project(self):
        # Create the output directory if it does not exist
        if not os.path.exists(self.output_project_name):
            os.makedirs(self.output_project_name)

        # Create the .cbp file
        self.create_codeblocks_project_config()

        # Copy the files to the output directory
        self.copy_files_to_project()

        # Write the .cbp file to the output directory
        with open(self.output_project_name + "/" + self.output_project_name + ".cbp", "w") as output_file:
            output_file.write(self.output)
