# 🚀 你现在应该做什么 — 完整行动清单

> 你已经有了 Quartus 工程并编译成功了，下面告诉你**具体做什么**来跑出仿真波形。
> **总共只需要 5 个动作**，预计 5 分钟完成。

---

## ✅ 第 1 步：复制文件到你的电脑

把整个 `fpga_waveform_generator` 文件夹下载/复制到你的电脑上。

建议放到你的 Quartus 工程目录旁边，例如：

```
D:\我的FPGA工程\
├── 你的Quartus工程文件夹\        ← 你已有的工程
│   ├── *.vhd                     ← 你已有的源文件
│   ├── *.qpf                     ← Quartus工程文件
│   └── ...
└── fpga_waveform_generator\      ← 把这个文件夹放在这里
    ├── src\
    │   ├── sine_lut.vhd          ← 正弦波查找表
    │   └── dds_waveform_gen.vhd  ← DDS顶层模块
    ├── sim\
    │   ├── tb_dds_waveform_gen.vhd  ← 测试平台
    │   └── run_sim.do               ← 一键仿真脚本
    └── README_simulation_guide.md   ← 详细指南
```

> 💡 **如果你已有自己的 VHDL 源文件**：
> 你可以选择使用我们提供的 `src/` 里的文件来仿真，
> 或者你也可以把 `sim/` 目录下的测试平台和脚本复制到你自己的工程目录下，
> 然后修改 `run_sim.do` 里的路径指向你自己的源文件。

---

## ✅ 第 2 步：打开 ModelSim

在你的电脑上找到 **ModelSim** 并打开它：

- **方式 A**：在开始菜单搜索 `ModelSim`，双击打开
- **方式 B**：在 Quartus 的安装目录下找到 ModelSim：
  ```
  C:\intelFPGA\18.0\modelsim_ase\win32aloem\modelsim.exe
  ```
  > 路径可能因你的安装位置不同而略有差异

打开后你会看到一个窗口，底部有一个白色的 **Transcript** 区域（命令行）。
**接下来的所有操作都在这个 Transcript 里输入命令。**

---

## ✅ 第 3 步：切换到 sim 目录

在 ModelSim 底部的 **Transcript** 窗口里，输入以下命令（然后按回车）：

```tcl
cd {D:/我的FPGA工程/fpga_waveform_generator/sim}
```

> ⚠️ **重要**：把 `D:/我的FPGA工程/` 换成你实际的路径！
>
> 例如如果你把文件放在了 `E:\FPGA_DDS\fpga_waveform_generator\sim`，就输入：
> ```tcl
> cd {E:/FPGA_DDS/fpga_waveform_generator/sim}
> ```
>
> **Windows 路径注意**：
> - ✅ 正确：`cd {D:/FPGA_Project/fpga_waveform_generator/sim}` （正斜杠）
> - ✅ 正确：`cd {D:\FPGA_Project\fpga_waveform_generator\sim}` （花括号包裹）
> - ❌ 错误：`cd D:\FPGA_Project\fpga_waveform_generator\sim` （反斜杠不加花括号会出错）

---

## ✅ 第 4 步：运行仿真脚本（一键完成！）

在 Transcript 里输入：

```tcl
do run_sim.do
```

然后**等待大约 10-30 秒**。

你会在 Transcript 里看到类似这样的输出：
```
=== Compilation complete ===
=== Testbench loaded ===
=== Waveform signals added ===
=== Starting simulation (approx. 2.7 ms) ===
=== Simulation complete ===
  Simulation finished successfully!
```

> **如果报错了怎么办？**
> - 看到 `cannot find` → 第 3 步的路径不对，重新检查
> - 看到 `Error loading design` → 编译出错，查看详细报错信息
> - 详细排错请看 `README_simulation_guide.md` 的第 7 节

---

## ✅ 第 5 步：查看波形

仿真跑完后，会自动弹出 **Wave 窗口**，里面显示了所有波形。

你会看到两个关键波形：
1. **DAC Analog** — 模拟波形图（数字值 -2048 ~ +2047）
2. **Voltage (V)** — 对应的实际电压（-10V ~ +10V）

### 波形的时间线（从左到右依次是）：

| 区域 | 时间 | 你会看到 |
|------|------|---------|
| 1️⃣ | 0 ~ 400 µs | **正弦波** (10 kHz, 满幅度 ±10V) |
| 2️⃣ | 400 ~ 800 µs | **三角波** (10 kHz, 峰值 20V) |
| 3️⃣ | 800 ~ 1200 µs | **方波** (10 kHz, ±10V) |
| 4️⃣ | 1200 ~ 1400 µs | 正弦波 (半幅度 ~±5V, 演示幅度调节) |
| 5️⃣ | 1400 ~ 2400 µs | 正弦波 (1 kHz, 低频) |
| 6️⃣ | 2400 ~ 2600 µs | 正弦波 (20 kHz, 高频) |

### 怎么放大看清楚：
- **鼠标滚轮**：缩放
- **Ctrl + 鼠标拖动**：框选放大
- 或在 Transcript 里输入 `wave zoom full` 看全局

---

## 🎉 完成！

到这里你应该已经看到了完整的 **正弦波、三角波、方波** 仿真波形。

### 接下来你可以做的事情：

- **想直接下板到 FPGA 实验箱，用示波器看真实波形？** → 🔧 **查看 [HARDWARE_GUIDE.md](HARDWARE_GUIDE.md)**
- **想看不同频率的波形？** → 查看 `README_simulation_guide.md` 的频率对照表（第 8 节）
- **想从 Quartus 直接启动仿真？** → 查看 `README_simulation_guide.md` 的方法二（第 5 节）
- **想把这些代码集成到你已有的工程？** → 把 `src/` 下两个 `.vhd` 文件添加到你的 Quartus 工程即可
- **波形有问题？** → 查看 `README_simulation_guide.md` 的常见问题（第 7 节）

---

## ❓ 遇到问题？

### 最常见的 3 个问题：

**问题 1：输入 `do run_sim.do` 后报错 "can't read ... no such file"**
→ 你的路径不对。输入 `pwd` 看看当前在哪个目录，确保在 `sim` 目录下。

**问题 2：波形窗口是空的**
→ 仿真可能没运行完。在 Transcript 输入 `run 3 ms`，然后 `wave zoom full`。

**问题 3：波形看起来是锯齿/方块，不像正弦波**
→ 你看的缩放级别太大了。用鼠标滚轮放大到 0.3~400 µs 区域就能看到平滑的正弦波。
