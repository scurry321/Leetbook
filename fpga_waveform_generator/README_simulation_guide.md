# FPGA DDS 任意波形发生器 — Quartus 18.0 + ModelSim 10.4 仿真指南

> **🚀 第一次用？先看 [QUICK_START.md](QUICK_START.md) — 5 分钟跑出波形！**

## 你现在应该做什么（速览）

```
打开 ModelSim → cd {你的路径/fpga_waveform_generator/sim} → do run_sim.do → 看波形
```

就这 3 步。详细操作见 [QUICK_START.md](QUICK_START.md) 或下面的「方法一」。

---

## 目录

1. [项目概述](#1-项目概述)
2. [文件说明](#2-文件说明)
3. [技术参数](#3-技术参数)
4. [方法一：独立运行 ModelSim（推荐新手）](#4-方法一独立运行-modelsim推荐新手)
5. [方法二：Quartus + ModelSim 联合仿真 (NativeLink)](#5-方法二quartus--modelsim-联合仿真-nativelink)
6. [仿真波形解读](#6-仿真波形解读)
7. [常见问题与排错](#7-常见问题与排错)
8. [频率调节对照表](#8-频率调节对照表)
9. [如何集成到你已有的 Quartus 工程](#9-如何集成到你已有的-quartus-工程)

---

## 1. 项目概述

本项目是一个基于 **DDS (Direct Digital Synthesis，直接数字合成)** 原理的 **任意波形发生器**，可输出：

| 波形类型 | 幅度      | 波形选择码 (`wave_sel`) |
|---------|-----------|----------------------|
| 正弦波   | ±10V     | `"00"`               |
| 三角波   | 峰值 20V  | `"01"`               |
| 方波     | ±10V     | `"10"`               |

- **频率范围**：1 kHz ~ 20 kHz，连续可调
- **幅度**：通过 `amp_ctrl` 信号 (0~255) 连续调节
- **系统时钟**：50 MHz
- **DAC 输出位宽**：12 位有符号 (-2047 ~ +2047)

### DDS 原理简述

```
                ┌──────────────┐     ┌──────────┐     ┌──────────┐
 freq_word ──>  │ 相位累加器    │ ──> │ 波形查找表 │ ──> │ 幅度控制  │ ──> dac_out
                │ (32-bit)     │     │ (ROM/计算) │     │ (乘法器)  │
                └──────────────┘     └──────────┘     └──────────┘
                      ↑
                    clk (50MHz)
```

- **相位累加器**：每个时钟周期加上 `freq_word`（频率控制字）
- **输出频率** = `freq_word × f_clk / 2^32`
- **频率控制字 (FTW)** = `f_out × 2^32 / f_clk`

---

## 2. 文件说明

```
fpga_waveform_generator/
├── src/                              ← VHDL 源代码（综合 + 仿真均可用）
│   ├── sine_lut.vhd                  ← 正弦波查找表 ROM（4096 点 × 12-bit）
│   └── dds_waveform_gen.vhd          ← DDS 波形发生器顶层模块
├── sim/                              ← 仿真文件（仅用于 ModelSim 仿真）
│   ├── tb_dds_waveform_gen.vhd       ← 测试平台（Testbench）
│   └── run_sim.do                    ← ModelSim 自动化脚本
└── README_simulation_guide.md        ← 本文件（仿真指南）
```

### 各文件的作用

| 文件 | 作用 |
|------|------|
| `sine_lut.vhd` | 正弦波 ROM 查找表，使用 `MATH_REAL` 库在编译时自动计算 4096 个正弦值 |
| `dds_waveform_gen.vhd` | 顶层模块：相位累加器 + 正弦/三角/方波生成 + 波形选择 + 幅度控制 |
| `tb_dds_waveform_gen.vhd` | 测试平台：自动生成时钟、复位，依次输出各种波形供仿真查看 |
| `run_sim.do` | ModelSim 脚本：一键编译、加载、配置波形显示、运行仿真 |

---

## 3. 技术参数

| 参数 | 值 |
|------|------|
| 系统时钟频率 | 50 MHz |
| 相位累加器位宽 | 32 bit |
| 频率分辨率 | 50 MHz / 2^32 ≈ 0.012 Hz |
| 正弦 ROM 深度 | 4096 点 (12-bit 地址) |
| DAC 输出位宽 | 12 bit 有符号 |
| 正弦波失真度 | < 1.5% (4096 点 ROM, THD < 0.03%) |
| 幅度控制精度 | 8 bit (256 级) |
| 最低频率 | 1 kHz (FTW = 85899 = 0x00014F8B) |
| 最高频率 | 20 kHz (FTW = 1717987 = 0x001A36E3) |

---

## 4. 方法一：独立运行 ModelSim（推荐新手）

> **此方法最简单、最可靠**，不需要 Quartus 参与，直接在 ModelSim 中编译和仿真。

### 第一步：打开 ModelSim

1. 在电脑桌面或开始菜单中找到 **ModelSim 10.4** (或 ModelSim-Altera)
2. 双击打开

### 第二步：切换到仿真目录

在 ModelSim 底部的 **Transcript** 窗口（命令行窗口）中输入：

```tcl
cd {D:/你的路径/fpga_waveform_generator/sim}
```

> ⚠️ **注意事项**：
> - 将 `D:/你的路径/` 替换为你实际存放本项目的路径
> - **Windows 路径用正斜杠 `/` 或者用花括号 `{}` 包裹反斜杠路径**
> - 示例：`cd {D:/FPGA_Project/fpga_waveform_generator/sim}`
> - 示例：`cd D:/FPGA_Project/fpga_waveform_generator/sim`

### 第三步：运行仿真脚本

在 Transcript 窗口中输入：

```tcl
do run_sim.do
```

> 这一条命令会自动完成以下所有操作：
> 1. 创建工作库 (work)
> 2. 编译 `sine_lut.vhd`
> 3. 编译 `dds_waveform_gen.vhd`
> 4. 编译 `tb_dds_waveform_gen.vhd`
> 5. 加载仿真
> 6. 添加信号到波形窗口（含模拟波形显示）
> 7. 运行仿真 3 ms
> 8. 自动缩放波形到全局视图

### 第四步：查看波形

仿真完成后，**Wave 窗口**会自动显示波形。你会看到：

- **DAC Analog**：以模拟波形格式显示 DAC 数字输出（-2048 ~ +2047）
- **Voltage (V)**：以实际电压显示输出（-10V ~ +10V）

#### 波形时间线

| 时间段 | 波形 | 频率 | 幅度 |
|--------|------|------|------|
| 0 ~ 0.3 µs | 复位 | — | — |
| 0.3 µs ~ 400 µs | **正弦波** | 10 kHz | 满幅 (±10V) |
| 400 µs ~ 800 µs | **三角波** | 10 kHz | 满幅 (峰值 20V) |
| 800 µs ~ 1200 µs | **方波** | 10 kHz | 满幅 (±10V) |
| 1200 µs ~ 1400 µs | 正弦波 (减小幅度) | 10 kHz | 半幅 (~±5V) |
| 1400 µs ~ 2400 µs | 正弦波 | 1 kHz | 满幅 |
| 2400 µs ~ 2600 µs | 正弦波 | 20 kHz | 满幅 |

### 第五步：放大查看具体波形

- **鼠标滚轮**：缩放
- **Ctrl + 鼠标左键拖动**：放大选定区域
- **右键点击波形** → **Zoom** → **Zoom In / Zoom Out / Zoom Full**

建议放大到以下区域仔细查看：
1. **0.3 µs ~ 400 µs** 查看正弦波
2. **400 µs ~ 800 µs** 查看三角波
3. **800 µs ~ 1200 µs** 查看方波
4. **1200 µs ~ 1400 µs** 对比半幅度和满幅度的差异

---

## 5. 方法二：Quartus + ModelSim 联合仿真 (NativeLink)

> 如果你已经有 Quartus 工程并想从 Quartus 内部启动 ModelSim 仿真。

### 前提条件

- 已在 Quartus 18.0 中创建了工程
- 工程中已添加 `sine_lut.vhd` 和 `dds_waveform_gen.vhd` 并编译成功

### 步骤 1：将测试平台文件添加到工程

1. 打开 Quartus 工程
2. 菜单栏：**Project** → **Add/Remove Files in Project...**
3. 点击 **...** 按钮，浏览到 `sim/tb_dds_waveform_gen.vhd`
4. 点击 **Add**，然后 **OK**

### 步骤 2：设置 ModelSim 为仿真工具

1. 菜单栏：**Assignments** → **Settings...**
2. 在左侧树形菜单中展开 **EDA Tool Settings**，选择 **Simulation**
3. 设置如下：
   - **Tool name**: `ModelSim-Altera` (如果使用 Altera 自带版本) 或 `ModelSim` (如果是独立版本)
   - **Format for output netlist**: `VHDL`
   - **Output directory**: `simulation/modelsim`
4. 点击 **OK**

### 步骤 3：配置测试平台

1. 菜单栏：**Assignments** → **Settings...**
2. **EDA Tool Settings** → **Simulation**
3. 在 **NativeLink settings** 区域，选择 **Compile test bench:**
4. 点击 **Test Benches...** 按钮
5. 点击 **New...** 按钮
6. 填写如下信息：
   - **Test bench name**: `tb_dds_waveform_gen`
   - **Top level module in test bench**: `tb_dds_waveform_gen`
   - **Simulation period**: `End simulation at:` 填 `3 ms`
   - 在 **Test bench and simulation files** 区域，点击 **...** 添加 `sim/tb_dds_waveform_gen.vhd`
7. 点击 **OK** → **OK** → **OK**

### 步骤 4：运行仿真

1. 菜单栏：**Tools** → **Run Simulation Tool** → **RTL Simulation**
2. Quartus 会自动启动 ModelSim，编译文件，加载仿真
3. 在 ModelSim 中，仿真可能不会自动运行，你需要：
   - 在 Transcript 窗口输入 `run 3 ms` 回车
   - 或点击工具栏的 **Run All** 按钮

### 步骤 5：手动添加波形信号

如果 ModelSim 打开后波形窗口是空的：

1. 在左侧 **sim** 标签页中，展开 `tb_dds_waveform_gen`
2. 右键点击 `dac_integer` → **Add Wave**
3. 右键点击 `dac_voltage` → **Add Wave**
4. 在 Wave 窗口中，右键点击 `dac_integer` 信号 → **Format** → **Analog (Step)**
5. 右键点击信号名 → **Format** → **设置 Height=150, Min=-2048, Max=2047**

> **更简单的方法**：在 ModelSim 的 Transcript 中，运行我们提供的 do 文件的信号配置部分。

---

## 6. 仿真波形解读

### 正弦波 (Sine Wave, wave_sel = "00")

```
   +10V ─  ╭──╮      ╭──╮
           ╱    ╲    ╱    ╲
    0V ─ ─╱──────╲──╱──────╲──
          ╲      ╱  ╲      ╱
  -10V ─   ╰──╯      ╰──╯
```

- 使用 4096 点 ROM 查找表，确保 THD (总谐波失真) < 1.5%
- 波形应该光滑连续，没有明显台阶感（12-bit 精度，4096 点/周期）
- 在 10 kHz 时，每个周期对应约 100 µs

### 三角波 (Triangle Wave, wave_sel = "01")

```
   +10V ─    ╱╲        ╱╲
            ╱  ╲      ╱  ╲
    0V ─ ─╱────╲────╱────╲──
          ╱      ╲  ╱      ╲
  -10V ─ ╱        ╲╱        ╲
```

- 峰峰值为 20V（从 -10V 到 +10V）
- 上升和下降斜率相等，完全对称
- 转折点处应尖锐（不圆滑）

### 方波 (Square Wave, wave_sel = "10")

```
   +10V ─  ┌────┐    ┌────┐
           │    │    │    │
    0V ─ ──┤    ├────┤    ├──
           │    │    │    │
  -10V ─   └────┘    └────┘
```

- 幅度为 ±10V
- 占空比为 50%
- 上升沿和下降沿在 1 个时钟周期（20 ns）内完成

---

## 7. 常见问题与排错

### Q1: 编译报错 "Cannot find design unit: work.sine_lut"

**原因**：编译顺序不对，`sine_lut.vhd` 需要在 `dds_waveform_gen.vhd` 之前编译。

**解决**：
```tcl
vcom -93 -work work ../src/sine_lut.vhd
vcom -93 -work work ../src/dds_waveform_gen.vhd
vcom -93 -work work ./tb_dds_waveform_gen.vhd
```

### Q2: 编译报错 "Library ieee.math_real not found" 或 "Cannot find package math_real"

**原因**：某些精简版的 ModelSim 可能不包含 `math_real` 库。

**解决**：
- ModelSim-Altera (随 Quartus 安装) 应该包含此库
- 如果仍有问题，在 `run_sim.do` 中将 `-2008` 改为 `-93`：
  ```tcl
  vcom -93 -work work ../src/sine_lut.vhd
  vcom -93 -work work ../src/dds_waveform_gen.vhd
  vcom -93 -work work ./tb_dds_waveform_gen.vhd
  ```

### Q3: 波形窗口没有显示任何波形

**解决步骤**：
1. 确认仿真已经运行（Transcript 中应显示 "Simulation complete"）
2. 如果 Wave 窗口为空，手动添加信号：
   - 在 Objects 窗口中找到信号
   - 右键 → Add Wave
3. 添加信号后，重新运行：
   ```tcl
   restart
   run 3 ms
   ```

### Q4: 波形显示为方块而不是平滑曲线

**原因**：显示格式不是模拟格式。

**解决**：
1. 在 Wave 窗口中，右键点击要修改的信号名
2. 选择 **Format** → **Analog (Step)** 或 **Analog (Interpolated)**
3. 再次右键 → **Format** → 设置合适的 **Height** (建议 150)、**Min** (-2048) 和 **Max** (2047)

### Q5: 看不到完整的波形，只看到一部分

**解决**：
```tcl
wave zoom full
```
或者在 Wave 窗口菜单中：**View** → **Zoom** → **Zoom Full**

### Q6: ModelSim 找不到 VHDL 文件

**原因**：当前目录不正确。

**解决**：
```tcl
pwd
```
确认当前目录是 `fpga_waveform_generator/sim/`。如果不是，使用 `cd` 命令切换。

### Q7: 想重新运行仿真

```tcl
restart -f
run 3 ms
wave zoom full
```

### Q8: 想修改频率后重新仿真

直接修改 `tb_dds_waveform_gen.vhd` 中的 FTW 常量值，然后：
```tcl
# 重新编译测试平台
vcom -2008 -work work ./tb_dds_waveform_gen.vhd
# 重新启动仿真
restart -f
run 3 ms
wave zoom full
```

---

## 8. 频率调节对照表

频率控制字 (FTW) 计算公式：

```
FTW = round(f_out × 2^32 / 50000000)
```

| 目标频率 (Hz) | FTW (十进制) | FTW (十六进制) | VHDL 赋值 |
|--------------|------------|--------------|-----------|
| 1,000 | 85,899 | 0x00014F8B | `freq_word <= x"00014F8B";` |
| 2,000 | 171,799 | 0x00029F17 | `freq_word <= x"00029F17";` |
| 3,000 | 257,698 | 0x0003EEA2 | `freq_word <= x"0003EEA2";` |
| 4,000 | 343,597 | 0x00053E2D | `freq_word <= x"00053E2D";` |
| 5,000 | 429,497 | 0x00068DB9 | `freq_word <= x"00068DB9";` |
| 6,000 | 515,396 | 0x0007DD44 | `freq_word <= x"0007DD44";` |
| 7,000 | 601,296 | 0x00092CD0 | `freq_word <= x"00092CD0";` |
| 8,000 | 687,195 | 0x000A7C5B | `freq_word <= x"000A7C5B";` |
| 9,000 | 773,094 | 0x000BCBE6 | `freq_word <= x"000BCBE6";` |
| 10,000 | 858,993 | 0x000D1B71 | `freq_word <= x"000D1B71";` |
| 11,000 | 944,893 | 0x000E6AFD | `freq_word <= x"000E6AFD";` |
| 12,000 | 1,030,792 | 0x000FBA88 | `freq_word <= x"000FBA88";` |
| 13,000 | 1,116,692 | 0x00110A14 | `freq_word <= x"00110A14";` |
| 14,000 | 1,202,591 | 0x0012599F | `freq_word <= x"0012599F";` |
| 15,000 | 1,288,490 | 0x0013A92A | `freq_word <= x"0013A92A";` |
| 16,000 | 1,374,390 | 0x0014F8B6 | `freq_word <= x"0014F8B6";` |
| 17,000 | 1,460,289 | 0x00164841 | `freq_word <= x"00164841";` |
| 18,000 | 1,546,188 | 0x001797CC | `freq_word <= x"001797CC";` |
| 19,000 | 1,632,088 | 0x0018E758 | `freq_word <= x"0018E758";` |
| 20,000 | 1,717,987 | 0x001A36E3 | `freq_word <= x"001A36E3";` |

---

## 快速启动 (Quick Start)

> **👉 更详细的快速启动指南请看 [QUICK_START.md](QUICK_START.md)**

如果你只想尽快看到波形，只需要执行以下 **两步**：

**第 1 步**：打开 ModelSim，在 Transcript 窗口输入：
```tcl
cd {你的完整路径/fpga_waveform_generator/sim}
```

**第 2 步**：输入：
```tcl
do run_sim.do
```

**完成！** 等待约 10 秒后，Wave 窗口会显示所有波形。

---

## 9. 如何集成到你已有的 Quartus 工程

> 如果你已经有一个 DDS 波形发生器的 Quartus 工程，想把这里的仿真文件与你的工程配合使用。

### 场景 A：你的工程里已有自己的 VHDL 源文件

如果你已经有自己编写的 `dds_waveform_gen.vhd` 等源文件，你只需要：

1. **复制 `sim/` 文件夹** 到你的 Quartus 工程目录下：
   ```
   你的工程目录/
   ├── 你的源文件.vhd
   ├── 你的工程.qpf
   └── sim/                          ← 复制到这里
       ├── tb_dds_waveform_gen.vhd
       └── run_sim.do
   ```

2. **修改 testbench**：打开 `sim/tb_dds_waveform_gen.vhd`，把 DUT 实例化部分改为你自己的顶层模块名和端口

3. **修改 `run_sim.do`**：把编译路径改为指向你自己的源文件
   ```tcl
   # 改成你自己的文件路径
   vcom -2008 -work work ../你的源文件1.vhd
   vcom -2008 -work work ../你的源文件2.vhd
   vcom -2008 -work work ./tb_dds_waveform_gen.vhd
   ```

### 场景 B：你想直接使用本项目提供的源文件

1. **打开 Quartus**，打开你的工程
2. 菜单 **Project** → **Add/Remove Files in Project...**
3. 添加以下两个文件：
   - `fpga_waveform_generator/src/sine_lut.vhd`
   - `fpga_waveform_generator/src/dds_waveform_gen.vhd`
4. 点击 **OK**
5. 在 Quartus 中重新编译：**Processing** → **Start Compilation**
6. 编译成功后，按照上面「方法一」或「方法二」运行仿真

### 场景 C：你只想仿真，不改动 Quartus 工程

直接按照「方法一」操作：
```tcl
cd {你的路径/fpga_waveform_generator/sim}
do run_sim.do
```
这个方式完全独立于 Quartus，不需要修改你的工程。

---

*本指南适用于 Quartus 18.0 + ModelSim 10.4 环境*
