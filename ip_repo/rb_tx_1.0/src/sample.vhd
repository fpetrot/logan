-- vim:ts=3:noexpandtab:
-- For now a simple counter to produce deterministic values !

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity sample is
	port (
		clk				: in std_logic;
		resetn			: in std_logic;
		fifo_out_data	: out std_logic_vector(31 downto 0);
		fifo_out_rdy	: out std_logic;
		fifo_out_ack	: in std_logic
	);
end sample;

architecture behavior of sample is

	signal reg_cnt			: std_logic_vector(31 downto 0) := (others => '0');

begin

	process (clk)
	begin
		if rising_edge(clk) then
			if resetn = '0' then
				reg_cnt <= (others => '0');
			else 
			   if fifo_out_ack = '1' then
					reg_cnt <= std_logic_vector(unsigned(reg_cnt) + 1);
				end if;
			end if;
		end if;
	end process;


	fifo_out_rdy <= '1';
	fifo_out_data <= reg_cnt;

end behavior;
