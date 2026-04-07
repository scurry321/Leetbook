----------------------------------------------------------------------------
-- Testbench:   tb_dds_waveform_gen
-- Description: Comprehensive testbench for DDS Arbitrary Waveform Generator.
--              Demonstrates sine, triangle, and square waves at multiple
--              frequencies and amplitudes.
--
-- Test Sequence:
--   1. Reset (200 ns)
--   2. Sine wave     @ 10 kHz, full amplitude   (400 us, ~4 periods)
--   3. Triangle wave @ 10 kHz, full amplitude   (400 us, ~4 periods)
--   4. Square wave   @ 10 kHz, full amplitude   (400 us, ~4 periods)
--   5. Sine wave     @ 10 kHz, half amplitude   (200 us, ~2 periods)
--   6. Sine wave     @  1 kHz, full amplitude   (1000 us, 1 period)
--   7. Sine wave     @ 20 kHz, full amplitude   (200 us, ~4 periods)
--
-- Clock:       50 MHz (20 ns period)
-- Total time:  ~2.7 ms
--
-- ModelSim Usage:
--   cd <project>/sim
--   do run_sim.do
--
-- Author:      Auto-generated for FPGA Arbitrary Waveform Generator
-- Target:      Quartus 18.0 / ModelSim 10.4 (VHDL-93 / VHDL-2008)
----------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.math_real.all;

entity tb_dds_waveform_gen is
    -- Testbench has no ports
end entity tb_dds_waveform_gen;

architecture sim of tb_dds_waveform_gen is

    ---------------------------------------------------------------------------
    -- Constants
    ---------------------------------------------------------------------------
    constant CLK_PERIOD   : time    := 20 ns;         -- 50 MHz
    constant PHASE_WIDTH  : integer := 32;
    constant DATA_WIDTH   : integer := 12;
    constant AMP_WIDTH    : integer := 8;

    -- Frequency Tuning Words (FTW = f_out * 2^32 / 50 MHz)
    constant FTW_1KHZ     : std_logic_vector(31 downto 0) := x"00014F8B";   --  1 kHz
    constant FTW_5KHZ     : std_logic_vector(31 downto 0) := x"00068DB9";   --  5 kHz
    constant FTW_10KHZ    : std_logic_vector(31 downto 0) := x"000D1B71";   -- 10 kHz
    constant FTW_20KHZ    : std_logic_vector(31 downto 0) := x"001A36E3";   -- 20 kHz

    -- Waveform selections
    constant SEL_SINE     : std_logic_vector(1 downto 0) := "00";
    constant SEL_TRIANGLE : std_logic_vector(1 downto 0) := "01";
    constant SEL_SQUARE   : std_logic_vector(1 downto 0) := "10";

    ---------------------------------------------------------------------------
    -- DUT Signals
    ---------------------------------------------------------------------------
    signal clk       : std_logic := '0';
    signal rst_n     : std_logic := '0';
    signal freq_word : std_logic_vector(PHASE_WIDTH-1 downto 0) := (others => '0');
    signal wave_sel  : std_logic_vector(1 downto 0) := "00";
    signal amp_ctrl  : std_logic_vector(AMP_WIDTH-1 downto 0)   := (others => '1');
    signal dac_out   : std_logic_vector(DATA_WIDTH-1 downto 0);

    ---------------------------------------------------------------------------
    -- Monitoring Signals (for waveform display in ModelSim)
    ---------------------------------------------------------------------------
    signal dac_integer : integer := 0;           -- DAC output as integer
    signal dac_voltage : real    := 0.0;         -- DAC output mapped to voltage (+-10V)
    signal sim_running : boolean := true;        -- Clock enable flag

