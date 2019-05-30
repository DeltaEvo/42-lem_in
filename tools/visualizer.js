#!/usr/bin/env node
const parseMap = require("./lib/parse");
const fs = require("fs");
const path = require("path");

const ANT_SIZE = 0.7;
const ANT = fs.readFileSync(path.join(__dirname, "ant.svg"));
const arg = parseFloat(process.argv.slice(2));
const TIME_PER_TURN = isNaN(arg) ? 1 : arg;
const COLOR = "#8D6E63";
const START_COLOR = "#2196F3";
const END_COLOR = "#4CAF50";

function html(strings, ...keys) {
  const lastIndex = strings.length - 1;
  return (
    strings.slice(0, lastIndex).reduce((p, s, i) => p + s + keys[i], "") +
    strings[lastIndex]
  );
}

function readStdin() {
  return new Promise((resolve, reject) => {
    const chunks = [];
    process.stdin.setEncoding("utf8");
    process.stdin.on("data", chunk => chunks.push(chunk));
    process.stdin.once("error", reject);
    process.stdin.once("end", () => resolve(chunks.join("")));
  });
}

function dist({ x: x1, y: y1 }, { x: x2, y: y2 }) {
  return Math.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2);
}

function generateSvg({ ants, rooms, links, turns }) {
  const xs = Object.values(rooms).map(({ x }) => x);
  const ys = Object.values(rooms).map(({ y }) => y);

  const minX = Math.min(...xs);
  const maxX = Math.max(...xs);
  const minY = Math.min(...ys);
  const maxY = Math.max(...ys);

  const start = Object.values(rooms).find(room => room.start);

  const positions = Array.from({ length: ants }).map(_ => [start]);

  for (const [i, moves] of turns.entries()) {
    for (const move of moves) positions[move.ant - 1].push(rooms[move.room]);
    for (const position of positions)
      if (position.length != i + 2) position.push(position.slice(-1)[0]);
  }

  process.stdout.write(html`
    <svg
      xmlns="http://www.w3.org/2000/svg"
      viewBox="${minX - 0.25} ${minY - 0.25} ${maxX - minX + 0.5} ${maxY -
        minY +
        0.5}"
    >
      <defs>
        <image
          id="ant"
          x="-${ANT_SIZE / 2}"
          y="-${ANT_SIZE / 2}"
          href="data:image/svg+xml;base64,${ANT.toString("base64")}"
          width="${ANT_SIZE}"
          height="${ANT_SIZE}"
          style="transform: rotateX(180deg)"
        />
      </defs>
      ${links
        .map(
          link => html`
            <path
              id="${link[0].name}-${link[1].name}"
              d="M${link[0].x} ${link[0].y} L${link[1].x} ${link[1].y}"
              fill="none"
              stroke="${COLOR}"
              stroke-width="0.05"
            />
          `
        )
        .join("")}
      ${Object.values(rooms)
        .map(
          room => html`
            <circle
              cx="${room.x}"
              cy="${room.y}"
              stroke="${room.start
                ? START_COLOR
                : room.end
                ? END_COLOR
                : COLOR}"
              stroke-width="0.025"
              r="0.20"
              fill="white"
            />
          `
        )
        .join("")}
      ${Object.values(rooms)
        .map(
          room => html`
            <text
              x="${room.x}"
              y="${room.y + 0.05}"
              font-size="0.2"
              fill="${room.start ? START_COLOR : room.end ? END_COLOR : COLOR}"
              text-anchor="middle"
              >${room.name}</text
            >
          `
        )
        .join("")}
      ${Object.values(positions)
        .map(position => {
          const distances = position.map((v, i, a) =>
            i ? dist(v, a[i - 1]) : 0
          );
          const totalDist = distances.reduce((c, v) => c + v, 0);
          return html`
            <use href="#ant">
              <animateMotion
                dur="${position.length * TIME_PER_TURN}s"
                rotate="auto"
                calcMode="linear"
                fill="freeze"
                keyTimes="${position
                  .map((_, i, { length }) => i / length)
                  .join(";")};1"
                keyPoints="${position
                  .map(
                    (_, i) =>
                      distances.slice(0, i + 1).reduce((c, v) => c + v, 0) /
                      totalDist
                  )
                  .join(";")};1"
                path="${position
                  .map((p, i) => `${i ? "L" : "M"}${p.x} ${p.y}`)
                  .join(" ")}"
              />
            </use>
          `;
        })
        .join("")}
    </svg>
  `);
}

readStdin()
  .then(parseMap)
  .then(generateSvg)
  .catch(console.error);
