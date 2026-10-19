#!/usr/bin/env bash
# 在 ROS2 工作空间中一键生成 VSCode C++ 调试环境配置
# 用法： bash setup_vscode_cpp_debug.sh
set -e

WS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VSCODE_DIR="$WS_DIR/.vscode"
mkdir -p "$VSCODE_DIR"

echo "==> 生成配置文件到 $VSCODE_DIR"

# ---------------------------------------------------------------- tasks.json
cat > "$VSCODE_DIR/tasks.json" <<'JSON'
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "colcon: build (Debug)",
      "type": "shell",
      "command": "colcon build --symlink-install --cmake-args -DCMAKE_BUILD_TYPE=Debug",
      "options": { "cwd": "${workspaceFolder}" },
      "group": { "kind": "build", "isDefault": true },
      "problemMatcher": ["$gcc"],
      "presentation": { "reveal": "always", "panel": "shared" }
    },
    {
      "label": "colcon: build (Release)",
      "type": "shell",
      "command": "colcon build --symlink-install",
      "options": { "cwd": "${workspaceFolder}" },
      "group": "build",
      "problemMatcher": ["$gcc"],
      "presentation": { "reveal": "always", "panel": "shared" }
    },
    {
      "label": "colcon: build (Debug, 单包)",
      "type": "shell",
      "command": "colcon build --symlink-install --cmake-args -DCMAKE_BUILD_TYPE=Debug --packages-select ${input:packageName}",
      "options": { "cwd": "${workspaceFolder}" },
      "group": "build",
      "problemMatcher": ["$gcc"],
      "presentation": { "reveal": "always", "panel": "shared" }
    },
    {
      "label": "colcon: clean",
      "type": "shell",
      "command": "rm -rf build install log",
      "options": { "cwd": "${workspaceFolder}" },
      "problemMatcher": []
    }
  ],
  "inputs": [
    {
      "id": "packageName",
      "type": "promptString",
      "description": "要编译的 C++ 功能包名（如 learning_topic_cpp）"
    }
  ]
}
JSON

# --------------------------------------------------------------- launch.json
cat > "$VSCODE_DIR/launch.json" <<'JSON'
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "C++: learning_node_cpp / node_helloworld_class",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/install/learning_node_cpp/lib/learning_node_cpp/node_helloworld_class",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "setupCommands": [
        { "description": "Enable pretty-printing for gdb", "text": "-enable-pretty-printing", "ignoreFailures": true }
      ],
      "preLaunchTask": "colcon: build (Debug)"
    },
    {
      "name": "C++: learning_topic_cpp / topic_helloworld_pub",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/install/learning_topic_cpp/lib/learning_topic_cpp/topic_helloworld_pub",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "setupCommands": [
        { "description": "Enable pretty-printing for gdb", "text": "-enable-pretty-printing", "ignoreFailures": true }
      ],
      "preLaunchTask": "colcon: build (Debug)"
    },
    {
      "name": "C++: learning_topic_cpp / topic_helloworld_sub",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/install/learning_topic_cpp/lib/learning_topic_cpp/topic_helloworld_sub",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "setupCommands": [
        { "description": "Enable pretty-printing for gdb", "text": "-enable-pretty-printing", "ignoreFailures": true }
      ],
      "preLaunchTask": "colcon: build (Debug)"
    },
    {
      "name": "C++: learning_service_cpp / service_adder_server",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/install/learning_service_cpp/lib/learning_service_cpp/service_adder_server",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "setupCommands": [
        { "description": "Enable pretty-printing for gdb", "text": "-enable-pretty-printing", "ignoreFailures": true }
      ],
      "preLaunchTask": "colcon: build (Debug)"
    },
    {
      "name": "C++: learning_service_cpp / service_adder_client",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/install/learning_service_cpp/lib/learning_service_cpp/service_adder_client",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "setupCommands": [
        { "description": "Enable pretty-printing for gdb", "text": "-enable-pretty-printing", "ignoreFailures": true }
      ],
      "preLaunchTask": "colcon: build (Debug)"
    },
    {
      "name": "C++: learning_parameter_cpp / param_declare",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/install/learning_parameter_cpp/lib/learning_parameter_cpp/param_declare",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "setupCommands": [
        { "description": "Enable pretty-printing for gdb", "text": "-enable-pretty-printing", "ignoreFailures": true }
      ],
      "preLaunchTask": "colcon: build (Debug)"
    },
    {
      "name": "C++: learning_action_cpp / action_move_server",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/install/learning_action_cpp/lib/learning_action_cpp/action_move_server",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "setupCommands": [
        { "description": "Enable pretty-printing for gdb", "text": "-enable-pretty-printing", "ignoreFailures": true }
      ],
      "preLaunchTask": "colcon: build (Debug)"
    },
    {
      "name": "C++: learning_action_cpp / action_move_client",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/install/learning_action_cpp/lib/learning_action_cpp/action_move_client",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false,
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "setupCommands": [
        { "description": "Enable pretty-printing for gdb", "text": "-enable-pretty-printing", "ignoreFailures": true }
      ],
      "preLaunchTask": "colcon: build (Debug)"
    }
  ]
}
JSON

