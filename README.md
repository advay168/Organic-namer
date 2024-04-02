# Organic-namer

## Features
- Create custom compounds by adding elements and bonds between them
- Name compounds
- Prettify compound graph with custom algorithm
- Save and Load compounds from disk

## Screenshots
- Naming `butane`:
![Butane](https://github.com/advay168/Organic-namer/assets/23453652/53d1552c-c4e7-488a-81db-76c199c42a06)

- Naming `3-methyl-2-methylbut-3-ene`:
![Unsaturated](https://github.com/advay168/Organic-namer/assets/23453652/b9d29196-4bc9-4119-8276-c706328a2ac6)

## High Level Implementation of Naming
1. Identify longest Carbon chain (coloured white)
2. identify functional group with highest priority
3. Name all remaining substituents as prefixes

## Cloning instructions
````git clone --recursive https://github.com/advay168/Organic-namer.git````

## Building instructions
* `mkdir build`
* `cd build`
* `cmake -G Ninja..`
* `ninja`
