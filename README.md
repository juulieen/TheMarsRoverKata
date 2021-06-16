# The Mars Rover Kata

The exercice was for me to do the Kata described [here](./MarsRoverKata.md).

## Simple Script

A node js script that do the job but isn't tested at all.

You can try it as follow:

```shell
cat samples/input_example.txt | node easy_solution_script/script.js
```

## C Version

Compile and Run

```shell
make
cat samples/input_example.txt | ./TheMarsRover
```

Run the Tests

> You will need to install the [Criterion Librarie](https://github.com/Snaipe/Criterion)

```shell
make tests_run
```
