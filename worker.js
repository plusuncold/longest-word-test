const { workerData, parentPort } = require('worker_threads')

// console.log('\n\nworkerThread')
const words = workerData.split(/\s/)
const longest = words.reduce((agg, word) => {
  return agg.length > word.length ? agg : word
}, '')
parentPort.postMessage(longest)
