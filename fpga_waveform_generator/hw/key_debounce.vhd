----------------------------------------------------------------------------
-- Module:      key_debounce
-- Description: Button debouncer with edge detection.
--              Filters mechanical switch bounce (20 ms debounce window).
--              Outputs a single-clock-cycle pulse on each valid press.
--
-- Inputs:
--   clk     : System clock (50 MHz)
--   rst_n   : Active-low asynchronous reset
--   key_in  : Raw button input (active-low, directly from physical pin)
--
-- Outputs:
--   key_pulse : Single clock-cycle pulse on each valid press (active-high)
--   key_state : Debounced steady state (active-high = pressed)
--
-- Author:      Auto-generated for FPGA Arbitrary Waveform Generator
-- Target:      Quartus 18.0 / Cyclone IV (VHDL-93)
----------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity key_debounce is
    generic (
        CLK_FREQ   : integer := 50_000_000;   -- System clock frequency (Hz)
        DEBOUNCE_MS: integer := 20             -- Debounce time (milliseconds)
    );
    port (
        clk       : in  std_logic;
        rst_n     : in  std_logic;
        key_in    : in  std_logic;             -- Raw input (active-low from button)
        key_pulse : out std_logic;             -- Single pulse on press
        key_state : out std_logic              -- Debounced state (high = pressed)
    );
end entity key_debounce;

architecture rtl of key_debounce is

    constant CNT_MAX : integer := CLK_FREQ / 1000 * DEBOUNCE_MS - 1;

    signal cnt       : integer range 0 to CNT_MAX;
    signal key_reg   : std_logic;   -- Synchronized input
    signal key_reg2  : std_logic;   -- Double-register for metastability
    signal key_deb   : std_logic;   -- Debounced state
    signal key_deb_d : std_logic;   -- Delayed debounced state (for edge detection)

begin

    -- Double-register synchronizer (metastability protection)
    process(clk, rst_n)
    begin
        if rst_n = '0' then
            key_reg  <= '1';   -- Button not pressed (active-low)
            key_reg2 <= '1';
        elsif rising_edge(clk) then
            key_reg  <= key_in;
            key_reg2 <= key_reg;
        end if;
    end process;

    -- Debounce counter
    process(clk, rst_n)
    begin
        if rst_n = '0' then
            cnt     <= 0;
            key_deb <= '1';
        elsif rising_edge(clk) then
            if key_reg2 /= key_deb then
                -- Input changed, start counting
                if cnt = CNT_MAX then
                    key_deb <= key_reg2;
                    cnt     <= 0;
                else
                    cnt <= cnt + 1;
                end if;
            else
                -- Input stable, reset counter
                cnt <= 0;
            end if;
        end if;
    end process;

    -- Edge detection (falling edge of debounced signal = button press)
    process(clk, rst_n)
    begin
        if rst_n = '0' then
            key_deb_d <= '1';
        elsif rising_edge(clk) then
            key_deb_d <= key_deb;
        end if;
    end process;

    -- Output: single pulse when button is pressed (active-low -> falling edge)
    key_pulse <= key_deb_d and (not key_deb);

    -- Output: debounced steady state (inverted: high = pressed)
    key_state <= not key_deb;

end architecture rtl;
