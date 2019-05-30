const child_process = require('child_process')
const stream = require('stream')
const parseMap = require("./lib/parse");
const prompts = require('prompts');


function mapFromGenerator(arg) {
	const { stdout } = child_process.spawn('./generator', [arg], {
		stdio: ['ignore', 'pipe', 'ignore']
	});
	return stdout;
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
  

function runProgram(input) {
	const { stdout, stdin } = child_process.spawn('./lem-in', {
		stdio: ['pipe', 'pipe', 'ignore']
	})
	input.pipe(stdin)
	return (stdout)
}

function compareArrays(a1, a2, cmp = (a, b) => a === b) {
	if (a1.length !== a2.length)
		return false;
	return a1.every((e, i) => cmp(a2[i], e))
}

function checkAnthil({
	links: oLinks, rooms: oRooms, ants: oAnts
}, {
	links: sLinks, rooms: sRooms, ants: sAnts
}) {
	if (oAnts !== sAnts)
		throw "Ants number is not the same"
	if (!compareArrays(sLinks.map(e => e.sort()).sort(), oLinks.map(e => e.sort()).sort(), (a, b) => a.name === b.name))
		throw "Links are differents"
	if (!compareArrays(Object.values(oRooms), Object.values(sRooms), (a, b) => {
		return a.name === b.name && a.x === b.x && a.y === b.y && a.start === b.start && a.end === b.end
	}))
		throw "Rooms are differents"
}

async function check(stream) {
	const [reference, forProgram] = cloneStream(stream);
	const output = runProgram(forProgram);
	const [
		source,
		solved
	] = await Promise.all([
		reference,
		output
	].map(readStream))

	const parsed = parseMap(solved);

	checkAnthil(parsed, parseMap(source))

	console.log(source.slice(source.slice(0, -1).lastIndexOf('\n'), -1))
}

(async () => {
	const { mode } = await prompts({
		type: 'select',
		name: 'mode',
		message: 'Mode',
		choices: [
			'generator',
			'files'
		]
	});

	if (mode === 0) {
		const flags = ["--flow-one", "--flow-ten", "--flow-thousand", "--big", "--big-superposition"]
		const { flag } = await prompts({
			type: 'select',
			name: 'flag',
			message: 'Generator flag',
			choices: flags
		})
		const { runs } = await prompts({
			type: 'number',
			name: 'runs',
			message: 'Number of runs',
			initial: 10
		})
		console.log(runs)
		const results = await Promise.all(Array.from({ length: runs }).map(() => {
			return check(mapFromGenerator(flag))
		}))
	}
	else if (mode === 1) {
		console.log(await check(require('fs').createReadStream("map/2.map")))
	}
})();