# Quine McCluskey method
Implementation of Quine McCluskey method in C. This method is used to minimize a Boolean function: given its implicants, the method returns a set of prime implicants that is guaranteed to cover all given minterms (but doesn't guarantee that the set is minimum). If you want to know more you can visit the [Wikipedia page](https://en.wikipedia.org/wiki/Quine%E2%80%93McCluskey_algorithm).

## Usage
- compile using `make` command
- create two text file: one will contain the ON set minterms, the other the DC set minterms
- to run the executable you must specify 3 parameters in this order:
    - `on_set_filename` name of text file from which to read minterms belonging to the ON set 
    - `dc_set_filename` name of text file from which to read minterms belonging to the DC set
    - `nOfVariables` number of variables to consider in the expansion (MAX 15 variables)
