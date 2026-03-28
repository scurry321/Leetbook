----------------------------------------------------------------------------
-- Module:      sine_lut
-- Description: Sine Wave Lookup Table (ROM)
--              Uses IEEE MATH_REAL to compute sine values at elaboration time.
--              4096 entries, 12-bit signed output, range -2047 to +2047.
--              Designed for DDS (Direct Digital Synthesis) waveform generation.
-- Author:      Auto-generated for FPGA Arbitrary Waveform Generator
-- Target:      Quartus 18.0 / ModelSim 10.4, VHDL-93
----------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.math_real.all;

entity sine_lut is
    generic (
        ADDR_WIDTH : integer := 12;   -- 2^12 = 4096 entries
        DATA_WIDTH : integer := 12    -- 12-bit signed output
    );
    port (
        clk      : in  std_logic;
        addr     : in  std_logic_vector(ADDR_WIDTH-1 downto 0);
        data_out : out std_logic_vector(DATA_WIDTH-1 downto 0)
    );
end entity sine_lut;

architecture rtl of sine_lut is

    constant ROM_DEPTH   : integer := 2**ADDR_WIDTH;               -- 4096
    constant MAX_AMP     : integer := 2**(DATA_WIDTH-1) - 1;       -- 2047

    type rom_type is array (0 to ROM_DEPTH - 1)
                     of std_logic_vector(DATA_WIDTH-1 downto 0);

    -- Initialize ROM with one full period of sine wave at elaboration time
    function init_sine_rom return rom_type is
        variable rom_v       : rom_type;
        variable angle       : real;
        variable sin_val     : real;
        variable int_val     : integer;
    begin
        for i in 0 to ROM_DEPTH - 1 loop
            angle   := real(i) * MATH_2_PI / real(ROM_DEPTH);
            sin_val := sin(angle);
            int_val := integer(sin_val * real(MAX_AMP));
            -- Clamp to valid signed range to avoid overflow
            if int_val > MAX_AMP then
                int_val := MAX_AMP;
            elsif int_val < -MAX_AMP then
                int_val := -MAX_AMP;
            end if;
            rom_v(i) := std_logic_vector(to_signed(int_val, DATA_WIDTH));
        end loop;
        return rom_v;
    end function init_sine_rom;

    constant SINE_ROM : rom_type := init_sine_rom;

begin

    -- Synchronous ROM read (single clock latency)
    process(clk)
    begin
        if rising_edge(clk) then
            data_out <= SINE_ROM(to_integer(unsigned(addr)));
        end if;
    end process;

end architecture rtl;
