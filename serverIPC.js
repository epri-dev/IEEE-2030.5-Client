//https://stackoverflow.com/questions/18664530/how-to-create-a-simple-socket-in-node-js
// server
require('net').createServer(function (socket) {
    console.log("connected");

    socket.on('data', function (data) {
        console.log(data.toString());
    });
    //socket.connect(8080);
    socket.write('Server accepted');
})

.listen(8080);

// client
/*
var s = require('net').Socket();
s.connect(8080);
s.write('Hello');
s.end();*/