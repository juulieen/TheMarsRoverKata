"use strict";
const fs = require("fs");

function load_input() {
  try {
    const buffer = fs.readFileSync(0);
    const input = buffer.toString();
    return input;
  } catch (error) {
    console.error("Can't read from the stdin (fd: 0)", error);
    process.exit(1);
  }
}

function move(o, y, x, Y, X) {
  switch (o) {
    case 0:
      y = (y + 1) % Y;
      break;
    case 1:
      x = (x + 1) % X;
      break;
    case 2:
      y = (y + Y - 1) % Y;
      break;
    case 3:
      x = (x + X - 1) % X;
      break;

    default:
      console.error("Should not happen");
      process.exit(1);
  }
  return { y, x };
}

function start() {
  const input = load_input();
  let [firstLine, ...RoverLines] = input.split("\n");
  const [X, Y] = firstLine.split(' ').map(nb => parseInt(nb, 10) + 1)
  const orientations = ["N", "E", "S", "W"];
  do {
    RoverLines = nextRoverLines ?? RoverLines;
    var [position, actions, ...nextRoverLines] = RoverLines;
    let [x, y, o] = position.split(" ");
    x = parseInt(x, 10);
    y = parseInt(y, 10);
    o = orientations.findIndex((p) => p === o);
    // Apply each action to the rover
    Array.from(actions).forEach((action) => {
      if (action === "L")
        o = (o + orientations.length - 1) % orientations.length;
      else if (action === "R") o = (o + 1) % orientations.length;
      else if (action === "M") ({ y, x } = move(o, y, x, Y, X));
    });
    console.log(x, y, orientations[o]);
  } while (nextRoverLines.length);
}

start();
