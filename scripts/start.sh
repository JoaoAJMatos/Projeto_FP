#!/bin/sh

# Disable some warnings for CLion shellcheck plugin
# shellcheck disable=SC2039
# shellcheck disable=SC2262
# shellcheck disable=SC2181
# shellcheck disable=SC2039
# shellcheck disable=SC2039
# shellcheck disable=SC2039
# shellcheck disable=SC2039
# shellcheck disable=SC2162
# shellcheck disable=SC2028
# shellcheck disable=SC2028


install_homebrew() {
      echo "[+] Installing Homebrew"
      /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
      export PATH="/usr/local/opt/python/libexec/bin:$PATH"
}

uninstall_homebrew() {
      echo "[+] Uninstalling Homebrew"
      /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/uninstall.sh)"
}

install_python() {
      echo "[+] Installing Python"

      # Check if python is already installed
      if ! command -v python3 &> /dev/null
      then
            echo "[+] Python could not be found"
            
            # Check if homebrew is installed, if not, install it
            if ! command -v brew &> /dev/null 
            then
                  install_homebrew
            fi
            echo "[+] Installing latest Python Version"
            brew install python
      else
            echo "[+] Python is already installed\n\n"
      fi
}

create_build_folder() {
  # Check if there already is a build folder, if not, create it
  if [ ! -d "build" ]; then
      mkdir build
  fi
}

rm_previous_build() {
  # Check if there already is a build folder, if not, create it
  if [ -d "build" ]; then
      rm -rf build
  fi
}

build_single_file_project() {
      echo "[+] Starting project indexer at $(date)"
      echo "[+] Running indexer"

      rm_previous_build
      create_build_folder

      # Run indexer
      python3 ./indexer/main.py -s ../ main.c build/out.c

      # Check there are no errors
      if [ $? -eq 0 ]; then
            echo "[+] Indexer ran successfully"
      else
            echo "[ERR] Indexer failed"
            exit 1
      fi

      echo "[+] Finished indexing at $(date)"
      echo "[+] Compiling..."

      gcc -o build/out build/out.c

      # Check there are no errors
      if [ $? -eq 0 ]; then
            echo "[+] Compiled successfully"
      else
            echo "[ERR] Compilation failed"
            exit 1
      fi

      # Ask if the user wants to run the program
      read -p "Do you wish to run the compiled binary? [y/n] " -n 1 -r
      echo
      if [[ $REPLY =~ ^[Yy]$ ]]
      then
            echo "[+] Running compiled binary"
            ./build/out
      fi

      echo "[+] Done"
}

build_codeblocks_project() {
      echo "[+] Starting project indexer at $(date)"

      echo "[+] Running indexer"
      python3 ./indexer/main.py -c ../ main.c codeblocks

      # Check there are no errors
      if [ $? -eq 0 ]; then
            echo "[+] Indexer ran successfully"
      else
            echo "[ERR] Indexer failed"
            exit 1
      fi

      echo "[+] Finished indexing at $(date)"

      echo "[+] Done"
}

install_python
alias python=python3

while true; do
      echo "What do you want to do?\n"
      echo "1) Build the project for Codeblocks"
      echo "2) Build a single file project"
      echo "3) Uninstall dependencies"
      echo "4) Update the project"
      echo "5) Exit\n\n"
      read -p "Option: " choice
      case $choice in
            [1]* ) echo "[+] Building Codeblocks project"; build_codeblocks_project; break;;
            [2]* ) echo "[+] Building single file project"; build_single_file_project; break;;
            [3]* ) echo "[+] Uninstalling dependencies"; uninstall_homebrew; break;;
            [4]* ) echo "[+] Updating project"; git pull; break;;
            [5]* ) exit;;
            * ) echo "[ERR] Invalid option";;
      esac
done