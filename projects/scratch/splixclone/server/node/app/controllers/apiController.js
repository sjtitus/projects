/*_____________________________________________________________________________
 * apiController
 * Controller handling client API requests
 *_____________________________________________________________________________
*/
var express = require('express');
var router  = express.Router();
var net     = require('net');

// Grab the singleton command api instance that allows us to 
// send commands to the game engine
var CommandApi = require('../models/CommandApi').Instance();
var Player = require('../models/Player');


//______________________________________________________________________________
// createPlayer
// Create a server-side player. 
router.post('/createPlayer', 
  function (req, res, next) 
  {
    console.log('api/createPlayer: request to create a player');
    console.log('api/createPlayer: ip: '+req.ip); 
    console.log('api/createPlayer: hostname '+req.hostname); 
    console.log('api/createPlayer: method '+req.method); 
    console.log('api/createPlayer: baseUrl '+req.baseUrl); 
    console.log('api/createPlayer: username '+req.body.username); 
   

    newPlayer = new Player(req); 
    CommandApi.CreatePlayer(newPlayer);
 
    //res.setHeader('Content-Type', 'application/json');
    //res.send(JSON.stringify({ playerID: 111222 }));
  }
);


module.exports = router;