# ------------------------------------------------------------- settings.json
cat > "$VSCODE_DIR/settings.json" <<'JSON'
{
  "files.associations": {
    "*.msg": "plaintext",
    "*.srv": "plaintext",
    "*.action": "plaintext",
    "*.launch.py": "python",
    "*.urdf": "xml",
    "*.xacro": "xml",
    "*.world": "xml"
  },
  "C_Cpp.default.compilerPath": "/usr/bin/gcc",
  "C_Cpp.default.cppStandard": "c++17",
  "C_Cpp.default.intelliSenseMode": "linux-gcc-x64",
  "terminal.integrated.profiles.linux": {
    "ROS2 (humble)": {
      "path": "bash",
      "args": [
        "-c",
        "source /opt/ros/humble/setup.bash && source ${workspaceFolder}/install/setup.bash && exec bash"
      ]
    }
  },
  "terminal.integrated.defaultProfile.linux": "ROS2 (humble)"
}
JSON

# ------------------------------------------------------ c_cpp_properties.json
cat > "$VSCODE_DIR/c_cpp_properties.json" <<'JSON'
{
  "version": 4,
  "configurations": [
    {
      "name": "Linux",
      "compilerPath": "/usr/bin/gcc",
      "cStandard": "c17",
      "cppStandard": "c++17",
      "intelliSenseMode": "linux-gcc-x64",
      "includePath": [
        "${workspaceFolder}/**",
        "/opt/ros/humble/include/**"
      ],
      "browse": {
        "path": [
          "${workspaceFolder}/**",
          "/opt/ros/humble/include/**"
        ],
        "limitSymbolsToIncludedHeaders": true
      }
    }
  ]
}
JSON

# ---------------------------------------------------------- extensions.json
cat > "$VSCODE_DIR/extensions.json" <<'JSON'
{
  "recommendations": [
    "ms-vscode.cpptools",
    "ms-iot.vscode-ros"
  ]
}
JSON

echo "==> 完成，已生成："
ls -1 "$VSCODE_DIR"

echo ""
echo "==> 环境自检"
if command -v gdb >/dev/null 2>&1; then
  echo "  [OK]  gdb 已安装：$(gdb --version | head -1)"
else
  echo "  [!!]  未检测到 gdb，C++ 断点调试不可用，请先安装："
  echo "        sudo apt update && sudo apt install -y gdb"
fi

echo ""
echo "==> 后续步骤"
echo "  1. 安装扩展：ms-vscode.cpptools（C/C++）、ms-iot.vscode-ros（可选）"
echo "  2. 以 Debug 模式编译： colcon build --symlink-install --cmake-args -DCMAKE_BUILD_TYPE=Debug"
echo "  3. 在 .cpp 源码行号左侧打断点，按 F5 选择 “C++: ... / <节点>” 启动调试"
