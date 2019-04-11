const fs = require('fs')
const { Worker } = require('worker_threads')
const start = Date.now()

let proms = []

const readStream = fs.createReadStream('corpus.txt', 'utf8')
let buffer = ''
let i = 0

readStream.on('data', function(chunk) {
  // chunk.split(/\s/).forEach(w => {
  //   if (w.length > longest.length) longest = w
  // })
  buffer += chunk
  if (i % 10000 === 0) {
    proms.push(runService(buffer))
    buffer = ''
  }
  i++
}).on('end', doRollup)

function doRollup () {
  Promise.all(proms).then(res => {
    console.log("Threads: ", res.length)
    const longest = res.reduce((agg, word) => {
      return agg.length > word.length ? agg : word
    }, '')
    console.log(longest, longest.length, Date.now() - start + 'ms');
  })
}

function runService(workerData) {
  return new Promise((resolve, reject) => {
    const worker = new Worker('./worker.js', { workerData });
    worker.on('message', resolve);
    worker.on('error', reject);
    worker.on('exit', (code) => {
      if (code !== 0)
        reject(new Error(`Worker stopped with exit code ${code}`));
    })
  })
}