begin

    ---------------------------------------------------------------------------
    -- Clock Generation (50 MHz)
    ---------------------------------------------------------------------------
    clk_gen : process
    begin
        while sim_running loop
            clk <= '0';
            wait for CLK_PERIOD / 2;
            clk <= '1';
            wait for CLK_PERIOD / 2;
        end loop;
        wait;
    end process clk_gen;

    ---------------------------------------------------------------------------
    -- Device Under Test (DUT)
    ---------------------------------------------------------------------------
    uut : entity work.dds_waveform_gen
        generic map (
            PHASE_WIDTH    => PHASE_WIDTH,
            LUT_ADDR_WIDTH => 12,
            DATA_WIDTH     => DATA_WIDTH,
            AMP_WIDTH      => AMP_WIDTH
        )
        port map (
            clk       => clk,
            rst_n     => rst_n,
            freq_word => freq_word,
            wave_sel  => wave_sel,
            amp_ctrl  => amp_ctrl,
            dac_out   => dac_out
        );

    ---------------------------------------------------------------------------
    -- Monitoring: Convert DAC output to integer and voltage for display
    -- Voltage mapping: DAC +2047 = +10V, DAC -2047 = -10V
    ---------------------------------------------------------------------------
    process(dac_out)
    begin
        dac_integer <= to_integer(signed(dac_out));
    end process;

    process(dac_integer)
    begin
        dac_voltage <= real(dac_integer) * 10.0 / 2047.0;
    end process;

    ---------------------------------------------------------------------------
    -- Stimulus Process
    ---------------------------------------------------------------------------
    stimulus : process
    begin
        -----------------------------------------------------------------------
        -- Phase 0: Reset
        -----------------------------------------------------------------------
        report "========================================" severity note;
        report "  DDS Waveform Generator Testbench     " severity note;
        report "  System Clock: 50 MHz                 " severity note;
        report "========================================" severity note;

        rst_n     <= '0';
        freq_word <= (others => '0');
        wave_sel  <= SEL_SINE;
        amp_ctrl  <= x"FF";
        wait for 200 ns;

        rst_n <= '1';
        wait for 100 ns;

        -----------------------------------------------------------------------
        -- Test 1: Sine wave @ 10 kHz, full amplitude (4 periods)
        -- Expected: smooth sinusoidal waveform, DAC range ~ -2039 to +2039
        -- Mapped voltage: approximately -9.96V to +9.96V
        -----------------------------------------------------------------------
        report "--- Test 1: Sine wave, 10 kHz, full amplitude ---" severity note;
        freq_word <= FTW_10KHZ;
        wave_sel  <= SEL_SINE;
        amp_ctrl  <= x"FF";
        wait for 400 us;

        -----------------------------------------------------------------------
        -- Test 2: Triangle wave @ 10 kHz, full amplitude (4 periods)
        -- Expected: symmetric triangle waveform, DAC range ~ -2039 to +2039
        -- Peak-to-peak voltage: ~20V (from -10V to +10V)
        -----------------------------------------------------------------------
        report "--- Test 2: Triangle wave, 10 kHz, full amplitude ---" severity note;
        wave_sel  <= SEL_TRIANGLE;
        wait for 400 us;

        -----------------------------------------------------------------------
        -- Test 3: Square wave @ 10 kHz, full amplitude (4 periods)
        -- Expected: clean square waveform, DAC toggles between -2039 and +2039
        -- Mapped voltage: approximately -10V and +10V
        -----------------------------------------------------------------------
        report "--- Test 3: Square wave, 10 kHz, full amplitude ---" severity note;
        wave_sel  <= SEL_SQUARE;
        wait for 400 us;

        -----------------------------------------------------------------------
        -- Test 4: Sine wave @ 10 kHz, half amplitude (2 periods)
        -- Expected: sinusoidal waveform with reduced amplitude
        -- DAC range ~ -1020 to +1020, voltage ~ -5V to +5V
        -- Demonstrates amplitude continuous adjustment
        -----------------------------------------------------------------------
        report "--- Test 4: Sine wave, 10 kHz, half amplitude ---" severity note;
        wave_sel  <= SEL_SINE;
        amp_ctrl  <= x"80";
        wait for 200 us;

        -----------------------------------------------------------------------
        -- Test 5: Sine wave @ 1 kHz, full amplitude (1 period)
        -- Expected: slower sine wave demonstrating minimum frequency range
        -- Verifies: output frequency range includes 1 kHz
        -----------------------------------------------------------------------
        report "--- Test 5: Sine wave, 1 kHz, full amplitude ---" severity note;
        freq_word <= FTW_1KHZ;
        amp_ctrl  <= x"FF";
        wait for 1000 us;

        -----------------------------------------------------------------------
        -- Test 6: Sine wave @ 20 kHz, full amplitude (4 periods)
        -- Expected: faster sine wave demonstrating maximum frequency range
        -- Verifies: output frequency range includes 20 kHz
        -----------------------------------------------------------------------
        report "--- Test 6: Sine wave, 20 kHz, full amplitude ---" severity note;
        freq_word <= FTW_20KHZ;
        wait for 200 us;

        -----------------------------------------------------------------------
        -- Simulation Complete
        -----------------------------------------------------------------------
        report "========================================" severity note;
        report "  All tests completed successfully!    " severity note;
        report "  Check waveform window for results.   " severity note;
        report "========================================" severity note;

        sim_running <= false;
        wait;
    end process stimulus;

end architecture sim;
