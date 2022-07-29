# Yolo RGBD Demo program
---

文件目录说明
```
YoloRGBD
    ├─assets    ------ 资源目录，存放图片，字体文件等
    ├─bin       ------ 默认可执行文件生成目录
    │  └─conf   ------ conf文件用于存放Debug生成的json文件
    ├─iconwin   ------ windows下icon的源文件，文件下app.rc是编译所需文件
    ├─include   ------ 头文件目录
    ├─qml       ------ qml界面文件目录
    ├─script    ------ 脚本文件目录，`nvidia_gpu_info.ps1`用于获取Nvidia GPU使用率
    ├─src       ------ 源文件目录
    ├─video     ------ 测试用视频文件目录
    ├─weights   ------ 存放yolox (Intel OpenVino), yolov5 Engine(yolov5 Nvidia tensrorRT) 的网络模型文件
    ├─qtquickcontrols2.conf    ----- qml主题文件
    └─qml.qrc   ------ qml的资源文件，里面存放了`.qml`文件路径信息， 资源文件路径信息，qml主题文件路径信息，是编译所需的源文件
```

## 编译方式
---
建议采用Qt Creator，（CMake + ninja）构建系统构建。

环境说明：
||||
|----|----|----|
|类别|版本|备注|
|OS|Windows 10 20H2|CPU: Intel i5/i7 六代以上, GPU: Nvidia GTX 1060|
|Qt|5.15.2(MSVC 2019)|Qt Creator 7.0.0|
|OpenCV|4.x|3.x到4.x需更改部分代码|
|OpenVino|2021.4.1(2022.1.0也测试可行)|需要自行复制`tbb.dll/tbb_debug.dll`到可执行文件目录(或者将其添加至环境变量中)|
|Nvidia CUDA|10.3|将`bin`添加至环境变量中|
|Nvidia cuDNN|8.x|将`bin`添加至环境变量中|
|Nvidia TensorRT|8.x|将`bin`添加至环境变量中|
|OpenNI2|/|需拷贝OpenNI Lib下所有文件到可执行文件目录|

**OpenVino 仅支持Intel CPU/GPU以及部分Arm设备**，详情参见[Support lists](https://docs.openvino.ai/latest/openvino_docs_OV_UG_Working_with_devices.html)，如需编译请访问[Get Started](https://docs.openvino.ai/latest/get_started.html)下载所需套件。

**Nvidia CUDA/cuDNN/TensorRT 仅支持Nvidia自家GPU**，如需编译请访问[Nvidia Developer](https://developer.nvidia.com/)下载所需套件。