#!/usr/bin/env node
const child_process = require("child_process");
const stream = require("stream");
const parseMap = require("./lib/parse");
const prompts = require("prompts");
const ProgressBar = require("progress");
const chalk = require("chalk");
const fs = require("fs");

function spawn(name, args, options) {
  return new Promise((resolve, reject) => {
    const process = child_process.spawn(name, args, options);
    process.once("error", reject);
    if (process.stdout)
      resolve(process);
  });
}

const delay = ms => new Promise(resolve => setTimeout(resolve, delay))

function spawnRetry(name, args, options) {
  return spawn(name, args, options).catch(e => {
    if (e.code === "EAGAIN")
      return delay(1000).then(() => spawnRetry(name, args, options))
    return Promise.reject(e);
  })
}

async function mapFromGenerator(arg, offset) {
  const { stdout } = await spawnRetry("./generator", [arg], {
    stdio: ["ignore", "pipe", "inherit"],
    env: {
      DYLD_FORCE_FLAT_NAMESPACE: 1,
      DYLD_INSERT_LIBRARIES:
        "/Users/dde-jesu/.brew/Cellar/libfaketime/0.9.7_1/lib/faketime/libfaketime.1.dylib",
      FAKETIME: `+${offset}s`
    }
  });
  return stdout
}

function cloneStream(source) {
  const sink1 = new stream.PassThrough();
  const sink2 = new stream.PassThrough();
  source.pipe(sink1);
  source.pipe(sink2);

  return [sink1, sink2];
}

function readStream(stream) {
  return new Promise((resolve, reject) => {
    const chunks = [];
    stream.setEncoding("utf8");
    stream.on("data", chunk => chunks.push(chunk));
    stream.once("error", reject);
    stream.once("end", () => resolve(chunks.join("")));
  });
}

async function runProgram(input) {
  const { stdout, stdin } = await spawnRetry("./lem-in", {
    stdio: ["pipe", "pipe", "ignore"]
  });
  input.pipe(stdin);
  return stdout;
}

function compareArrays(a1, a2, cmp = (a, b) => a === b) {
  if (a1.length !== a2.length) return false;
  return a1.every((e, i) => cmp(a2[i], e));
}

function checkAnthil(
  { links: oLinks, rooms: oRooms, ants: oAnts },
  { links: sLinks, rooms: sRooms, ants: sAnts }
) {
  if (oAnts !== sAnts) throw "Ants number is not the same";
  if (
    !compareArrays(
      sLinks.map(e => e.sort()).sort(),
      oLinks.map(e => e.sort()).sort(),
      (a, b) => a.name === b.name
    )
  )
    throw "Links are differents";
  if (
    !compareArrays(Object.values(oRooms), Object.values(sRooms), (a, b) => {
      return (
        a.name === b.name &&
        a.x === b.x &&
        a.y === b.y &&
        a.start === b.start &&
        a.end === b.end
      );
    })
  )
    throw "Rooms are differents";
}

async function check(stream) {
  const [reference, forProgram] = cloneStream(stream);
  const output = await runProgram(forProgram);
  const [source, solved] = await Promise.all(
    [reference, output].map(readStream)
  );

  const parsed = parseMap(solved);

  checkAnthil(parsed, parseMap(source));

  const lastLine = source.slice(source.slice(0, -1).lastIndexOf("\n") + 1, -1);
  const [, required] =
    /^#Here is the number of lines required: (\d+)$/.exec(lastLine) || [];

  return parsed.turns.length - (required || 0);
}

(async () => {
  const { mode } = await prompts({
    type: "select",
    name: "mode",
    message: "Mode",
    choices: ["generator", "file"]
  });

  if (mode === 0) {
    const { flag } = await prompts({
      type: "select",
      name: "flag",
      message: "Generator flag",
      choices: [
        {
          title: "Flow One",
          value: "--flow-one"
        },
        {
          title: "Flow Ten",
          value: "--flow-ten"
        },
        {
          title: "Flow Thousand",
          value: "--flow-thousand"
        },
        {
          title: "Big",
          value: "--big"
        },
        {
          title: "Big Superposition",
          value: "--big-superposition"
        }
      ]
    });
    if (!flag) return;
    const { runs } = await prompts({
      type: "number",
      name: "runs",
      message: "Number of runs",
      initial: 10
    });
    if (!runs) return;
    const bar = new ProgressBar(
      `[${chalk.magenta(":bar")}] eta: ${chalk.blueBright(":eta")}s`,
      { total: runs }
    );
    bar.render(undefined, true);
    const results = (await Promise.all(
      Array.from({ length: runs }).map(async (_, i) => {
        const p = check(await mapFromGenerator(flag, i));
        p.then(() => bar.tick());
        return p;
      })
    )).sort((a, b) => a - b);
    console.log("Results: ", results.join(", "));
    console.log("Min:", Math.min(...results));
    console.log("Max:", Math.max(...results));
    console.log(
      "Moyenne:",
      results.reduce((c, v) => c + v, 0) / results.length
    );
  } else if (mode === 1) {
    const { file } = await prompts({
      type: 'text',
      name: 'file',
      message: `File path`
    })
    if (!file)
      return
    if (!fs.existsSync(file))
      console.error("File not found:", file)
    else
      console.log(await check(fs.createReadStream(file)));
  }
})();
