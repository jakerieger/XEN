"""
Script for creating new GLEngine projects.

Projects are created in a "Projects" subdirectory of
GLEngine's source for simplicity's sake
"""

import os

projects_dir = os.getcwd().replace("Scripts", "Projects")

if not os.path.exists(projects_dir):
    os.mkdir(projects_dir)


def get_project_name():
    name = input("Project name: ")
    if name == "":
        print("Project name cannot be empty!")
        get_project_name()
    if " " in name:
        name = name.replace(" ", "_")

    return name


project_name = get_project_name()
project_dir = os.path.join(projects_dir, project_name)
if not os.path.exists(project_dir):
    os.mkdir(project_dir)
else:
    print("Project directory already exists!")
    exit(1)

cmake_template = '''cmake_minimum_required(VERSION 3.25)

project({name})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        ../../Core
)

# Disable deprecation warnings on Windows
if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

find_package(EASTL CONFIG REQUIRED)

add_executable({name} main.cpp)
target_link_libraries({name} PRIVATE GLEngine-Core EASTL)

file(COPY ../../Resources DESTINATION "${{CMAKE_CURRENT_BINARY_DIR}}")
'''.format(name=project_name)

with open(os.path.join(project_dir, "CMakeLists.txt"), "w") as cmake_file:
    cmake_file.write(cmake_template)

main_source = '''#include "Resources.h"
#include "GameApp.h"
#include "GraphicsContext.h"
#include "InputCodes.h"

class {app_name}App final : public IGameApp {{
public:
    {app_name}App() = default;
    void Startup() override;
    void Cleanup() override;
    void OnKeyDown(FKeyEvent& event) override;

private:
    //====================================//
    // Put application-specific data here //
    //====================================//
}};

void {app_name}App::Startup() {{
    //========================================//
    // - Initialize application-specific data //
    // - Create and load scenes               //
    // - Etc...                               //
    //========================================//
}}

void {app_name}App::Cleanup() {{}}

void {app_name}App::OnKeyDown(FKeyEvent& event) {{
    if (event.KeyCode == KeyCode::Escape) {{
        Graphics::MarkWindowForClose();
    }}
}}

int main(int argc, char* argv[]) {{
    Resources::SetCwd(argv[0]);

    {app_name}App app;
    Application::InitializeApp(app,
                               {{1280, 720}},
                               "{app_name}");
    Application::RunApp(app);

    return 0;
}}
'''.format(app_name=project_name)

with open(os.path.join(project_dir, "main.cpp"), "w") as main_cpp:
    main_cpp.write(main_source)

print("\nProject '{}' created successfully!".format(project_name))
print("In order to build this project, you need to add the following line to Projects.cmake:")
print("\n    add_subdirectory(Projects/{})\n".format(project_name))
print("Then you can reconfigure GLEngine and select '{}' as your build target.".format(project_name))
