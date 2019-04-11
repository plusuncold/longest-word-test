const fs = require('fs')
const start = Date.now()

let longest = ''

const readStream = fs.createReadStream('corpus.txt', 'utf8')

readStream.on('data', function(chunk) {  
    chunk.split(/\s/).forEach(w => {
      if (w.length > longest.length) longest = w
    })
}).on('end', function() {
    console.log(longest, longest.length, Date.now() - start + 'ms');
});