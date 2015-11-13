This repository contains a minimal system with a master tx device which, for now, puts a sequence of integers into consecutive addresses in memory.

There are several things to fix right now:
- check that all files required by Vivado are indeed into the repo, as the magical gui makes things behind the scene that I have no idea of
- slave register addresses are inheritedf from an other project and must be changed to be more reasonnable
- the 'sample.vhd' file contains a basic counter for checking the system, here is where the sampling should take place
