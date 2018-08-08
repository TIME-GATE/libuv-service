const net = require('net')

const HOST = '127.0.0.1'
const PORT = 7000

const  client = new net.Socket()

client.connect(PORT, HOST, () => {

    console.log('CONNECTED TO: ' + HOST + ':' + PORT);
    
    setInterval(() => {
      client.write('jessica')
    }, 1000)

});

client.on('data', function(data) {

    console.log('DATA: ' + data)

})

client.on('end', function(data) {

    console.log('end: ' + data)

})

client.on('close', function(data) {

    console.log('close: ' + data)

})


process.on('exit', () => {
  
  client.destroy() 
  
  client.on('close', function() {
    console.log('Connection closed')
  })

})
