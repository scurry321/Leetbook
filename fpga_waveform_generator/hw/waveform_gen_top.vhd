----------------------------------------------------------------------------
-- Module:      waveform_gen_top
-- Description: Hardware Top-Level for DDS Waveform Generator
--              Wraps the DDS core (dds_waveform_gen) with physical I/O:
--              - Switches for waveform selection
--              - Buttons for frequency up/down and amplitude up/down
--              - Parallel DAC data output (directly to external DAC)
--              - DAC write strobe (active-low, for DAC0832 or similar)
--              - LED indicators for current waveform type
--
-- Physical Interface:
--   Inputs:
--     CLK_50M       : 50 MHz crystal oscillator on your board
--     RST_N         : Active-low reset (button, directly to pin)
--     SW_WAVE[1:0]  : 2 toggle switches for waveform selection
--                       "00"=sine, "01"=triangle, "10"=square
--     KEY_FREQ_UP   : Push button to increase frequency (active-low)
--     KEY_FREQ_DOWN : Push button to decrease frequency (active-low)
--     KEY_AMP_UP    : Push button to increase amplitude (active-low)
--     KEY_AMP_DOWN  : Push button to decrease amplitude (active-low)
--
--   Outputs:
--     DAC_DATA[7:0] : 8-bit unsigned DAC data (for DAC0832 or R-2R)
--                     Mapped from signed 12-bit to unsigned 8-bit
--     DAC_DATA_12[11:0] : Full 12-bit unsigned DAC data (optional)
--     DAC_WR_N      : DAC write strobe (active-low, directly usable
--                     as DAC0832 /WR signal)
--     LED_SINE      : LED on when sine wave selected
--     LED_TRI       : LED on when triangle wave selected
--     LED_SQR       : LED on when square wave selected
--     LED_FREQ[3:0] : 4 LEDs indicating frequency level (binary)
--
-- Frequency Presets (10 steps):
--     Level 0:  1 kHz      Level 5:  6 kHz
--     Level 1:  2 kHz      Level 6:  8 kHz
--     Level 2:  3 kHz      Level 7: 10 kHz
--     Level 3:  4 kHz      Level 8: 15 kHz
--     Level 4:  5 kHz      Level 9: 20 kHz
--
-- DAC Output Mapping:
--     Internal signed (-2047 ~ +2047) is converted to:
--       8-bit unsigned (0 ~ 255) for DAC_DATA[7:0]
--       12-bit unsigned (0 ~ 4095) for DAC_DATA_12[11:0]
--     Midpoint voltage corresponds to DAC output = 128 (8-bit) or 2048 (12-bit)
--
-- Author:      Auto-generated for FPGA Arbitrary Waveform Generator
-- Target:      Quartus 18.0 / Cyclone IV (VHDL-93)
----------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity waveform_gen_top is
    port (
        -- Clock and Reset
        CLK_50M        : in  std_logic;                      -- 50 MHz oscillator
        RST_N          : in  std_logic;                      -- Active-low reset button

        -- Waveform Selection (toggle switches)
        SW_WAVE        : in  std_logic_vector(1 downto 0);   -- "00"=sine "01"=tri "10"=sqr

        -- Frequency and Amplitude Control (push buttons, active-low)
        KEY_FREQ_UP    : in  std_logic;                      -- Increase frequency
        KEY_FREQ_DOWN  : in  std_logic;                      -- Decrease frequency
        KEY_AMP_UP     : in  std_logic;                      -- Increase amplitude
        KEY_AMP_DOWN   : in  std_logic;                      -- Decrease amplitude

        -- DAC Output (directly to external DAC chip)
        DAC_DATA       : out std_logic_vector(7 downto 0);   -- 8-bit unsigned output
        DAC_DATA_12    : out std_logic_vector(11 downto 0);  -- 12-bit unsigned output
        DAC_WR_N       : out std_logic;                      -- DAC write strobe (active-low)

        -- LED Indicators
        LED_SINE       : out std_logic;                      -- Sine wave indicator
        LED_TRI        : out std_logic;                      -- Triangle wave indicator
        LED_SQR        : out std_logic;                      -- Square wave indicator
        LED_FREQ       : out std_logic_vector(3 downto 0)    -- Frequency level (binary)
    );
end entity waveform_gen_top;

