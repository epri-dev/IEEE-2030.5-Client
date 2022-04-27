//https://stackoverflow.com/questions/18664530/how-to-create-a-simple-socket-in-node-js
// server
require('net').createServer(function (socket) {
    console.log("connected");

    //this receives the der control
    socket.on('data', function (data) {
        console.log(data.toString());
    });
    //socket.connect(8080);
    //socket.write('Server send monitor data plain txt');
    //socket.write('Server send monitor data plain txt 2');
    
    //at poll rate specified by the utility
    var msg='Server send monitor data plain txt';
    var conversion=1000;
    var pollRate=120*conversion; // 1 s * (1000 ms / 1s) [vvv]
    setInterval(function() {
         socket.write(msg)
    },pollRate);
        //poll resources from the EMS
        
        //socket.write( the ems resources)
})

.listen(8080);

// client
/*
var s = require('net').Socket();
s.connect(8080);
s.write('Hello');
s.end();*/