# ===========================================================================
# ModelSim Simulation Script for DDS Waveform Generator
# ===========================================================================
# Usage:
#   1. Open ModelSim 10.4
#   2. In the Transcript window, type:
#        cd {<full_path_to_project>/fpga_waveform_generator/sim}
#   3. Then type:
#        do run_sim.do
#
# This script will:
#   - Create a work library
#   - Compile all VHDL source files
#   - Load the testbench
#   - Configure waveform display (analog format for DAC output)
#   - Run the full simulation
#   - Zoom the waveform window to show all results
# ===========================================================================

# --- Step 1: Create/clean work library ---
if {[file exists work]} {
    vdel -all -lib work
}
vlib work
vmap work work

# --- Step 2: Compile VHDL sources (order matters) ---
# Note: Using -2008 for VHDL-2008 standard (supported by ModelSim 10.4).
#       If you encounter issues, try changing -2008 to -93.

# Compile the sine lookup table first (dependency of top module)
vcom -2008 -work work ../src/sine_lut.vhd

# Compile the top-level DDS waveform generator
vcom -2008 -work work ../src/dds_waveform_gen.vhd

# Compile the testbench
vcom -2008 -work work ./tb_dds_waveform_gen.vhd

puts "=== Compilation complete ==="

# --- Step 3: Load the testbench ---
vsim -t ns work.tb_dds_waveform_gen

puts "=== Testbench loaded ==="

# --- Step 4: Add signals to waveform window ---

# Clock and reset
add wave -divider "===== Clock & Reset ====="
add wave -label "clk"   /tb_dds_waveform_gen/clk
add wave -label "rst_n" /tb_dds_waveform_gen/rst_n

# Control signals
add wave -divider "===== Control Signals ====="
add wave -label "freq_word" -radix hexadecimal /tb_dds_waveform_gen/freq_word
add wave -label "wave_sel"  -radix binary      /tb_dds_waveform_gen/wave_sel
add wave -label "amp_ctrl"  -radix unsigned     /tb_dds_waveform_gen/amp_ctrl

# Internal phase accumulator
add wave -divider "===== Phase Accumulator ====="
add wave -label "phase_acc" -radix hexadecimal /tb_dds_waveform_gen/uut/phase_acc

# DAC output (digital value)
add wave -divider "===== DAC Output (Digital) ====="
add wave -label "dac_out (hex)"    -radix hexadecimal /tb_dds_waveform_gen/dac_out
add wave -label "dac_out (signed)" -radix decimal     /tb_dds_waveform_gen/dac_integer

# DAC output (analog waveform display) -- THIS IS THE KEY WAVEFORM VIEW
add wave -divider "===== DAC Output (Analog Waveform) ====="
add wave -label "DAC Analog" \
    -format Analog-Step \
    -height 150 \
    -max 2047 \
    -min -2048 \
    -radix decimal \
    /tb_dds_waveform_gen/dac_integer

# Voltage output (real value, mapped to +-10V)
add wave -divider "===== Output Voltage (+-10V) ====="
add wave -label "Voltage (V)" \
    -format Analog-Step \
    -height 150 \
    -max 10.0 \
    -min -10.0 \
    /tb_dds_waveform_gen/dac_voltage

puts "=== Waveform signals added ==="

# --- Step 5: Run simulation ---
puts "=== Starting simulation (approx. 2.7 ms) ==="
puts "=== Please wait... ==="
run 3 ms

puts "=== Simulation complete ==="

# --- Step 6: Zoom waveform to show all data ---
wave zoom full

puts "============================================"
puts "  Simulation finished successfully!"
puts "  Check the Wave window for waveforms."
puts ""
puts "  Waveform Timeline:"
puts "    0    - 0.3 us  : Reset"
puts "    0.3  - 400 us  : Sine wave @ 10 kHz"
puts "    400  - 800 us  : Triangle wave @ 10 kHz"
puts "    800  - 1200 us : Square wave @ 10 kHz"
puts "    1200 - 1400 us : Sine wave @ 10 kHz (half amplitude)"
puts "    1400 - 2400 us : Sine wave @ 1 kHz"
puts "    2400 - 2600 us : Sine wave @ 20 kHz"
puts ""
puts "  Tips:"
puts "    - Use mouse scroll to zoom in/out"
puts "    - Click and drag to pan"
puts "    - Right-click on signals for more options"
puts "============================================"
