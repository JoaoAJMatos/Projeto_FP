# This script is used to convert the multi-file project into a single C file because of compatibility
# issues with the copyright analysis tools my university uses. 
# 
# The script recursively searches a given root directory for all .c and .h files and appends them to a single output
# C file. This process takes into consideration the #include statements in the files. For example, if a file includes
# another file, the other file should be added first to the output file.
# 
# In short, it generates a compilable C file from a multi-file project.

import context
import sys


def menu():
    while True:
        print("1. Build Single C File Project")
        print("2. Build CodeBlocks Project")
        print("3. Update Project")
        print("4. Exit")

        choice = input("Option: ")
        if choice in ["1", "2", "3", "4"]:
            return int(choice)


def show_project_info(proj_context):
    print("====================== PROJECT INFO ======================")
    print("Project path: " + proj_context.path)
    print("Entry point: " + proj_context.entry_point)
    print("Output file: " + proj_context.output_file)
    print("Total indexed files: " + str(len(proj_context.indexed_files)))
    print("Project size: " + str(proj_context.get_project_size()) + " bytes")
    print("C files: ")
    show_indexed_files(proj_context.c_files)
    print("Header files: ")
    show_indexed_files(proj_context.h_files)


def show_indexed_files(file_list):
    for file in file_list:
        print("     - " + file)


def build_single_file_project():
    print("Building project context for single file project...")
    ctx = context.ProjectContext(sys.argv[2], sys.argv[3], sys.argv[4])
    ctx.index_project()
    show_project_info(ctx)
    ctx.write_output_file()


def build_codeblocks_project():
    print("Indexing project for CodeBlocks...")
    ctx = context.CodeBlocksContextCreator(sys.argv[2], sys.argv[3], sys.argv[4])
    ctx.index_project()
    ctx.build_codeblocks_project()


if __name__ == "__main__":
    # Check if the arguments are valid
    if len(sys.argv) != 5:
        print("Usage: python main.py <option> <path> <entry_point> <output_file>")
        exit(1)

    if sys.argv[1] == "-s":
        build_single_file_project()

    elif sys.argv[1] == "-c":
        build_codeblocks_project()

    elif sys.argv[1] == "-m":
        choice = menu()

        if choice == 1:
            build_single_file_project()
        elif choice == 2:
            build_codeblocks_project()
        elif choice == 3:
            print("Updating project...")


    else:
        print("Invalid option " + sys.argv[1])
