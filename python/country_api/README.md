When you run `$: python3 get_country_data.py`, it will download each country’s cities and store them in a dedicated file named by the country code, e.g. `AU.txt` for Australia, `LB.txt` for Lebanon etc.

These files will all live in a folder called data, which you will need to create first. You can use the helper functions defined in the Python module to do customised API calls in the interpreter.

To make a master list from all these country files, simply use the `cat` shell command (on Linux): `$: cat *.txt > country_master_list.txt`
