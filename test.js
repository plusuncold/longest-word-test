const fs = require('fs')

fs.readFile('corpus.txt', 'utf8', (err, data) => {

  if (data) console.log('data read')

  const start = Date.now()

  const words = data.split(/\s/)

  const longest = words.reduce((agg, word) => {

    return agg.length > word.length ? agg : word

  }, '')

  console.log(longest, longest.length, Date.now() - start)

})
