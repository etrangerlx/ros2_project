# 在 VSCode 中调试与可视化（ROS2 入门 21 讲）

本文说明如何在 VSCode（含 Trae）中对本工作空间 `<WS>`（`/root/ros2_ws/ros2_21_tutorials`）的节点进行**断点调试**与**结果可视化**。

> 由于编辑器对 `.vscode/` 目录做了保护，本机无法由工具自动写入，请按第 1 节的说明手动创建下列文件并粘贴对应内容。

---

## 0. 前置准备

### 0.1 安装扩展

在 VSCode 扩展市场安装（或打开工作空间后按提示安装，见附录 `extensions.json`）：

- `ms-vscode.cpptools` — C++ 调试（gdb）
- `ms-python.python` + `ms-python.debugpy` — Python 调试
- `ms-iot.vscode-ros` — ROS 辅助（`ros2` 命令、launch 语法高亮等，可选）

### 0.2 关键前提：让编辑器带上 ROS 环境

调试进程需要能加载 `rclpy` / `rclcpp`，因此**必须先 source ROS 环境再启动 VSCode/Trae**：

```bash
cd <WS>
source /opt/ros/humble/setup.bash
source <WS>/install/setup.bash
code .        # 若用 Trae，则用对应启动命令打开本目录
```

若无法从终端启动，也可在 VSCode 集成终端里使用 `settings.json` 中配置的 “ROS2 (humble)” 终端配置（已自动 source），或在 `launch.json` 的 `env` 字段里追加所需环境变量。

### 0.3 C++ 调试必须以 Debug 模式编译

C++ 要能命中断点，编译时必须带调试符号：

```bash
colcon build --symlink-install --cmake-args -DCMAKE_BUILD_TYPE=Debug
```

该命令已内置为 `tasks.json` 中的 `colcon: build (Debug)` 任务，并被 C++ 调试配置设为 `preLaunchTask`（按 F5 会自动先编译）。Python 无需重新编译。

---

## 1. 创建配置文件

在工作空间根目录下创建 `.vscode/` 目录，并新建以下 5 个文件，内容见附录。

| 文件 | 作用 |
|---|---|
| `tasks.json` | 定义编译/清理/启动仿真等任务 |
| `launch.json` | 定义 C++ 与 Python 的调试配置（F5 下拉选择） |
| `settings.json` | 文件关联、Python/ROS 路径、ROS 集成终端 |
| `c_cpp_properties.json` | C++ 智能提示的 include 路径 |
| `extensions.json` | 推荐安装的扩展 |

---

## 2. 调试操作流程

### 2.1 C++ 节点（gdb）

