# Definições do CMake e do standard do C
cmake_minimum_required(VERSION 3.24)
set(CMAKE_C_STANDARD 23)

# Definir nome e versão do projeto
project(Projeto1 VERSION 0.0.1)

# Adicionar os ficheiros ao projeto
add_executable(projetofp main.c)