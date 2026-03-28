----------------------------------------------------------------------------
-- Module:      dds_waveform_gen
-- Description: DDS-based Arbitrary Waveform Generator (Top Level)
--              Generates sine, triangle, and square waves.
--              - 32-bit phase accumulator for fine frequency resolution
--              - 12-bit DAC output (signed, range -2047 to +2047)
--              - 8-bit amplitude control (0 = mute, 255 = full scale)
--              - 2-bit wave selection: "00"=sine, "01"=triangle, "10"=square
--
-- Frequency Tuning Word (FTW) calculation:
--   FTW = round(f_out * 2^32 / f_clk)
--   For f_clk = 50 MHz:
--     1 kHz  -> FTW = 85899     (0x00014F8B)
--     10 kHz -> FTW = 858993    (0x000D1B71)
--     20 kHz -> FTW = 1717987   (0x001A36E3)
--
-- Physical voltage mapping (with external DAC):
--   DAC output +2047 -> +10V
--   DAC output     0 ->   0V
--   DAC output -2047 -> -10V
--
-- Author:      Auto-generated for FPGA Arbitrary Waveform Generator
-- Target:      Quartus 18.0 / ModelSim 10.4, VHDL-93
----------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity dds_waveform_gen is
    generic (
        PHASE_WIDTH    : integer := 32;   -- Phase accumulator width
        LUT_ADDR_WIDTH : integer := 12;   -- Sine LUT address width
        DATA_WIDTH     : integer := 12;   -- DAC output width (signed)
        AMP_WIDTH      : integer := 8     -- Amplitude control width
    );
    port (
        clk       : in  std_logic;                                    -- System clock (50 MHz)
        rst_n     : in  std_logic;                                    -- Active-low reset
        freq_word : in  std_logic_vector(PHASE_WIDTH-1 downto 0);     -- Frequency tuning word
        wave_sel  : in  std_logic_vector(1 downto 0);                 -- Waveform: "00"=sine, "01"=triangle, "10"=square
        amp_ctrl  : in  std_logic_vector(AMP_WIDTH-1 downto 0);       -- Amplitude: 0~255
        dac_out   : out std_logic_vector(DATA_WIDTH-1 downto 0)       -- DAC output (signed)
    );
end entity dds_waveform_gen;

architecture rtl of dds_waveform_gen is

    -- Phase accumulator
    signal phase_acc : unsigned(PHASE_WIDTH-1 downto 0);

    -- Sine LUT signals
    signal lut_addr  : std_logic_vector(LUT_ADDR_WIDTH-1 downto 0);
    signal sine_data : std_logic_vector(DATA_WIDTH-1 downto 0);

    -- Triangle wave signals
    signal tri_msb       : std_logic;
    signal tri_phase_half: unsigned(LUT_ADDR_WIDTH-2 downto 0);
    signal tri_data      : signed(DATA_WIDTH-1 downto 0);

    -- Square wave signal
    signal sqr_data : signed(DATA_WIDTH-1 downto 0);

    -- Waveform selection and amplitude scaling
    signal wave_data  : signed(DATA_WIDTH-1 downto 0);
    signal amp_signed : signed(AMP_WIDTH downto 0);
    signal scaled     : signed(DATA_WIDTH + AMP_WIDTH downto 0);

begin

    ---------------------------------------------------------------------------
    -- Phase Accumulator
    -- Increments by freq_word every clock cycle.
    -- Output frequency = freq_word * f_clk / 2^PHASE_WIDTH
    ---------------------------------------------------------------------------
    process(clk, rst_n)
    begin
        if rst_n = '0' then
            phase_acc <= (others => '0');
        elsif rising_edge(clk) then
            phase_acc <= phase_acc + unsigned(freq_word);
        end if;
    end process;

    ---------------------------------------------------------------------------
    -- Sine Wave Generation (via LUT)
    -- Use top LUT_ADDR_WIDTH bits of phase accumulator as ROM address
    ---------------------------------------------------------------------------
    lut_addr <= std_logic_vector(
                    phase_acc(PHASE_WIDTH-1 downto PHASE_WIDTH-LUT_ADDR_WIDTH)
                );

    sine_lut_inst : entity work.sine_lut
        generic map (
            ADDR_WIDTH => LUT_ADDR_WIDTH,
            DATA_WIDTH => DATA_WIDTH
        )
        port map (
            clk      => clk,
            addr     => lut_addr,
            data_out => sine_data
        );

    ---------------------------------------------------------------------------
    -- Triangle Wave Generation (from phase accumulator)
    -- First half  (MSB=0): output ramps from -peak to +peak
    -- Second half (MSB=1): output ramps from +peak to -peak
    ---------------------------------------------------------------------------
    tri_msb        <= std_logic(phase_acc(PHASE_WIDTH-1));
    tri_phase_half <= phase_acc(PHASE_WIDTH-2 downto PHASE_WIDTH-LUT_ADDR_WIDTH);

    process(tri_msb, tri_phase_half)
        variable half_val : integer;
        variable max_val  : integer;
    begin
        max_val  := 2**(DATA_WIDTH-1) - 1;   -- 2047
        half_val := to_integer(tri_phase_half);
        if tri_msb = '0' then
            -- Rising: -2047 + 2*half_val  (0->-2047, 2047->+2047)
            tri_data <= to_signed(-max_val + 2 * half_val, DATA_WIDTH);
        else
            -- Falling: +2047 - 2*half_val (0->+2047, 2047->-2047)
            tri_data <= to_signed(max_val - 2 * half_val, DATA_WIDTH);
        end if;
    end process;

    ---------------------------------------------------------------------------
    -- Square Wave Generation (from phase accumulator MSB)
    -- MSB=1 -> +peak,  MSB=0 -> -peak
    ---------------------------------------------------------------------------
    process(phase_acc)
        variable max_val : integer;
    begin
        max_val := 2**(DATA_WIDTH-1) - 1;   -- 2047
        if phase_acc(PHASE_WIDTH-1) = '1' then
            sqr_data <= to_signed(max_val, DATA_WIDTH);
        else
            sqr_data <= to_signed(-max_val, DATA_WIDTH);
        end if;
    end process;

    ---------------------------------------------------------------------------
    -- Waveform Multiplexer
    -- "00" = Sine,  "01" = Triangle,  "10" = Square
    ---------------------------------------------------------------------------
    process(wave_sel, sine_data, tri_data, sqr_data)
    begin
        case wave_sel is
            when "00"   => wave_data <= signed(sine_data);    -- Sine
            when "01"   => wave_data <= tri_data;             -- Triangle
            when "10"   => wave_data <= sqr_data;             -- Square
            when others => wave_data <= (others => '0');      -- Default: zero
        end case;
    end process;

    ---------------------------------------------------------------------------
    -- Amplitude Scaling
    -- output = wave_data * amp_ctrl / 256
    -- amp_ctrl=255 gives ~99.6% of full scale (negligible loss)
    -- amp_ctrl=128 gives ~50% of full scale
    -- amp_ctrl=0   gives zero output
    ---------------------------------------------------------------------------
    amp_signed <= signed('0' & amp_ctrl);                        -- 9-bit signed (always positive)
    scaled     <= wave_data * amp_signed;                        -- 21-bit signed product
    dac_out    <= std_logic_vector(
                      scaled(DATA_WIDTH + AMP_WIDTH - 1 downto AMP_WIDTH)
                  );                                             -- Take upper 12 bits (divide by 256)

end architecture rtl;