architecture rtl of waveform_gen_top is

    ---------------------------------------------------------------------------
    -- Frequency Tuning Word (FTW) Lookup Table
    -- FTW = round(f_out * 2^32 / 50_000_000)
    -- 10 preset frequencies
    ---------------------------------------------------------------------------
    type ftw_array_type is array (0 to 9) of std_logic_vector(31 downto 0);
    constant FTW_TABLE : ftw_array_type := (
        x"00014F8B",   -- Level 0:  1 kHz
        x"00029F17",   -- Level 1:  2 kHz
        x"0003EEA2",   -- Level 2:  3 kHz
        x"00053E2D",   -- Level 3:  4 kHz
        x"00068DB9",   -- Level 4:  5 kHz
        x"0007DD44",   -- Level 5:  6 kHz
        x"000A7C5B",   -- Level 6:  8 kHz
        x"000D1B71",   -- Level 7: 10 kHz
        x"0013A92A",   -- Level 8: 15 kHz
        x"001A36E3"    -- Level 9: 20 kHz
    );

    ---------------------------------------------------------------------------
    -- Amplitude Presets (8 steps, 8-bit)
    ---------------------------------------------------------------------------
    type amp_array_type is array (0 to 7) of std_logic_vector(7 downto 0);
    constant AMP_TABLE : amp_array_type := (
        x"20",   -- Level 0: ~12.5% amplitude
        x"40",   -- Level 1: ~25%
        x"60",   -- Level 2: ~37.5%
        x"80",   -- Level 3: ~50%
        x"A0",   -- Level 4: ~62.5%
        x"C0",   -- Level 5: ~75%
        x"E0",   -- Level 6: ~87.5%
        x"FF"    -- Level 7: ~100% (full scale)
    );

    ---------------------------------------------------------------------------
    -- Internal Signals
    ---------------------------------------------------------------------------
    -- Debounced button pulses
    signal freq_up_pulse   : std_logic;
    signal freq_down_pulse : std_logic;
    signal amp_up_pulse    : std_logic;
    signal amp_down_pulse  : std_logic;

    -- Current selection indices
    signal freq_level : integer range 0 to 9;
    signal amp_level  : integer range 0 to 7;

    -- DDS core connections
    signal freq_word  : std_logic_vector(31 downto 0);
    signal wave_sel   : std_logic_vector(1 downto 0);
    signal amp_ctrl   : std_logic_vector(7 downto 0);
    signal dac_out_signed : std_logic_vector(11 downto 0);

    -- DAC conversion
    signal dac_signed_int  : signed(11 downto 0);
    signal dac_unsigned_12 : unsigned(11 downto 0);
    signal dac_unsigned_8  : unsigned(7 downto 0);

    -- DAC write strobe generation
    signal dac_wr_cnt : unsigned(2 downto 0);