1. 在 `install/learning_node_cpp/lib/learning_node_cpp/` 等目标可执行文件对应的 **源码**（如 [node_helloworld_class.cpp](file:///root/ros2_ws/ros2_21_tutorials/learning_node_cpp/src/node_helloworld_class.cpp)）中，点击行号左侧打**红点断点**。
2. 打开调试面板（`Ctrl+Shift+D`），在顶部下拉选择例如 `C++: learning_node_cpp / node_helloworld_class`。
3. 按 `F5` 启动：会先执行 Debug 编译，再启动 gdb，命中后暂停在断点。
4. 使用调试工具条：继续 `F5`、单步跳过 `F10`、单步进入 `F11`、跳出 `Shift+F11`、重启 `Ctrl+Shift+F5`、停止 `Shift+F5`。
5. 在左侧「变量 / 监视 / 调用堆栈」面板查看运行状态；在「调试控制台」输入表达式求值。

> 相同话题的两个节点需分别调试时，可先在一个终端 `ros2 run` 起对端，再用 F5 调试另一端。

### 2.2 Python 节点（debugpy）

1. 在源码（如 [node_helloworld.py](file:///root/ros2_ws/ros2_21_tutorials/learning_node/learning_node/node_helloworld.py)）中打断点。
2. 在调试下拉选择 `Python: learning_node / node_helloworld`（或 `Python: learning_topic / topic_helloworld_pub`）。
3. 按 `F5` 启动，程序运行在集成终端中，命中断点后即可单步、查看变量、在控制台交互（相当于 Python 交互式调试）。

> 也可选 `Python: 当前打开的脚本`，直接对当前编辑的 `.py` 文件调试。

---

## 3. 可视化

调试运行时想“看到”数据或机器人状态，可用以下方式：

### 3.1 RViz2 / Gazebo（机器人、传感器可视化）

项目已提供 launch 文件，直接在集成终端运行：

```bash
# 关节/URDF 模型可视化（含 RViz2）
ros2 launch learning_urdf display.launch.py

# xacro 版本
ros2 launch learning_urdf display_xacro.launch.py

# 在 Gazebo 中加载机器人
ros2 launch learning_gazebo load_urdf_into_gazebo.launch.py
```

也可通过“运行任务”启动（`Ctrl+Shift+P` → Tasks: Run Task → 对应 `ros2: launch ...`）。

### 3.2 rqt_plot / PlotJuggler（把话题数据画成曲线）

```bash
# 系统工具 rqt_plot，例如绘制 /chatter 的 data 字段
ros2 run rqt_plot rqt_plot

# 或安装 PlotJuggler 2（更强的曲线/数据分析工具）
ros2 run plotjuggler plotjuggler     # 需先 sudo apt install ros-humble-plotjuggler-ros
```

### 3.3 VSCode 内直接看图像 / 点云

- `learning_cv` 的 `cv_bridge_test` 会把相机图像转成 OpenCV 窗口，可直接在调试中观察：`ros2 run learning_cv cv_bridge_test`
- 图像/点云话题可用 `rviz2` 添加 `Image` / `PointCloud2` 显示项查看。

---

## 附录：配置文件内容

### A. `.vscode/tasks.json`

```json
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
      "label": "colcon: build current package (Debug)",
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
    },
    {
      "label": "ros2: launch rviz (learning_urdf)",
      "type": "shell",
      "command": "ros2 launch learning_urdf display.launch.py",
      "options": { "cwd": "${workspaceFolder}" },
      "problemMatcher": []
    },
    {
      "label": "ros2: launch gazebo (learning_gazebo)",
      "type": "shell",
      "command": "ros2 launch learning_gazebo load_urdf_into_gazebo.launch.py",
      "options": { "cwd": "${workspaceFolder}" },
      "problemMatcher": []
    }
  ],
  "inputs": [
    {
      "id": "packageName",
      "type": "promptString",
      "description": "要编译的功能包名（如 learning_topic_cpp）"
    }
  ]
}
```

### B. `.vscode/launch.json`

```json
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
      "name": "Python: learning_node / node_helloworld",
      "type": "debugpy",
      "request": "launch",
      "program": "${workspaceFolder}/learning_node/learning_node/node_helloworld.py",
      "args": [],
      "console": "integratedTerminal",
      "cwd": "${workspaceFolder}",
      "justMyCode": true,
      "env": { "PYTHONUNBUFFERED": "1" }
    },
    {
      "name": "Python: learning_topic / topic_helloworld_pub",
      "type": "debugpy",
      "request": "launch",
      "program": "${workspaceFolder}/learning_topic/learning_topic/topic_helloworld_pub.py",
      "args": [],
      "console": "integratedTerminal",
      "cwd": "${workspaceFolder}",
      "justMyCode": true,
      "env": { "PYTHONUNBUFFERED": "1" }
    },
    {
      "name": "Python: learning_topic / topic_helloworld_sub",
      "type": "debugpy",
      "request": "launch",
      "program": "${workspaceFolder}/learning_topic/learning_topic/topic_helloworld_sub.py",
      "args": [],
      "console": "integratedTerminal",
      "cwd": "${workspaceFolder}",
      "justMyCode": true,
      "env": { "PYTHONUNBUFFERED": "1" }
    },
    {
      "name": "Python: 当前打开的脚本",
      "type": "debugpy",
      "request": "launch",
      "program": "${file}",
      "args": [],
      "console": "integratedTerminal",
      "cwd": "${workspaceFolder}",
      "justMyCode": true,
      "env": { "PYTHONUNBUFFERED": "1" }
    }
  ]
}
```

### C. `.vscode/settings.json`

```json
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
  "python.analysis.extraPaths": ["/opt/ros/humble/lib/python3.10/site-packages"],
  "python.autoComplete.extraPaths": ["/opt/ros/humble/lib/python3.10/site-packages"],
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
```

### D. `.vscode/c_cpp_properties.json`

```json
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
```

### E. `.vscode/extensions.json`

```json
{
  "recommendations": [
    "ms-vscode.cpptools",
    "ms-python.python",
    "ms-python.debugpy",
    "ms-iot.vscode-ros"
  ]
}
```

---

## 常见问题

- **F5 起来了但断点不生效（C++）**：没有以 Debug 模式编译，用 `colcon build --cmake-args -DCMAKE_BUILD_TYPE=Debug` 重编，或确认用 `--symlink-install` 后 `install` 与源码一致。
- **提示找不到 `rclpy` / `rclcpp`**：编辑器启动前没有 source ROS 环境（见 0.2）。
- **Python 断点不命中**：确认 `justMyCode` 为 `true` 时只调试自己的代码、路径正确；调试的是源码 `.py` 而非 `install/` 里的生成脚本。
- **只能调试一个节点**：ROS 多节点场景下，对端节点用 `ros2 run` 在终端单独启动即可。
