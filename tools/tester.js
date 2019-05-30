const child_process = require('child_process')
const stream = require('stream')

function mapFromGenerator(arg) {
	const { stdio } = child_process.spawn('./generator', [arg]);
	return stdio;
}

function cloneStream(source) {
	const sink1 = new stream.PassThrough();
	const sink2 = new stream.PassThrough();
	source.pipe(sink1);
	source.pipe(sink2);

	return [sink1, sink2];
}
