echo "Starting project indexer at %DATE% %TIME%"

function install_chocolatey() {
  echo "[+] Installing Chocolatey Package Manager"
  powershell -NoProfile -ExecutionPolicy Bypass -Command "iex ((new-object net.webclient).DownloadString('https://chocolatey.org/install.ps1'))" && SET "PATH=%PATH%;%ALLUSERSPROFILE%\chocolatey\bin"
}

function uninstall_chocolatey() {
  echo "[+] Uninstalling Chocolatey Package Manager"
    powershell -NoProfile -ExecutionPolicy Bypass -Command "iex ((new-object net.webclient).DownloadString('https://chocolatey.org/install.ps1'))" && SET "PATH=%PATH%;%ALLUSERSPROFILE%\chocolatey\bin"
}

function install_python() {
    echo "Installing Python 3.7.3"
    choco install python --version 3.7.3
}

function build_codeblocks_project() {
    echo "Starting project indexer at %DATE% %TIME%"
    echo "Running indexer"

    # Run indexer
    python ./indexer/main.py -c ../ main.c codeblocks

    # Check there are no errors
    if [ $? -eq 0 ]; then
        echo "Indexer completed successfully"
    else
        echo "Indexer failed"
        exit 1
    fi
}

if [ ! -d "C:\ProgramData\chocolatey" ]; then
    install_chocolatey
fi

# Check if Python is installed
if [ ! -d "C:\Python37" ]; then
    install_python
fi

while true; do
    echo "What do you want to do?\n"
          echo "1) Build the project for Codeblocks"
          echo "2) Build a single file project"
          echo "3) Uninstall dependencies"
          echo "4) Update the project"
          echo "5) Exit\n\n"
          read -p "Option: " choice
          case $choice in
                1 ) echo "Building project for Codeblocks"
                    build_codeblocks_project
                    break;;
                2 ) echo "Building single file project"
                    python build.py -s
                    break;;
                3 ) echo "Uninstalling dependencies"
                    uninstall_chocolatey
                    break;;
                4 ) echo "Updating project"
                    python build.py -u
                    break;;
                * ) echo "[ERR] Invalid option";;
          esac
done

echo "Done"