begin

    ---------------------------------------------------------------------------
    -- Button Debouncers
    ---------------------------------------------------------------------------
    debounce_freq_up : entity work.key_debounce
        generic map (CLK_FREQ => 50_000_000, DEBOUNCE_MS => 20)
        port map (
            clk       => CLK_50M,
            rst_n     => RST_N,
            key_in    => KEY_FREQ_UP,
            key_pulse => freq_up_pulse,
            key_state => open
        );

    debounce_freq_down : entity work.key_debounce
        generic map (CLK_FREQ => 50_000_000, DEBOUNCE_MS => 20)
        port map (
            clk       => CLK_50M,
            rst_n     => RST_N,
            key_in    => KEY_FREQ_DOWN,
            key_pulse => freq_down_pulse,
            key_state => open
        );

    debounce_amp_up : entity work.key_debounce
        generic map (CLK_FREQ => 50_000_000, DEBOUNCE_MS => 20)
        port map (
            clk       => CLK_50M,
            rst_n     => RST_N,
            key_in    => KEY_AMP_UP,
            key_pulse => amp_up_pulse,
            key_state => open
        );

    debounce_amp_down : entity work.key_debounce
        generic map (CLK_FREQ => 50_000_000, DEBOUNCE_MS => 20)
        port map (
            clk       => CLK_50M,
            rst_n     => RST_N,
            key_in    => KEY_AMP_DOWN,
            key_pulse => amp_down_pulse,
            key_state => open
        );

    ---------------------------------------------------------------------------
    -- Frequency Level Control (button press cycles through presets)
    ---------------------------------------------------------------------------
    process(CLK_50M, RST_N)
    begin
        if RST_N = '0' then
            freq_level <= 7;   -- Default: 10 kHz (level 7)
        elsif rising_edge(CLK_50M) then
            if freq_up_pulse = '1' then
                if freq_level < 9 then
                    freq_level <= freq_level + 1;
                end if;
            elsif freq_down_pulse = '1' then
                if freq_level > 0 then
                    freq_level <= freq_level - 1;
                end if;
            end if;
        end if;
    end process;

    -- Lookup FTW from table
    freq_word <= FTW_TABLE(freq_level);

    ---------------------------------------------------------------------------
    -- Amplitude Level Control
    ---------------------------------------------------------------------------
    process(CLK_50M, RST_N)
    begin
        if RST_N = '0' then
            amp_level <= 7;   -- Default: full amplitude (level 7)
        elsif rising_edge(CLK_50M) then
            if amp_up_pulse = '1' then
                if amp_level < 7 then
                    amp_level <= amp_level + 1;
                end if;
            elsif amp_down_pulse = '1' then
                if amp_level > 0 then
                    amp_level <= amp_level - 1;
                end if;
            end if;
        end if;
    end process;

    -- Lookup amplitude from table
    amp_ctrl <= AMP_TABLE(amp_level);

    ---------------------------------------------------------------------------
    -- Waveform Selection (directly from switches)
    ---------------------------------------------------------------------------
    wave_sel <= SW_WAVE;

    ---------------------------------------------------------------------------
    -- DDS Waveform Generator Core
    ---------------------------------------------------------------------------
    dds_core : entity work.dds_waveform_gen
        generic map (
            PHASE_WIDTH    => 32,
            LUT_ADDR_WIDTH => 12,
            DATA_WIDTH     => 12,
            AMP_WIDTH      => 8
        )
        port map (
            clk       => CLK_50M,
            rst_n     => RST_N,
            freq_word => freq_word,
            wave_sel  => wave_sel,
            amp_ctrl  => amp_ctrl,
            dac_out   => dac_out_signed
        );

    ---------------------------------------------------------------------------
    -- DAC Output Conversion
    -- Internal: signed 12-bit (-2047 ~ +2047)
    -- DAC needs: unsigned 12-bit (0 ~ 4095) or unsigned 8-bit (0 ~ 255)
    --
    -- Conversion: unsigned = signed + 2048
    -- This maps: -2048 -> 0,  0 -> 2048,  +2047 -> 4095
    ---------------------------------------------------------------------------
    dac_signed_int  <= signed(dac_out_signed);
    dac_unsigned_12 <= unsigned(dac_signed_int + 2048);
    dac_unsigned_8  <= dac_unsigned_12(11 downto 4);   -- Take top 8 bits

    DAC_DATA    <= std_logic_vector(dac_unsigned_8);
    DAC_DATA_12 <= std_logic_vector(dac_unsigned_12);

    ---------------------------------------------------------------------------
    -- DAC Write Strobe Generation
    -- Generates a periodic active-low write pulse.
    -- For DAC0832: /WR should pulse low once per sample.
    -- For R-2R ladder or direct connection: can leave unconnected.
    --
    -- Strategy: Use a small counter to create a short low pulse every
    -- 8 clock cycles (6.25 MHz update rate, far above Nyquist for 20 kHz)
    ---------------------------------------------------------------------------
    process(CLK_50M, RST_N)
    begin
        if RST_N = '0' then
            dac_wr_cnt <= (others => '0');
        elsif rising_edge(clk_50m) then
            dac_wr_cnt <= dac_wr_cnt + 1;
        end if;
    end process;

    -- /WR is low when counter = "000" (1/8 duty cycle, ~160 ns low pulse)
    DAC_WR_N <= '0' when dac_wr_cnt = "000" else '1';

    ---------------------------------------------------------------------------
    -- LED Indicators
    ---------------------------------------------------------------------------
    LED_SINE <= '1' when wave_sel = "00" else '0';
    LED_TRI  <= '1' when wave_sel = "01" else '0';
    LED_SQR  <= '1' when wave_sel = "10" else '0';

    -- Show frequency level in binary on 4 LEDs
    LED_FREQ <= std_logic_vector(to_unsigned(freq_level, 4));

end architecture rtl;
