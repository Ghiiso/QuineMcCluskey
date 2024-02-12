# Quine McCluskey/Petrick method implementation
Implementation of Quine McCluskey and Petrick method in C language. This method is used to minimize a Boolean function: given its implicants, the method returns a set of prime implicants that is guaranteed to cover all given minterms (but doesn't guarantee that the set is minimum). Given this list, Petrick method proceeds to then isolate only the essential ones. In this way the final result is guaranteed to contain the best implicants possible. The program returns both the list of essential implicants and the PoS form of the function. 

If you want to know more about the Quine McCluskey method you can visit the relative [Wikipedia page](https://en.wikipedia.org/wiki/Quine%E2%80%93McCluskey_algorithm).

If you want to know more about the Petrick method you can visit the relative [Wikipedia page](https://en.wikipedia.org/wiki/Petrick%27s_method).

> [!NOTE]
> Not completely tested: some edge cases might be wrong

## Usage
- compile using `make` command
- create two text file: one will contain the ON set minterms, the other the DC set minterms
- the executable needs 3 parameters in this order:
    - `on_set_filename` name of text file from which to read minterms belonging to the ON set 
    - `dc_set_filename` name of text file from which to read minterms belonging to the DC set
    - `nOfVariables` number of variables to consider in the expansion (MAX 15 variables)
- you can change the displayed letters by changing or rearranging the content of `static const char variables[]` array at the top of [main file](main.c): be sure to leave at least 15 characters to avoid problems. 
## Future updates
- more variables
