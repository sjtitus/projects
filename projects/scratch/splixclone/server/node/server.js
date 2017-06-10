/*_____________________________________________________________________________
 * SplixClone Server-Side Application
 * A Node/express backend for the splixclone game.
 * ____________________________________________________________________________
*/

// Included modules
var express         = require('express');
var bodyParser      = require('body-parser');       // body-parser will let us get parameters from our POST requests
var methodOverride  = require('method-override');   // simulate DELETE and PUT (express4)
var morgan          = require('morgan')
var fs              = require('fs');
var path            = require('path');

// My modules 
var db              = require('./libs/database.js');

var app             = express();

// request logging 
console.log('SplixClone Node: starting dir: '+__dirname);
var accessLogStream = fs.createWriteStream(path.join(__dirname, 'access.log'), {flags: 'a'})
app.use(morgan('dev', {stream: accessLogStream}))

// configure body parser 
app.use(bodyParser.urlencoded({'extended':'true'}));            // parse application/x-www-form-urlencoded
app.use(bodyParser.json());                                     // parse application/json
app.use(bodyParser.json({ type: 'application/vnd.api+json' })); // parse application/vnd.api+json as json

// configure method-override
app.use(methodOverride());

// Routes
var apiRoutes   = require('./app/controllers/apiController');


// test the db
/*
db.connectionPool.getConnection(
    function(err, connection) {
        if (err) {
            throw err;
        }
        else {
            connection.query('SELECT 1 + 1 AS solution', function (error, results, fields) {
                if (error) throw error;
                    console.log('The solution is: ', results[0].solution);
                });
        } 
    }
);
*/


// serve static files
app.use(express.static(__dirname + '/public')); 

// Set up routing
app.use('/api',apiRoutes);
 

// start listening for connections 
console.log('SplixClone Node: listening: http://localhost:80');
app.listen(80);




/*
var net = require('net');
var http = require('http');
var url = require('url');
var WebSocketServer = require('ws').Server;
var express = require('express');
var app = express();
var port = 80;


console.log('Creating HTTP server');
var server = http.createServer();
var wss = new WebSocketServer({ server: server });

// Default HTTP response
app.use(function (req, res) {
  res.send({ msg: "hello" });
});


// WebSocket Handling
function echo(ws, payload) {
  console.log('echo payload: '+payload);
  ws.send(JSON.stringify({type: "echo", payload: payload}));
}

function broadcast(ws, payload) {
  console.log('broadcast payload: '+payload);
  var msg = JSON.stringify({type: "broadcast", payload: payload});
  wss.clients.forEach(function each(client) {
    client.send(msg);
  });
  ws.send(JSON.stringify({type: "broadcastResult", payload: payload}));
}




wss.on('connection', 
    function connection(ws) 
    {
        console.log('creating new local socket to comm with game server');
        
        var COMMAND_PATH = '/tmp/splixcommand'; 
        var client = new net.Socket();
        client.setEncoding('utf8');
        client.on('data', function(data) {
            console.log('received data: ' + data);
            console.log('received data type: ' + typeof data);
            console.log('socket state: ' + ws.readyState);
            ws.send(data);
            //client.destroy();
        });
        client.on('close', function() {
            console.log('Close handler: Connection closed');
        })
        client.connect(COMMAND_PATH, function() {
            console.log('connected to: ' + COMMAND_PATH);
            //client.write("I am Chuck Norris!\n");
        });

	    ws.on('close',
	        function(code,reason)
	        {
		        console.log('websocket closed: code='+code+', reason = '+reason);
		        client.destroy();
	        });
	    
        ws.on('error',
	        function(error)
	        {
		        console.log('websocket error:'+error);
		        client.destroy();
	        });

        ws.on('message', 
            function incoming(message) 
            {
                var msg = JSON.parse(message);
                console.log('received msg: type='+msg.type+", payload: "+msg.payload);
                switch (msg.type) {
                    case "command":
                        client.write(msg.payload+"\n");
                        break;
                    case "echo":
                        echo(ws, msg.payload + " back at ya ");
                        break;
                    case "broadcast":
                        broadcast(ws, msg.payload);
                        break;
                    default:
                        console.log("unknown message type: %s", message);
                }
            });
    });

server.on('request', app);
server.listen(port, function () { console.log('HTTPD Listening on ' + server.address().port) });

*/
