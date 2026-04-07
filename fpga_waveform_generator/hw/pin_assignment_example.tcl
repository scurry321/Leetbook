# ===========================================================================
# Quartus Pin Assignment Script — DDS Waveform Generator
# ===========================================================================
#
# 使用方法 (How to use):
#   1. 在 Quartus 中打开你的工程
#   2. 菜单: Tools → Tcl Scripts...
#   3. 浏览到此文件, 点击 Run
#   4. 或者在 Quartus Tcl Console 中输入:
#        source pin_assignment_example.tcl
#
# ⚠️ 重要：你必须根据你自己的实验箱/开发板修改下面的引脚编号！
#    以下引脚编号仅为示例，基于常见的 Cyclone IV 开发板。
#    请参考你的实验箱原理图或用户手册来确定正确的引脚。
#
# ===========================================================================

puts "============================================"
puts "  DDS Waveform Generator Pin Assignment"
puts "============================================"
puts ""
puts "  WARNING: These are EXAMPLE pin numbers!"
puts "  You MUST modify them for YOUR board!"
puts ""

# --- 全局设置 ---
# 取消注释并修改为你的 FPGA 型号:
# set_global_assignment -name FAMILY "Cyclone IV E"
# set_global_assignment -name DEVICE EP4CE6E22C8
# set_global_assignment -name TOP_LEVEL_ENTITY waveform_gen_top

# ===========================================================================
# 时钟和复位 (Clock & Reset)
# ===========================================================================
# 50 MHz 晶振 — 查看你的板子上晶振连接到哪个引脚
set_location_assignment PIN_23 -to CLK_50M
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to CLK_50M

# 复位按钮 (通常是 KEY0 或一个独立的 RESET 按钮)
set_location_assignment PIN_25 -to RST_N
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to RST_N

# ===========================================================================
# 波形选择开关 (Waveform Selection Switches)
# ===========================================================================
# SW_WAVE[0] 和 SW_WAVE[1] — 用拨码开关选择波形
# "00"=正弦, "01"=三角, "10"=方波
set_location_assignment PIN_88 -to SW_WAVE[0]
set_location_assignment PIN_89 -to SW_WAVE[1]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW_WAVE[0]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to SW_WAVE[1]

# ===========================================================================
# 频率/幅度控制按钮 (Frequency/Amplitude Control Buttons)
# ===========================================================================
# 按钮通常是低电平有效 (按下=0, 松开=1)
set_location_assignment PIN_91 -to KEY_FREQ_UP
set_location_assignment PIN_90 -to KEY_FREQ_DOWN
set_location_assignment PIN_87 -to KEY_AMP_UP
set_location_assignment PIN_86 -to KEY_AMP_DOWN
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to KEY_FREQ_UP
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to KEY_FREQ_DOWN
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to KEY_AMP_UP
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to KEY_AMP_DOWN

# ===========================================================================
# DAC 数据输出 — 8 位 (DAC Data Output - 8-bit)
# ===========================================================================
# 这 8 个引脚连接到你的 DAC 芯片 (如 DAC0832) 或 R-2R 电阻网络
# 通常使用 GPIO 排针或专用 DAC 接口
# ⚠️ 必须根据你的板子修改这些引脚！
set_location_assignment PIN_30 -to DAC_DATA[0]
set_location_assignment PIN_31 -to DAC_DATA[1]
set_location_assignment PIN_32 -to DAC_DATA[2]
set_location_assignment PIN_33 -to DAC_DATA[3]
set_location_assignment PIN_34 -to DAC_DATA[4]
set_location_assignment PIN_38 -to DAC_DATA[5]
set_location_assignment PIN_39 -to DAC_DATA[6]
set_location_assignment PIN_42 -to DAC_DATA[7]

set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to DAC_DATA[0]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to DAC_DATA[1]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to DAC_DATA[2]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to DAC_DATA[3]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to DAC_DATA[4]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to DAC_DATA[5]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to DAC_DATA[6]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to DAC_DATA[7]

# ===========================================================================
# DAC 数据输出 — 12 位 (可选, 如果你的 DAC 支持 12 位)
# ===========================================================================
# 如果你只用 8 位 DAC, 可以不分配这些引脚
# set_location_assignment PIN_43 -to DAC_DATA_12[0]
# set_location_assignment PIN_44 -to DAC_DATA_12[1]
# set_location_assignment PIN_46 -to DAC_DATA_12[2]
# set_location_assignment PIN_49 -to DAC_DATA_12[3]
# set_location_assignment PIN_50 -to DAC_DATA_12[4]
# set_location_assignment PIN_51 -to DAC_DATA_12[5]
# set_location_assignment PIN_52 -to DAC_DATA_12[6]
# set_location_assignment PIN_53 -to DAC_DATA_12[7]
# set_location_assignment PIN_54 -to DAC_DATA_12[8]
# set_location_assignment PIN_55 -to DAC_DATA_12[9]
# set_location_assignment PIN_58 -to DAC_DATA_12[10]
# set_location_assignment PIN_59 -to DAC_DATA_12[11]

# ===========================================================================
# DAC 写入选通 (DAC Write Strobe)
# ===========================================================================
# 连接到 DAC0832 的 /WR 引脚
# 如果你用 R-2R 电阻网络, 不需要此信号
set_location_assignment PIN_60 -to DAC_WR_N
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to DAC_WR_N

# ===========================================================================
# LED 指示灯 (LED Indicators)
# ===========================================================================
set_location_assignment PIN_72 -to LED_SINE
set_location_assignment PIN_73 -to LED_TRI
set_location_assignment PIN_74 -to LED_SQR
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to LED_SINE
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to LED_TRI
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to LED_SQR

# 频率等级 LED (4 个 LED 显示当前频率档位)
set_location_assignment PIN_75 -to LED_FREQ[0]
set_location_assignment PIN_76 -to LED_FREQ[1]
set_location_assignment PIN_77 -to LED_FREQ[2]
set_location_assignment PIN_80 -to LED_FREQ[3]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to LED_FREQ[0]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to LED_FREQ[1]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to LED_FREQ[2]
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to LED_FREQ[3]

# ===========================================================================
# 输出驱动强度设置 (DAC 输出建议使用较强驱动)
# ===========================================================================
set_instance_assignment -name CURRENT_STRENGTH_NEW 8MA -to DAC_DATA[0]
set_instance_assignment -name CURRENT_STRENGTH_NEW 8MA -to DAC_DATA[1]
set_instance_assignment -name CURRENT_STRENGTH_NEW 8MA -to DAC_DATA[2]
set_instance_assignment -name CURRENT_STRENGTH_NEW 8MA -to DAC_DATA[3]
set_instance_assignment -name CURRENT_STRENGTH_NEW 8MA -to DAC_DATA[4]
set_instance_assignment -name CURRENT_STRENGTH_NEW 8MA -to DAC_DATA[5]
set_instance_assignment -name CURRENT_STRENGTH_NEW 8MA -to DAC_DATA[6]
set_instance_assignment -name CURRENT_STRENGTH_NEW 8MA -to DAC_DATA[7]
set_instance_assignment -name CURRENT_STRENGTH_NEW 8MA -to DAC_WR_N

puts ""
puts "============================================"
puts "  Pin assignment complete!"
puts ""
puts "  REMEMBER: Modify PIN numbers for YOUR board!"
puts "  Then: Processing -> Start Compilation"
puts "============================================"
