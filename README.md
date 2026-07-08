# MiniImageRetrieval

> 一个基于 **Qt6 + OpenCV + C++17** 的轻量级桌面图像检索应用。加载本地图库后，自动提取图片的 **HSV 颜色、LBP 纹理、Hu 形状** 三类特征并融合，通过加权余弦相似度返回与查询图片最相似的 Top-K 结果，并以缩略图卡片形式展示在 Qt 界面中。

---

## 目录

1. [项目简介](#项目简介)
2. [环境与依赖](#环境与依赖)
3. [项目目录结构](#项目目录结构)
4. [快速开始](#快速开始)
5. [功能演示与使用示例](#功能演示与使用示例)
6. [核心算法说明](#核心算法说明)
7. [版本更新记录](#版本更新记录)
8. [常见问题](#常见问题)
9. [作者与联系方式](#作者与联系方式)

---

## 项目简介

MiniImageRetrieval 是一个面向学习与快速原型的**小型图像检索系统**：

- 采用模块化 DLL 设计，便于单独扩展图像加载、特征提取、索引和搜索模块。
- 使用 Qt6 构建简单的图形用户界面，操作直观。
- 搜索结果以**缩略图卡片**展示，包含图片预览、文件路径和相似度分数。
- 搜索时**自动排除查询图片本身**，避免它因相似度为 1.0 而始终排在第一位。
- 使用 OpenCV 提取图片的 HSV 颜色直方图、LBP 纹理直方图和 Hu 不变矩三类特征，并按权重融合。
- 支持在本地文件夹中加载 `.jpg` 和 `.png` 图片，实现“以图搜图”的最基本流程。

---

## 环境与依赖

| 依赖项 | 版本/说明 | 备注 |
|--------|----------|------|
| 操作系统 | Windows 10/11 | 使用 Windows API 遍历文件夹 |
| 编程语言 | C++17 | `CMakeLists.txt` 中已指定 `-std=c++17` |
| 构建工具 | CMake ≥ 3.15 | 项目根目录 `CMakeLists.txt` 要求 |
| 编译器 | MinGW-w64 GCC/G++ | 推荐使用与 OpenCV 同一套 MinGW |
| GUI 框架 | Qt 6.11.1 MinGW 版 | 默认路径：`D:\QT\6.11.1\mingw_64`，可通过环境变量 `QT6_DIR` 覆盖 |
| 图像处理库 | OpenCV 4.x MinGW 版 | 默认路径：`C:\Users\chick\opencv-project\opencv-install`，可通过环境变量 `OpenCV_DIR` 覆盖 |

> **注意**：`CMakeLists.txt` 中 Qt 和 OpenCV 的默认路径可通过环境变量覆盖。若安装路径不同，执行编译前设置：
> ```cmd
> set QT6_DIR=D:\QT\6.11.1\mingw_64\lib\cmake\Qt6
> set OpenCV_DIR=C:\Users\chick\opencv-project\opencv-install
> ```
> 或直接修改 `CMakeLists.txt` 中的默认路径。
>
> **路径建议**：为避免 CMake/MinGW 在处理中文路径时出错，建议将项目解压到英文路径下（例如 `D:\MiniImageRetrieval`）。

---

## 项目目录结构

```
MiniImageRetrieval/
├── App/                          # Qt 桌面主程序
│   ├── app.cpp                   # 主窗口、按钮事件、搜索流程、结果缩略图展示
│   └── CMakeLists.txt
├── ImageLoaderDLL/               # 图像加载模块（DLL）
│   ├── include/image_loader.h
│   ├── src/image_loader.cpp      # 使用 Win32 API 遍历 .jpg/.png
│   └── CMakeLists.txt
├── FeatureExtractorDLL/          # 特征提取模块（DLL）
│   ├── include/feature_extract.h
│   ├── src/feature_extract.cpp   # OpenCV 提取 114 维多特征向量
│   └── CMakeLists.txt
├── IndexerDLL/                   # 索引模块（DLL）
│   ├── include/indexer.h
│   ├── src/indexer.cpp           # 统计有效特征数量
│   └── CMakeLists.txt
├── SearcherDLL/                  # 检索模块（DLL）
│   ├── include/searcher.h
│   ├── src/searcher.cpp          # 加权余弦相似度 + Top-K 排序 + 排除查询图
│   └── CMakeLists.txt
├── common.h                      # 公共数据结构（ImageInfo / ImageDB / SearchResult）
├── CMakeLists.txt                # 根 CMake 配置
├── test_images/                  # 示例图片（动物、风景、人物、食物等）
├── build/                        # CMake 构建生成目录（运行前需先编译）
│   └── App/App.exe               # 编译好的可执行文件
├── README.md                     # 本文件
├── CHANGELOG.md                  # 版本更新记录
├── LICENSE                       # 开源许可证
└── .gitignore                    # Git 忽略规则
```

---

## 快速开始

### 方式一：直接运行已编译版本（推荐）

如果你使用的是独立的软件压缩包，解压后进入软件目录，直接运行 `launch.bat`：

```bash
cd "D:\MiniImageRetrieval"
launch.bat
```

`launch.bat` 会自动设置 Qt、OpenCV、MinGW 运行时以及项目 DLL 的 PATH，并启动 `App.exe`。

如果 `App.exe` 同级目录已经包含 Qt/OpenCV/MinGW 运行时 DLL，也可以直接双击 `App.exe` 运行。

### 方式二：从源码重新编译

1. 检查根目录 `CMakeLists.txt` 中的默认路径是否与你电脑一致：

   ```cmake
   set(Qt6_DIR "D:/QT/6.11.1/mingw_64/lib/cmake/Qt6" CACHE PATH "...")
   set(OpenCV_DIR "C:/Users/chick/opencv-project/opencv-install" CACHE PATH "...")
   ```

   也可以通过环境变量覆盖，无需修改文件：

   ```cmd
   set QT6_DIR=D:\QT\6.11.1\mingw_64\lib\cmake\Qt6
   set OpenCV_DIR=C:\Users\chick\opencv-project\opencv-install
   ```

2. 在项目根目录执行：

   ```bash
   cd "D:\MiniImageRetrieval"
   build.bat
   ```

   或手动：

   ```bash
   cmake -S . -B build -G "MinGW Makefiles"
   cmake --build build -j4
   ```

   构建完成后，项目 DLL 和 MinGW 运行时会自动复制到 `build/App/` 目录。

3. 运行生成的可执行文件：

   ```bash
   "build\App\App.exe"
   ```

### 方式三：在 VS Code 中运行

1. 安装扩展 **CMake Tools**。
2. 按 `Ctrl+Shift+P` → `CMake: Select Kit`，选择 MinGW 工具链。
3. 执行 `CMake: Configure` → `CMake: Build`。
4. 运行 `build/App/App.exe`。

> 建议直接使用 CMake Tools 进行 Configure / Build / Run，比手工编写 `tasks.json` 更稳定。

---

## 功能演示与使用示例

### 启动界面

运行 `App.exe` 后，会弹出一个标题为 **MiniImageRetrieval** 的窗口，包含：

1. **选择图库文件夹**：加载本地图片库。
2. **选择查询图片并搜索**：选择一张查询图片，系统返回最相似的图片列表。
3. **查询图片预览区**：显示当前选中的查询图片。
4. **搜索结果缩略图区**：以卡片网格形式展示 Top-K 相似图片。

### 操作步骤

1. 点击 **“选择图库文件夹”**，选择项目自带的示例图库：

   ```
   D:\MiniImageRetrieval\test_images
   ```

   程序会自动加载 `.jpg` / `.png` 图片、提取特征并建立索引。完成后状态区会提示：

   ```
   图库加载完成，共 X 张图片
   ```

2. 点击 **“选择查询图片并搜索”**，从 `test_images` 中任选一张图片，例如 `动物.jpg`。

3. 程序会在下方结果区显示 Top-5 相似结果的缩略图卡片：

   - 图片预览
   - 文件路径
   - 相似度分数（越大越相似）

4. 如果查询图片本身也在图库中，它会自动被排除，不会出现在结果列表里。

---

## 核心算法说明

本项目采用**多特征融合**策略，将图片表示为一个 **114 维** 的扁平特征向量，并通过加权余弦相似度进行检索。

### 1. 特征组成

| 特征块 | 维度 | 计算方法 |
|--------|------|----------|
| **HSV 颜色直方图** | 48 | 将图片从 BGR 转为 HSV，H、S、V 通道各计算 16-bin 直方图，分别 L1 归一化。 |
| **Uniform LBP 纹理直方图** | 59 | 将图片转灰度，手动计算 8 邻域 uniform LBP 直方图（58 个 uniform 模式 + 1 个非 uniform 桶），L1 归一化。 |
| **Hu 不变矩** | 7 | 对灰度图计算 `cv::moments` 和 `cv::HuMoments`，取 `log10(\|Hu\|)` 后 L2 归一化。 |
| **合计** | **114** | — |

### 2. 相似度度量

对查询图片和图库图片分别计算三种特征的余弦相似度，再按权重融合：

```
score = 0.35 × sim_HSV + 0.35 × sim_LBP + 0.30 × sim_Hu
```

默认权重可通过 `SearcherDLL` 导出的 `set_search_weights()` 接口在运行时调整。

### 3. Top-K 检索

对图库中所有图片计算加权相似度后降序排序，返回前 K 个结果（当前默认 K=5）。搜索时会跳过与查询图片路径相同的图片，避免查询图自己排第一。

---

## 版本更新记录

详见 [CHANGELOG.md](CHANGELOG.md)。

---

## 扩展说明

- 若未来需要更高准确率，可引入 CNN 预训练模型（如 MobileNetV2/ResNet18）提取深度特征，作为第四种特征块加入 `FeatureExtractorDLL`。
- 当前权重是通过在 `动物`、`风景`、`食物` 等测试集上观察效果后设置的通用初值，你可以根据实际数据在 `App.cpp` 中调用 `set_search_weights()` 进一步调优。

---

## 常见问题

| 问题 | 解决方案 |
|------|---------|
| 找不到 `Qt6Core.dll`、`Qt6Gui.dll` 等 | 将 `D:\QT\6.11.1\mingw_64\bin` 加入系统 PATH，或直接运行 `launch.bat` |
| 找不到 `libopencv_*.dll` | 将 `C:\Users\chick\opencv-project\opencv-install\x64\mingw\bin` 加入系统 PATH，或直接运行 `launch.bat` |
| Qt 平台插件错误 | 设置环境变量 `QT_QPA_PLATFORM_PLUGIN_PATH=D:\QT\6.11.1\mingw_64\plugins\platforms`，或运行 `launch.bat` |
| 找不到 `libgcc_s_seh-1.dll`、`libstdc++-6.dll` | 将 MinGW 的 `bin` 目录加入系统 PATH，或运行 `launch.bat` |
| 图库加载为 0 张 | 确认文件夹内只有 `.jpg` 或 `.png` 后缀的图片，且路径不含特殊字符 |
| CMake 配置失败，提示找不到路径 | 将项目解压到纯英文路径下，避免中文目录名 |

---

## 作者与联系方式

- **作者**：traetryyy
- **项目地址**：`D:\MiniImageRetrieval`
- **联系方式**：[填写你的邮箱/微信/GitHub]

欢迎提交 Issue 或 PR，一起完善这个小型图像检索项目！
